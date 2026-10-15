#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // Load config files, if any

    // Run command loop
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
