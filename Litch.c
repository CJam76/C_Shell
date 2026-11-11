#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/* Function Declarations for builtin shell commands: */
int litch_cd(char **args);
int litch_help(char **args);
int litch_exit(char **args);

/* List of built in commands and corresponding functions */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &litch_cd,
    &litch_help,
    &litch_exit
};

int litch_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/* Built in function implementations */
int litch_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "litch: expectd argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("litch");
        }
    }
    return 1;
}

int litch_help(char **args)
{
    int i;
    printf("Christian Flynn's LITCH\n");
    printf("Type program names and arguements, and hit enter:\n");
    printf("The following are built in:\n");

    for (i = 0; i < litch_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs:\n");
    return 1;
}

int litch_exit(char **args)
{
    return 0;
}

#define LITCH_RL_BUFSIZE 1024
char *litch_read_line(void)
{
    int bufsize = LITCH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "litch: allocation error\n");
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
                fprintf(stderr, "litch; allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define LITCH_TOK_BUFSIZE 64
#define LITCH_TOK_DELIM " \t\r\n\a"
char **litch_split_line(char *line)
{
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

        if (position >= bufsize) {
            bufsize += LITCH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "litch: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LITCH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int litch_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child Process
        if (execvp(args[0], args) == -1) {
            perror("litch");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("litch");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int litch_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < litch_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return litch_launch(args);
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
