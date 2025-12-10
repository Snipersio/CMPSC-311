#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

/*
This is the implementation of the highlighter program.
Please read the documentation in the README before you start working on this file.
*/

// Color codes to use in your program. Print a color first, then your text, then the reset color.
static const char* COLOR_RED = "\033[0;31m";
static const char* COLOR_GREEN = "\033[0;32m";
static const char* COLOR_BLUE = "\033[0;34m";
static const char* COLOR_YELLOW = "\033[1;33m";
static const char* COLOR_MAGENTA = "\033[0;35m";
static const char* COLOR_CYAN = "\033[0;36m";
static const char* COLOR_WHITE = "\033[1;37m";
static const char* COLOR_RESET_COLOR = "\033[0m";


// These function prototypes / definitions are suggestions but not required to implement.
// typedef struct Settings with fields FILE*/handle input_stream, output_stream; strings search_text, prefix, postfix; and boolean no_color;
// void print_help()
// void print_error(const char* error_message)
// void output_final_result(int count, const char* search_text)
// const char* get_color_code(const char* color_str)
// int process_args(int argc, char* argv[], Settings* settings)
// int parse_line(Settings* settings, char* line)
// ...


typedef struct { //Struct that contains all settings
    FILE *input_stream;
    FILE *output_stream;
    char *search_text;
    char *prefix;
    char *postfix;
    const char *color_code;
    bool no_color;
} Settings;

void print_help() { //Prints the help/usage message to stdout
    printf(
        "Usage:\n"
        "  ./highlighter [OPTIONS] TEXT\n\n"

        "Description:\n"
        "  Highlights all occurrences of TEXT from input to output.\n\n"

        "Positional Arguments:\n"
        "  TEXT               Text to search for and highlight.\n\n"

        "Options:\n"
        "  -h, --help         Show this help message and exit.\n"
        "  -i, --input FILE   Read input from FILE (default: stdin).\n"
        "  -o, --output FILE  Write output to FILE (default: stdout).\n"
        "  -c, --color COLOR  Highlight color (default: RED).\n"
        "                     Available: RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE.\n"
        "  --no-color         Disable color output.\n"
        "  --prefix TEXT      Add TEXT before each highlighted match.\n"
        "  --postfix TEXT     Add TEXT after each highlighted match.\n\n"

        "Examples:\n"
        "  ./highlighter -i file.txt \"error\"\n"
        "  ./highlighter -c GREEN \"warning\"\n"
        "  ./highlighter --prefix \">>\" --postfix \"<<\" \"keyword\" -o out.txt\n"
    );
} 

void print_error(const char* msg) { //If an error occurs then prints the error message to stderr
    fprintf(stderr, "Error: %s\n", msg);
}

const char* get_color_code(const char* color) { //Maps all the possible valid colors and returns their actual color values
    if (strcmp(color, "RED") == 0) return COLOR_RED;
    if (strcmp(color, "GREEN") == 0) return COLOR_GREEN;
    if (strcmp(color, "BLUE") == 0) return COLOR_BLUE;
    if (strcmp(color, "YELLOW") == 0) return COLOR_YELLOW;
    if (strcmp(color, "MAGENTA") == 0) return COLOR_MAGENTA;
    if (strcmp(color, "CYAN") == 0) return COLOR_CYAN;
    if (strcmp(color, "WHITE") == 0) return COLOR_WHITE;
    return NULL;
}

int process_args(int argc, char* argv[], Settings* s) { //Processes all command line arguments and sets the settings struct correctly
    s->input_stream = stdin; //Initilizing all settings to default values
    s->output_stream = stdout;
    s->prefix = "";
    s->postfix = "";
    s->color_code = COLOR_RED;
    s->no_color = false;
    s->search_text = NULL;
    for (int i = 1; i < argc; i++) { //Loops through all aruguments
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { //If help flag is seen
            print_help();
            exit(0);
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {//If input flag is seen
            if (i + 1 >= argc) { //No file given after the flag
                print_error("Missing input file after -i/--input");
                return 1;
            }
            s->input_stream = fopen(argv[++i], "r"); //Opens the file
            if (!s->input_stream) { //If the file couldn't be opened
                print_error("Could not open input file");
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) { //If the output flag is seen
            if (i + 1 >= argc) { //No file given after the flag
                print_error("Missing output file after -o/--output");
                return 1;
            }
            s->output_stream = fopen(argv[++i], "w");
            if (!s->output_stream) { //The file couldn't be opened
                print_error("Could not open output file");
                return 1;
            }
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0) { //If the color flag is seen
            if (i + 1 >= argc) { //No color given after the flag
                print_error("Missing color argument");
                return 1;
            }
            const char* color = argv[++i]; //Gets colors string
            const char* code = get_color_code(color); //Gets the actual color code
            if (!code) { //If color is invalid
                print_error("Invalid color");
                return 1;
            }
            s->color_code = code;
        } else if (strcmp(argv[i], "--no-color") == 0) { //If no color flag is seen
            s->no_color = true;
        } else if (strcmp(argv[i], "--prefix") == 0) { //If prefix flag is seen
            if (i + 1 >= argc) {
                print_error("Missing prefix argument");
                return 1;
            }
            s->prefix = argv[++i];
        } else if (strcmp(argv[i], "--postfix") == 0) { //If postfix flag is seen
            if (i + 1 >= argc) {
                print_error("Missing postfix argument");
                return 1;
            }
            s->postfix = argv[++i];
        } else if (argv[i][0] == '-') { //If invalid flag is seen
            print_error("Invalid flag");
            return 1;
        } else { 
            if (s->search_text != NULL) { //If a multi positional aregument was given
                print_error("Multiple positional arguments provided");
                return 1;
            }
            s->search_text = argv[i]; //Sets the search text
        }
    }
    if (!s->search_text) { //If no positional argument was given
        print_error("Missing text argument to highlight");
        return 1;
    }
    if (strlen(s->search_text) == 0) { //If search text is empty
        print_error("Empty search text");
        return 1;
    }
    return 0;
}

int parse_line(Settings* s, char* line) { //parses a line and highlights all found matches of given text and returns the number of times the match was found
    int count = 0; //Initialzing data
    char* start = line;
    char* match;
    while ((match = strstr(start, s->search_text)) != NULL) { //While there's still matches in the line
        fwrite(start, 1, match - start, s->output_stream); //Prints everything before the match
        fputs(s->prefix, s->output_stream); //print prefix
        if (!s->no_color) { //Print match with color if no_color is false
            fprintf(s->output_stream, "%s%.*s%s", s->color_code, (int)strlen(s->search_text), match, COLOR_RESET_COLOR);
        } else { //Prints match without color
            fprintf(s->output_stream, "%.*s", (int)strlen(s->search_text), match);
        }
        fputs(s->postfix, s->output_stream); //Print postfix
        count++;
        start = match + strlen(s->search_text);
    }
    fputs(start, s->output_stream); //prints remainder
    return count;
}

int main(int argc, char *argv[]) {//The main function
     Settings settings; //Import settings
    if (process_args(argc, argv, &settings) != 0) { //Proccess arguments and check for errors
        return 1;
    }
    char *line = NULL; //Initialize data for reading lines
    size_t len = 0;
    ssize_t nread;
    int total_matches = 0;
    while ((nread = getline(&line, &len, settings.input_stream)) != -1) { //Reads each line from the input
        total_matches += parse_line(&settings, line); //Parses the line and adds to total matches
    }
    if (settings.input_stream != stdin) fclose(settings.input_stream); //Closes files if not stdin/stdout
    if (settings.output_stream != stdout) fclose(settings.output_stream);
    fprintf(stderr, "Highlighted %d matches.\n", total_matches);
    free(line);
    return 0;
}



