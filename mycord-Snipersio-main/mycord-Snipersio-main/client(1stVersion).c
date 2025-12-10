#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>
#include <stdint.h>

typedef struct Settings {
    struct sockaddr_in server;
    bool quiet;
    int socket_fd;
    bool running;
    char username[32];
} settings_t;

static char* COLOR_RED = "\033[31m";
static char* COLOR_GRAY = "\033[90m";
static char* COLOR_RESET = "\033[0m";
static settings_t settings = {0};

typedef enum MessageType { //Message Protocols
    MSG_LOGIN = 0,
    MSG_LOGOUT = 1,
    MSG_MESSAGE_SEND = 2,
    MSG_MESSAGE_RECV = 10,
    MSG_DISCONNECT = 12,
    MSG_SYSTEM = 13
} message_type_t;

typedef struct __attribute__((packed)) Message {//Message Structure
    uint32_t type;
    uint32_t timestamp;
    char username[32];
    char message[1024];
} message_t;

static volatile sig_atomic_t got_signal = 0; //flags for shuting down
static volatile sig_atomic_t got_disconnect = 0;

int process_args(int argc, char *argv[]);//Function Declarations
int get_username();
void handle_signal(int signal);
ssize_t perform_full_read(void *buf, size_t n);
void* receive_messages_thread(void* arg);

static void print_usage(const char *progname) {//Help Menu
    printf("usage: %s [-h] [--port PORT] [--ip IP] [--domain DOMAIN] [--quiet]\n"
           "\n"
           "mycord client\n"
           "\n"
           "options:\n"
           "  --help                show this help message and exit\n"
           "  --port PORT           port to connect to (default: 8080)\n"
           "  --ip IP               IP to connect to (default: \"127.0.0.1\")\n"
           "  --domain DOMAIN       Domain name to connect to (if domain is specified, IP must not be)\n"
           "  --quiet               do not perform alerts or mention highlighting\n"
           "\n"
           "examples:\n"
           "  %s --help\n"
           "  %s --port 1738\n"
           "  %s --domain example.com\n",
           progname, progname, progname, progname);
}

int process_args(int argc, char *argv[]) { //Argument parsing
    int port = 8080;
    char *ip_str = NULL;
    char *domain_str = NULL;
    memset(&settings.server, 0, sizeof(settings.server)); //Initializing server settings
    settings.server.sin_family = AF_INET;
    settings.quiet = false;
    for (int i = 1; i < argc; i++) { //Parsing through arguments
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "--port") == 0) { //Port argument
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --port requires a value\n");
                return -1;
            }
            char *endptr = NULL;
            long val = strtol(argv[++i], &endptr, 10);
            if (*endptr != '\0' || val <= 0 || val > 65535) { //Validating the port
                fprintf(stderr, "Error: invalid port '%s'\n", argv[i]);
                return -1;
            }
            port = (int)val;
        } else if (strcmp(argv[i], "--ip") == 0) { //IP argument
            if (i + 1 >= argc) { 
                fprintf(stderr, "Error: --ip requires a value\n");
                return -1;
            }
            ip_str = argv[++i];
        } else if (strcmp(argv[i], "--domain") == 0) { //Domain argument
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --domain requires a value\n");
                return -1;
            }
            domain_str = argv[++i];
        } else if (strcmp(argv[i], "--quiet") == 0) { //Quiet mode
            settings.quiet = true;
        } else {
            fprintf(stderr, "Error: unknown argument '%s'\n", argv[i]); //unkown arugument
            return -1;
        }
    }
    if (ip_str && domain_str) { //Can't have both IP and Domain
        fprintf(stderr, "Error: --ip and --domain cannot both be specified\n");
        return -1;
    }
    if (!ip_str && !domain_str) { //Defaults to localhost
        ip_str = "127.0.0.1";
    }
    if (domain_str) { //domain to IP
        struct hostent *he = gethostbyname(domain_str);
        if (he == NULL || he->h_addrtype != AF_INET || he->h_addr_list[0] == NULL) { //if failed
            fprintf(stderr, "Error: failed to resolve domain '%s'\n", domain_str);
            return -1;
        }
        struct in_addr addr;
        memcpy(&addr, he->h_addr_list[0], sizeof(struct in_addr)); //copy the IP address
        settings.server.sin_addr = addr;
    } else { //IP as a string to binary
        if (inet_pton(AF_INET, ip_str, &settings.server.sin_addr) != 1) { 
            fprintf(stderr, "Error: invalid IP address '%s'\n", ip_str);
            return -1;
        }
    }
    settings.server.sin_port = htons((uint16_t)port);
    return 0;
}

int get_username() { //Get username from env or whoami
    char buf[64] = {0};
    const char *env_user = getenv("USER");
    if (env_user != NULL && env_user[0] != '\0') { //From env
        strncpy(buf, env_user, sizeof(buf) - 1);
    } else { //From whoami
        FILE *fp = popen("whoami", "r");
        if (fp == NULL) { //whoami failed to run
            fprintf(stderr, "Error: failed to run whoami: %s\n", strerror(errno));
            return -1;
        }
        if (fgets(buf, sizeof(buf), fp) == NULL) { //failed to read username from whoami
            fprintf(stderr, "Error: failed to read username from whoami\n");
            pclose(fp);
            return -1;
        }
        pclose(fp);
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    }
    if (buf[0] == '\0') { //Empty username checker
        fprintf(stderr, "Error: username must not be empty\n");
        return -1;
    }
    for (size_t i = 0; buf[i] != '\0'; i++) { //Validating the username characters
        unsigned char c = (unsigned char)buf[i];
        if (c >= 128 || !isalnum(c)) {
            fprintf(stderr, "Error: username must be ASCII alphanumeric\n");
            return -1;
        }
    }
    strncpy(settings.username, buf, sizeof(settings.username) - 1); //Copying the username settings
    settings.username[sizeof(settings.username) - 1] = '\0';
    return 0;
}

void handle_signal(int signal) { //Handler for graceful shutdown
    (void)signal;
    got_signal = 1;
    settings.running = false;
}

ssize_t perform_full_read(void *buf, size_t n) { //Helper func to read n bytes from socket
    size_t total = 0;
    char *p = (char *)buf;
    while (total < n) { //Loops until n bytes are read
        ssize_t r = read(settings.socket_fd, p + total, n - total);
        if (r < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (r == 0) {
            break;//EOF
        }
        total += (size_t)r;
    }
    return (ssize_t)total;
}

static void print_user_message(uint32_t net_timestamp, const char *username, const char *message) { //Prints the message with timestap and highlights
    time_t t = (time_t)ntohl(net_timestamp);
    struct tm *tm_info = localtime(&t);
    char time_buf[32];
    if (tm_info != NULL) { //Formats the timestamp
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    } else { //In case of weird timestamp
        strcpy(time_buf, "1970-01-01 00:00:00");
    }
    printf("[%s] %s: ", time_buf, username);
    if (settings.quiet || settings.username[0] == '\0') { //If in quiet mode or theres no username then print normally
        printf("%s\n", message);
        fflush(stdout);
        return;
    }
    char pattern[40]; //Building the @username pattern for highlighting
    pattern[0] = '@';
    strncpy(pattern + 1, settings.username, sizeof(pattern) - 2);
    pattern[sizeof(pattern) - 1] = '\0';
    const char *cursor = message;
    const char *hit = strstr(cursor, pattern);
    while (hit != NULL) { //Highlights all showings of @username
        fwrite(cursor, 1, (size_t)(hit - cursor), stdout);
        printf("\a%s", COLOR_RED); //Bell and red highlight
        fwrite(pattern, 1, strlen(pattern), stdout);
        printf("%s", COLOR_RESET);
        cursor = hit + strlen(pattern);
        hit = strstr(cursor, pattern);
    }
    printf("%s\n", cursor);
    fflush(stdout); //Flush the output
}

void* receive_messages_thread(void* arg) { //Thread for recieving messages from server
    (void)arg;
    while (settings.running) {
        message_t msg;
        ssize_t r = perform_full_read(&msg, sizeof(msg));
        if (r == 0) { 
            settings.running = false; //Server closed connection
            break;
        } else if (r < 0) { //Error reading from server
            fprintf(stderr, "Error: failed to read from server: %s\n", strerror(errno));
            settings.running = false;
            break;
        } else if ((size_t)r < sizeof(msg)) { //checking if short read
            fprintf(stderr, "Error: short read from server\n");
            settings.running = false;
            break;
        }
        msg.username[sizeof(msg.username) - 1] = '\0'; //Null terminate the username
        msg.message[sizeof(msg.message) - 1] = '\0';
        uint32_t type = ntohl(msg.type);
        if (type == MSG_MESSAGE_RECV) { //message types
            print_user_message(msg.timestamp, msg.username, msg.message);
        } else if (type == MSG_SYSTEM) {
            printf("%s[SYSTEM] %s%s\n", COLOR_GRAY, msg.message, COLOR_RESET);
            fflush(stdout);
        } else if (type == MSG_DISCONNECT) {
            printf("%s[DISCONNECT] %s%s\n", COLOR_RED, msg.message, COLOR_RESET);
            fflush(stdout);
            got_disconnect = 1;
            settings.running = false;
            break;
        } else {
            fprintf(stderr, "Error: unknown message type %u\n", type);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) { //Main
    struct sigaction sa; //Setting up singal handlers
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    settings.socket_fd = -1;
    settings.running = true;
    settings.username[0] = '\0';
    if (process_args(argc, argv) != 0) { //parse arguments
        return 1;
    }
    if (get_username() != 0) { //Get username
        return 1;
    }
    settings.socket_fd = socket(AF_INET, SOCK_STREAM, 0); //Creates the socket
    if (settings.socket_fd < 0) { //Error when creating the socket
        fprintf(stderr, "Error: failed to create socket: %s\n", strerror(errno));
        return 1;
    }
    if (connect(settings.socket_fd, (struct sockaddr *)&settings.server, sizeof(settings.server)) < 0) { //Connects to the server
        fprintf(stderr, "Error: failed to connect to server: %s\n", strerror(errno));
        close(settings.socket_fd);
        return 1;
    }
    message_t login_msg; //Creates and sends login information
    memset(&login_msg, 0, sizeof(login_msg));
    login_msg.type = htonl(MSG_LOGIN);
    login_msg.timestamp = htonl(0); //Timestamp time of 0 for login
    strncpy(login_msg.username, settings.username, sizeof(login_msg.username) - 1); //Copys username
    login_msg.username[sizeof(login_msg.username) - 1] = '\0';
    ssize_t w = write(settings.socket_fd, &login_msg, sizeof(login_msg)); //Sends the login message
    if (w < 0) { //Error sending the login message
        fprintf(stderr, "Error: failed to send LOGIN: %s\n", strerror(errno));
        close(settings.socket_fd);
        return 1;
    }
    pthread_t thread; //Creates and starts receiving the message thread
    if (pthread_create(&thread, NULL, receive_messages_thread, NULL) != 0) { //Error when creating the thread
        fprintf(stderr, "Error: failed to create receive thread\n");
        close(settings.socket_fd);
        return 1;
    }
    char *line = NULL; //Main input loop
    size_t cap = 0;
    while (settings.running) { //Loops until shutdown
        errno = 0;
        ssize_t n = getline(&line, &cap, stdin);
        if (n == -1) {
            if (feof(stdin)) { //EOF from user
                break;
            }
            if (errno == EINTR) { //Interrupted by a signal
                if (!settings.running) {
                    break;
                }
                continue;
            }
            fprintf(stderr, "Error: failed to read from stdin: %s\n", strerror(errno));
            settings.running = false;
            break;
        }
        if (n > 0 && line[n - 1] == '\n') { //Removes trailing newline
            line[n - 1] = '\0';
            n--;
        }
        if (n <= 0) { //Checks if empty message
            fprintf(stderr, "Error: message must not be empty\n");
            continue;
        }
        if (n > 1023) { //Checks if message is too long
            fprintf(stderr, "Error: message too long (max 1023 characters)\n");
            continue;
        }
        bool bad = false;
        for (ssize_t i = 0; i < n; i++) { //Validates the message characters
            unsigned char c = (unsigned char)line[i];
            if (!isprint(c)) { //Not printable character
                bad = true;
                break;
            }
        }
        if (bad) {
            fprintf(stderr, "Error: message must contain only printable ASCII characters\n");
            continue;
        }
        if (!settings.running || got_disconnect) { //If still running
            break;
        }
        message_t send_msg; //Sends the message to the server
        memset(&send_msg, 0, sizeof(send_msg));
        send_msg.type = htonl(MSG_MESSAGE_SEND); //Message type
        send_msg.timestamp = htonl(0);
        strncpy(send_msg.message, line, sizeof(send_msg.message) - 1);
        send_msg.message[sizeof(send_msg.message) - 1] = '\0';
        ssize_t ws = write(settings.socket_fd, &send_msg, sizeof(send_msg)); //Write the message to the socket
        if (ws < 0) { //Error sending the message
            fprintf(stderr, "Error: failed to send message: %s\n", strerror(errno));
            settings.running = false;
            break;
        }
    }
    if (!got_disconnect && settings.socket_fd >= 0) { //Sends a logout message if not already disconnected
        message_t logout_msg;
        memset(&logout_msg, 0, sizeof(logout_msg));
        logout_msg.type = htonl(MSG_LOGOUT);
        logout_msg.timestamp = htonl(0);
        write(settings.socket_fd, &logout_msg, sizeof(logout_msg));
    }
    settings.running = false; //Shuts down the receiving thread
    shutdown(settings.socket_fd, SHUT_RDWR);
    pthread_join(thread, NULL);
    if (settings.socket_fd >= 0) { //Closes the socket
        close(settings.socket_fd);
    }
    free(line);
    return 0;
}

