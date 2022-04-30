/* 
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org/>
 */

// A simple shell.
// Compile and run with:
// $ gcc -o simple-shell simple-shell.c
// $ ./simple-shell

// Ideas for enhancement:
//   - Implement `exit` command 
//   - Capture and handle the return value of commands

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ARGS_LEN 4096

void print_prompt();
size_t count_arg_len(char *line);
size_t count_leading_spaces(char *line);
void get_args(char *line, char **args);
int is_cd(char *command);
void change_directory(char **args);

int main(int argc, char *argv[]) 
{
    char *args[ARGS_LEN];
    char *command = NULL;
    char *line = NULL;
    size_t line_len = 0;

    while(1) {
        print_prompt();

        // Exit if EOF (or other error) is found
        if (getline(&line, &line_len, stdin) < 0)
            exit(0);

        get_args(line, args);
        command = args[0];

        // Exit if line is empty 
        if (command == NULL) 
            exit(0);

        // Is it cd? 
        if (is_cd(command)) {
            change_directory(args);
            continue;
        }

        pid_t rv = fork(); 
        if (rv == 0) {
            // I am child
 
            execvp(command, args); // This library function searches PATH for us

            perror("execvp");
            exit(1);
        } else if ( rv > 0 ) {
            // I am parent
 
            wait(NULL); // Wait for child process to terminate
        } else {
            perror("fork");
            exit(1);
        }
    }
}

void print_prompt() 
{
    printf("%c ", getuid() == 0 ? '#' : '$');

    return;
}

size_t count_arg_len(char *line) 
{
    size_t i = 0;
    while(line[i] && line[i] != ' ' && line[i] != '\n')
        ++i;

    return i;
}

size_t count_leading_spaces(char *line)
{
    int i = 0;
    while( line[i] == ' ' || line[i] == '\t' )
        ++i;

    return i;
}

void get_args(char *line, char **args)
{
    size_t args_i = 0;
    size_t arg_len = 0;

    line += count_leading_spaces(line);

    while( (arg_len = count_arg_len(line)) > 0 && args_i < ARGS_LEN - 1) {

        args[args_i] = line;
        ++args_i;

        line += arg_len;
        if (*line) {
            *line = '\0';
            ++line;
        }

        line += count_leading_spaces(line);
    }

    args[args_i] = NULL;

    return;
}

// Return true if command is cd, false otherwise
int is_cd(char *word)
{
    return strcmp(word, "cd") == 0 ? 1 : 0;
}

void change_directory(char **args )
{
    int rv;

    if (args[1] == NULL) {
        rv = chdir(getenv("HOME"));
    } else {
        rv = chdir(args[1]);
    }

    if (rv != 0) {
        perror("cd");
    }

    return;
}
