#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    // Load config files, if any
    //
    // Run command loop
    litch_loop();
    //
    // perform any shutdown/cleanup
    //
    return EXIT_SUCCESS;
}

void litch_loop(void)
{
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

#define LITCH_RL_BUFSIZE 1024
char *litch_read_line(void)
{
    int bufsize = LITCH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprint(stderr, "litch: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate
        if (position >= bufsize) {
            bufsize += LITCH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprint(stderr, "litch; allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// New way of doing litch_read_line
// char *litch_read_line(void)
// {
//  char *line = NULL;
//  ssize_t bufsize = 0; // have get line allocate a buffer for us
//
//  if (getline(&line; &bufsize, stdin) == 1) {
//      if (feof(stdin)) {
//          exit(EXIT_SUCCESS); // We received an EOF
//       } else {
//          perror("readline");
//          exit(EXIT_FAILURE);
//       }
//      }
//      return line;
// }
