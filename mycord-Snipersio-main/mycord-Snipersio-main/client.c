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

// Global settings used by the entire client for the duration of the program.
// Holds server address, socket file descriptor, flags, and the local username.
typedef struct Settings {
    struct sockaddr_in server;
    bool quiet;
    int socket_fd;
    bool running;
    char username[32];
} settings_t;

// Simple ANSI color codes for nicer terminal output.
// These are used only for printing and do not affect program behavior.
static char* COLOR_RED = "\033[31m";
static char* COLOR_GRAY = "\033[90m";
static char* COLOR_RESET = "\033[0m";
static settings_t settings = {0};

/* Improved message protocol definitions */

typedef enum MessageType {
    // Sent by client when first connecting.
    MSG_LOGIN        = 0,
    // Sent by client when voluntarily disconnecting.
    MSG_LOGOUT       = 1,
    // Sent by client when user types a chat message.
    MSG_MESSAGE_SEND = 2,
    // Sent by server to deliver a chat message to this client.
    MSG_MESSAGE_RECV = 10,
    // Sent by server to force this client to disconnect (e.g., duplicate username).
    MSG_DISCONNECT   = 12,
    // Sent by server for informational messages (server status, etc.).
    MSG_SYSTEM       = 13
} message_type_t;

// Packed message structure that both client and server agree on.
// All integer fields are in network byte order when transmitted.
typedef struct __attribute__((packed)) Message {
    uint32_t type;          // MessageType enum, in network byte order.
    uint32_t timestamp;     // UNIX timestamp, in network byte order.
    char username[32];      // Username associated with this message (if applicable).
    char message[1024];     // Message body or system text.
} message_t;

/* Global flags for clean shutdown */
static volatile sig_atomic_t got_signal = 0;      // Set when SIGINT/SIGTERM is received.
static volatile sig_atomic_t got_disconnect = 0;  // Set when server sends MSG_DISCONNECT.

/* Forward declarations from template */
int process_args(int argc, char *argv[]);
int get_username();
void handle_signal(int signal);
ssize_t perform_full_read(void *buf, size_t n);
void* receive_messages_thread(void* arg);

/* Additional helpers */
static void print_usage(const char *progname);
static ssize_t perform_full_write(const void *buf, size_t n);
static void print_timestamped_user_message(uint32_t net_ts,
                                           const char *user,
                                           const char *msg);

/* Usage text: describes all supported command-line options. */
static void print_usage(const char *progname) {
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

/* Robust CLI parsing with clear error messages.
 * Fills in 'settings.server' and 'settings.quiet' based on argv.
 * - Accepts either --ip OR --domain (but not both).
 * - Validates the port is in [1, 65535].
 * - Resolves domain names to an IPv4 address.
 */
int process_args(int argc, char *argv[]) {
    int port = 8080;          // Default port if none is provided.
    char *ip_str = NULL;      // Stores literal IP string if given.
    char *domain_str = NULL;  // Stores domain name if given.

    // Initialize server struct and default to IPv4 stream socket.
    memset(&settings.server, 0, sizeof(settings.server));
    settings.server.sin_family = AF_INET;
    settings.quiet = false;

    // Iterate over all command-line arguments starting from index 1.
    for (int i = 1; i < argc; i++) {
        // Handle help flags.
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            exit(0);

        // Handle port option, which must have a numeric argument.
        } else if (strcmp(argv[i], "--port") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --port requires a value\n");
                return -1;
            }
            char *endptr = NULL;
            long val = strtol(argv[++i], &endptr, 10);
            // Ensure the entire string is a number and within valid port range.
            if (*endptr != '\0' || val <= 0 || val > 65535) {
                fprintf(stderr, "Error: invalid port '%s'\n", argv[i]);
                return -1;
            }
            port = (int)val;

        // Handle IP address option.
        } else if (strcmp(argv[i], "--ip") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --ip requires a value\n");
                return -1;
            }
            ip_str = argv[++i];

        // Handle domain option.
        } else if (strcmp(argv[i], "--domain") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --domain requires a value\n");
                return -1;
            }
            domain_str = argv[++i];

        // Enable quiet mode, which disables mention highlighting.
        } else if (strcmp(argv[i], "--quiet") == 0) {
            settings.quiet = true;

        // Any other flag is unknown and causes an error.
        } else {
            fprintf(stderr, "Error: unknown argument '%s'\n", argv[i]);
            return -1;
        }
    }

    // It is invalid to specify both an IP address and a domain at the same time.
    if (ip_str && domain_str) {
        fprintf(stderr, "Error: --ip and --domain cannot both be specified\n");
        return -1;
    }

    // If neither IP nor domain is provided, default to localhost.
    if (!ip_str && !domain_str) {
        ip_str = (char *)"127.0.0.1";
    }

    if (domain_str) {
        // Resolve the domain name to an IPv4 address using gethostbyname.
        struct hostent *he = gethostbyname(domain_str);
        if (he == NULL || he->h_addrtype != AF_INET || he->h_addr_list[0] == NULL) {
            fprintf(stderr, "Error: failed to resolve domain '%s'\n", domain_str);
            return -1;
        }
        // Copy the resolved IP into the sockaddr_in structure.
        struct in_addr addr;
        memcpy(&addr, he->h_addr_list[0], sizeof(struct in_addr));
        settings.server.sin_addr = addr;
    } else {
        // Convert a text IPv4 address to binary form.
        if (inet_pton(AF_INET, ip_str, &settings.server.sin_addr) != 1) {
            fprintf(stderr, "Error: invalid IP address '%s'\n", ip_str);
            return -1;
        }
    }

    // Store the port in network byte order.
    settings.server.sin_port = htons((uint16_t)port);
    return 0;
}

/* Username retrieval with validation and clear errors.
 * Tries $USER first, and if not set uses `whoami`.
 * Then enforces that the username is non-empty and contains only ASCII alphanumerics.
 */
int get_username() {
    char buf[64] = {0};
    const char *env_user = getenv("USER");

    // Prefer the USER environment variable when available.
    if (env_user != NULL && env_user[0] != '\0') {
        strncpy(buf, env_user, sizeof(buf) - 1);
    } else {
        // Fall back to running "whoami" and reading its output.
        FILE *fp = popen("whoami", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error: failed to run whoami: %s\n", strerror(errno));
            return -1;
        }
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            fprintf(stderr, "Error: failed to read username from whoami\n");
            pclose(fp);
            return -1;
        }
        pclose(fp);
        // Strip trailing newline if present.
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    }

    // Reject completely empty usernames.
    if (buf[0] == '\0') {
        fprintf(stderr, "Error: username must not be empty\n");
        return -1;
    }

    // Ensure every character is ASCII and alphanumeric.
    for (size_t i = 0; buf[i] != '\0'; i++) {
        unsigned char c = (unsigned char)buf[i];
        if (c >= 128 || !isalnum(c)) {
            fprintf(stderr, "Error: username must be ASCII alphanumeric\n");
            return -1;
        }
    }

    // Copy the validated username into the global settings.
    strncpy(settings.username, buf, sizeof(settings.username) - 1);
    settings.username[sizeof(settings.username) - 1] = '\0';
    return 0;
}

/* Signal handler sets flags only (async-signal-safe).
 * We do not perform any I/O here; we just tell the main loop to shut down.
 */
void handle_signal(int signal) {
    (void)signal;      // Unused parameter; we don't care which signal exactly.
    got_signal = 1;   // Remember that we received a termination signal.
    settings.running = false;  // Ask the main loop and receiver thread to stop.
}

/* Ensure we read exactly n bytes unless EOF or error.
 * This function loops until:
 *   - 'n' bytes have been read (success)
 *   - read() returns 0 (remote side closed the connection)
 *   - read() returns a non-EINTR error (failure)
 */
ssize_t perform_full_read(void *buf, size_t n) {
    size_t total = 0;
    char *p = (char *)buf;

    while (total < n) {
        ssize_t r = read(settings.socket_fd, p + total, n - total);
        if (r < 0) {
            // If the read was interrupted by a signal, retry.
            if (errno == EINTR) {
                continue;
            }
            // Any other error means the read failed.
            return -1;
        }
        if (r == 0) {
            // End-of-file: the server closed the connection.
            break;
        }
        // Accumulate the number of bytes read so far.
        total += (size_t)r;
    }

    // Return how many bytes we actually got (may be less than n on EOF).
    return (ssize_t)total;
}

/* Ensure we write exactly n bytes unless error.
 * Loops until either n bytes have been written or write() fails.
 */
static ssize_t perform_full_write(const void *buf, size_t n) {
    size_t total = 0;
    const char *p = (const char *)buf;

    while (total < n) {
        ssize_t w = write(settings.socket_fd, p + total, n - total);
        if (w < 0) {
            // Retry on EINTR; fail on any other error.
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (w == 0) {
            // write() returning 0 usually indicates the connection is broken.
            break;
        }
        total += (size_t)w;
    }

    return (ssize_t)total;
}

/* Helper for consistent timestamp formatting and mention highlighting.
 * - Converts network-byte-order timestamp to host time_t.
 * - Formats time as "YYYY-MM-DD HH:MM:SS".
 * - Prints "user: message" line.
 * - If quiet mode is off, highlights any occurrences of "@<our-username>" in red and rings the terminal bell.
 */
static void print_timestamped_user_message(uint32_t net_ts,
                                           const char *user,
                                           const char *msg) {
    // Convert from network byte order to host and then to local time.
    time_t t = (time_t)ntohl(net_ts);
    struct tm *tm_info = localtime(&t);
    char time_buf[32];

    if (tm_info != NULL) {
        // Format the timestamp into a human-readable form.
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        // Fallback timestamp if localtime fails.
        strcpy(time_buf, "1970-01-01 00:00:00");
    }

    // Prefix each message with its timestamp and sender.
    printf("[%s] %s: ", time_buf, user);

    // If quiet mode is on or username is not set, print the message as-is.
    if (settings.quiet || settings.username[0] == '\0') {
        printf("%s\n", msg);
        fflush(stdout);
        return;
    }

    // Build "@username" pattern to scan for mentions.
    char pattern[40];
    pattern[0] = '@';
    strncpy(pattern + 1, settings.username, sizeof(pattern) - 2);
    pattern[sizeof(pattern) - 1] = '\0';

    const char *cursor = msg;
    const char *hit = strstr(cursor, pattern);

    // Walk through the message, printing segments and highlighting mentions.
    while (hit != NULL) {
        // Print everything before the mention normally.
        fwrite(cursor, 1, (size_t)(hit - cursor), stdout);
        // Print the mention in red with a bell character to draw attention.
        printf("\a%s", COLOR_RED);
        fwrite(pattern, 1, strlen(pattern), stdout);
        printf("%s", COLOR_RESET);
        // Advance cursor past this mention and search for the next.
        cursor = hit + strlen(pattern);
        hit = strstr(cursor, pattern);
    }

    // Print any remaining text after the last mention.
    printf("%s\n", cursor);
    fflush(stdout);
}

/* Thread that continuously receives messages from server.
 * - Performs full reads to avoid partial message_t structs.
 * - Dispatches behavior based on msg.type (in host byte order).
 * - Terminates on server close, protocol error, or disconnect request.
 */
void* receive_messages_thread(void* arg) {
    (void)arg;

    // Keep reading as long as the main program indicates we should run.
    while (settings.running) {
        message_t msg;
        // Try to read an entire message struct.
        ssize_t r = perform_full_read(&msg, sizeof(msg));
        if (r == 0) {
            // Server closed the connection gracefully.
            settings.running = false;
            break;
        } else if (r < 0) {
            // A true read error occurred.
            fprintf(stderr, "Error: failed to read from server: %s\n", strerror(errno));
            settings.running = false;
            break;
        } else if ((size_t)r < sizeof(msg)) {
            // Short read: we did not get a full message struct.
            fprintf(stderr, "Error: short read from server\n");
            settings.running = false;
            break;
        }

        // Ensure both username and message buffers are null-terminated
        // to avoid printing garbage if the server didn't fill them.
        msg.username[sizeof(msg.username) - 1] = '\0';
        msg.message[sizeof(msg.message) - 1] = '\0';

        // Convert type to host byte order so we can switch on it.
        uint32_t type = ntohl(msg.type);

        if (type == MSG_MESSAGE_RECV) {
            // Regular chat message from another user.
            print_timestamped_user_message(msg.timestamp, msg.username, msg.message);

        } else if (type == MSG_SYSTEM) {
            // System message from the server, printed in gray.
            printf("%s[SYSTEM] %s%s\n", COLOR_GRAY, msg.message, COLOR_RESET);
            fflush(stdout);

        } else if (type == MSG_DISCONNECT) {
            // Server is forcing us to disconnect (e.g., duplicate username).
            printf("%s[DISCONNECT] %s%s\n", COLOR_RED, msg.message, COLOR_RESET);
            fflush(stdout);
            got_disconnect = 1;
            settings.running = false;
            break;

        } else {
            // Unexpected type indicates a possible protocol mismatch or bug.
            fprintf(stderr, "Error: unknown message type %u\n", type);
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    /* Main client workflow:
     *   1. Install signal handlers for SIGINT/SIGTERM.
     *   2. Parse command-line arguments and configure server address.
     *   3. Obtain and validate the username.
     *   4. Create a TCP socket and connect to the server.
     *   5. Send an initial LOGIN message.
     *   6. Create a receiver thread to handle incoming messages.
     *   7. Enter a loop reading from stdin and sending MSG_MESSAGE_SEND messages.
     *   8. On exit, send LOGOUT (if needed), shut down the socket, and join the thread.
     */

    // 1. Setup sigaction (preferred over signal()) to catch SIGINT/SIGTERM.
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Initialize global state.
    settings.socket_fd = -1;
    settings.running = true;
    settings.username[0] = '\0';

    // 2. Parse arguments and configure settings.server.
    if (process_args(argc, argv) != 0) {
        return 1;
    }

    // 3. Determine the username used for messaging and login.
    if (get_username() != 0) {
        return 1;
    }

    // 4. Create a TCP socket using IPv4.
    settings.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (settings.socket_fd < 0) {
        fprintf(stderr, "Error: failed to create socket: %s\n", strerror(errno));
        return 1;
    }

    // 5. Connect to the server using the configured address and port.
    if (connect(settings.socket_fd,
                (struct sockaddr *)&settings.server,
                sizeof(settings.server)) < 0) {
        fprintf(stderr, "Error: failed to connect to server: %s\n", strerror(errno));
        close(settings.socket_fd);
        return 1;
    }

    // 6. Create and send the LOGIN message so the server knows our username.
    message_t login_msg;
    memset(&login_msg, 0, sizeof(login_msg));
    login_msg.type = htonl(MSG_LOGIN);
    // For now, timestamp is set to 0; the server can fill real timestamps.
    login_msg.timestamp = htonl(0);
    strncpy(login_msg.username, settings.username, sizeof(login_msg.username) - 1);
    login_msg.username[sizeof(login_msg.username) - 1] = '\0';

    // Send the login message and verify the full struct was written.
    if (perform_full_write(&login_msg, sizeof(login_msg)) != (ssize_t)sizeof(login_msg)) {
        fprintf(stderr, "Error: failed to send LOGIN: %s\n", strerror(errno));
        close(settings.socket_fd);
        return 1;
    }

    // 7. Start the background thread to receive messages from the server.
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages_thread, NULL) != 0) {
        fprintf(stderr, "Error: failed to create receive thread\n");
        close(settings.socket_fd);
        return 1;
    }

    // Buffer used by getline() for reading user input.
    char *line = NULL;
    size_t cap = 0;

    // 8. Main input loop: read lines from stdin and send as chat messages.
    while (settings.running) {
        errno = 0;
        // getline() allocates/resizes 'line' as needed and reads a full line.
        ssize_t n = getline(&line, &cap, stdin);
        if (n == -1) {
            if (feof(stdin)) {
                // User hit Ctrl+D (EOF). Exit the loop cleanly.
                break;
            }
            if (errno == EINTR) {
                // If interrupted by a signal, re-check running flag and continue.
                if (!settings.running) {
                    break;
                }
                continue;
            }
            // Any other error reading stdin is fatal for the input loop.
            fprintf(stderr, "Error: failed to read from stdin: %s\n", strerror(errno));
            settings.running = false;
            break;
        }

        // Strip the trailing newline, if present.
        if (n > 0 && line[n - 1] == '\n') {
            line[n - 1] = '\0';
            n--;
        }

        // Reject empty messages to match project expectations.
        if (n <= 0) {
            fprintf(stderr, "Error: message must not be empty\n");
            continue;
        }
        // Enforce the project limit of 1023 characters (leaving room for '\0').
        if (n > 1023) {
            fprintf(stderr, "Error: message too long (max 1023 characters)\n");
            continue;
        }

        // Validate that every character is printable ASCII.
        bool bad = false;
        for (ssize_t i = 0; i < n; i++) {
            unsigned char c = (unsigned char)line[i];
            if (!isprint(c)) {
                bad = true;
                break;
            }
        }
        if (bad) {
            fprintf(stderr, "Error: message must contain only printable ASCII characters\n");
            continue;
        }

        // If we were told to stop (e.g., from a disconnect message), exit loop.
        if (!settings.running || got_disconnect) {
            break;
        }

        // Construct MSG_MESSAGE_SEND with the user-provided text.
        message_t send_msg;
        memset(&send_msg, 0, sizeof(send_msg));
        send_msg.type = htonl(MSG_MESSAGE_SEND);
        send_msg.timestamp = htonl(0);  // Timestamp can be handled by the server.
        strncpy(send_msg.message, line, sizeof(send_msg.message) - 1);
        send_msg.message[sizeof(send_msg.message) - 1] = '\0';

        // Attempt to send the complete message struct to the server.
        if (perform_full_write(&send_msg, sizeof(send_msg)) != (ssize_t)sizeof(send_msg)) {
            fprintf(stderr, "Error: failed to send message: %s\n", strerror(errno));
            settings.running = false;
            break;
        }
    }

    // 9. If the server did not already send us a MSG_DISCONNECT, send a LOGOUT.
    if (!got_disconnect && settings.socket_fd >= 0) {
        message_t logout_msg;
        memset(&logout_msg, 0, sizeof(logout_msg));
        logout_msg.type = htonl(MSG_LOGOUT);
        logout_msg.timestamp = htonl(0);
        // We ignore write errors here because we are shutting down anyway.
        perform_full_write(&logout_msg, sizeof(logout_msg));
    }

    // 10. Tell the receiver thread to stop and shutdown the socket.
    settings.running = false;
    shutdown(settings.socket_fd, SHUT_RDWR);

    // Wait for the receiver thread to finish before closing.
    pthread_join(recv_thread, NULL);

    // Close the socket if it is still open.
    if (settings.socket_fd >= 0) {
        close(settings.socket_fd);
    }

    // Free the getline() buffer if allocated.
    free(line);

    // 11. Exit with success (or error code if we wanted to propagate more detail).
    return 0;
}

