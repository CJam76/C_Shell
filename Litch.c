#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // Load config files, if any

    // Run command loop

    // Readline function without using getline() function from C Library.

    // #define LITCH_RL_BUFSIZE 1024
    // char *litch_read_line(void)
    // {
    //     int bufsize = LITCH_RL_BUFSIZE;
    //     int position = 0;
    //     char *buffer = malloc(sizeof(char) * bufsize);
    //     int c;

    //     if (!buffer) {
    //         fprintf(stderr, "Litch: allocation error\n");
    //         exit(EXIT_FAILURE);
    //     }

    //     while (1){
    //         // Read a character
    //         c = getchar();

    //         // If we hit EOF, replace it with a null character and return.
    //         if (c == EOF||c == '\n') {
    //             buffer[position] = '\0';
    //             return buffer;
    //         } else {
    //             buffer[position] = c;
    //         }
    //         position++;

    //         // If we have exceeded the buffer, reallocate.
    //         if (position >= bufsize) {
    //             bufsize += LITCH_RL_BUFSIZE;
    //             buffer = realloc(buffer, bufsize);
    //             if (!buffer) {
    //                 fprintf(stderr, "Litch: allocation error\n");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }
    // }

    char *litch_read_line(void) {
        char *line = NULL;
        ssize_t bufsize = 0; // have getline allocate a buffer for us

        if (getline(&line, &bufsize, stdin) == -1) {
            if (feof(stdin)) {
                exit(EXIT_SUCCESS); // We recieved an EOF
            } else {
                perror("readline");
                exit(EXIT_FAILURE);
            }
        }
        return line;
    }

    #define LITCH_TOK_BUFSIZE 64
    #define LITCH_TOK_DELIM " \t\r\n\a"
    char **litch_split_line(char *line){
        int bufsize = LITCH_TOK_BUFSIZE, position = 0;
        char **tokens = malloc(bufsize * sizeof(char*));
        char *token;

        if (!tokens) {
            fprintf(stderr, "litch: allocation error\n");
            exit(EXIT_FAILURE);
        }

        token = strtok(line, LITCH_TOK_DELIM);
        while (token != NULL) {
            tokens[position] = token;
            position++;

            if (position >=bufsize) {
                bufsize += LITCH_TOK_BUFSIZE;
                tokens = realloc(tokens, bufsize * sizeof(char*));
                if (!tokens) {
                    fprintf(stderr, "litch: allocation error\n");
                    exit(EXIT_FAILURE);
                }
            }

            token = strtok(NULL, LITCH_TOK_DELIM);

        }
    }

    void litch_loop(void) {
        char *line;
        char **args;
        int status;

        do {
            printf("> ");
            line = litch_read_line();
            args = litch_split_line(line);
            status = litch_execute(args);

            free(line);
            free(args);
        } while (status);
    }

    // Perform any shutdown/cleanup

    return EXIT_SUCCESS;
}
