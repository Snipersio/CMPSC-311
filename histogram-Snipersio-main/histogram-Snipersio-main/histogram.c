#include "histogram.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

// These function prototypes / definitions are suggestions but not required to implement.
// hist_int_t get_max_count(void)
// void output_histogram(FILE* destination_stream)
// void usr1_handler/exit_handler/signal_handler(int signum)

// Note: It is bad practice to use STDIO in signal handlers. 
// For this assignment however, I am guaranteeing that multiple signals will not be delivered at the same time,
// thus it is __okay__ to use STDIO in your signal handlers.
// That is to say, feel free to use printf, fprintf, etc. in your signal handlers.
// Challenge: For the astute, try to implement the histogram program without using STDIO in signal handlers
// (e.g., by setting a volatile/flag). Even better if you use sigaction instead of signal.


static hist_int_t get_max_count(void) { //Find max count currently in histogram
    hist_int_t max = 0;
    for (int i = 0; i < 256; i++) { //Itterate through histogram
        if (histogram[i] > max) {
            max = histogram[i];
        }
    }
    return max; //returns max count
}

static void output_histogram(FILE *destination_stream) { //Outputs histogram to a stream
    hist_int_t max_count = get_max_count();
    for (int i = 0; i < 256; i++) { //Itterate through histogram
        hist_int_t count = histogram[i];
        fprintf(destination_stream, "%20llu 0x%02X |", //Prints count and byte value
                (unsigned long long) count, i);
        if (max_count > 0 && count > 0) { //If max count and current count are greater than 0 then print bar
            int bar_width = (int) ((count * MAX_BAR_WIDTH) / max_count);
            for (int j = 0; j < bar_width; j++) { //Prints the bar
                fputc('#', destination_stream);
            }
        }
        if (count > 0) { //If the current count is greater than 0 then print end of bar
            fputc('|', destination_stream);
        }
        fputc('\n', destination_stream);
    }
    fflush(destination_stream); //Makes sure all output is written
}

static void dumpHistogram_MaybeExit(int should_exit, int exit_status) { //Dumps histogram to file and exits if needed
    FILE *fp = fopen("histo.out", "w"); //opens file for writting
    if (fp == NULL) { //If the file can't be opened then erorr
        perror("fopen");
        if (should_exit) {
            exit(exit_status);
        }
        return;
    }
    output_histogram(fp);
    fclose(fp);
    if (should_exit) { //If exits then shows exit status
        exit(exit_status);
    }
}

static void handle_sigusr1(int signum) { //Handles the sigurs1 signal
    (void) signum; 
    dumpHistogram_MaybeExit(0, 0);
}

static void handle_termination(int signum) { //Handles termination signals
    (void) signum; // unused
    dumpHistogram_MaybeExit(1, 0);
}

int main(void) {
    if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR) { //Sets up all the signal handlers
        perror("signal SIGUSR1");
        return 1;
    }
    if (signal(SIGINT, handle_termination) == SIG_ERR) {
        perror("signal SIGINT");
        return 1;
    }
    if (signal(SIGTERM, handle_termination) == SIG_ERR) {
        perror("signal SIGTERM");
        return 1;
    }
    unsigned char buffer[4096]; //Creats buffer for reading
    ssize_t bytes_read;
    for (;;) { //A loop to read from stdin
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));//Reads fom stdin into buffer
        if (bytes_read == 0) { //EOF is reached so outputs the histogram and exits
            output_histogram(stdout);
            return 0;
        } else if (bytes_read < 0) { //Handling errors from read
            if (errno == EINTR) {
                continue;
            } else {
                perror("read");
                return 1;
            }
        } else {
            for (ssize_t i = 0; i < bytes_read; i++) { //Updates histogram with read bytes
                histogram[buffer[i]]++;
            }
        }
    }
    return 0; //In case there's a very weird input
}

