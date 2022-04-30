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


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

void check_1();
void check_2();
void check_3();

int main() {
    //check_1();
    check_2();
    //check_3();

    return 0;
}

// fork()
void check_1() {
    pid_t rv = fork();      // One process executes this line
    if(rv == 0) {           // Two processes execute this line
        printf("Child proc PID = %zu\n", getpid());
    } else if (rv > 0) {
        printf("Parent proc PID = %zu\n", getpid());
    }
}


// open()
void check_2() {
    int fd = open("rubber_ducky", O_RDONLY);
    printf("fd = %d\n", fd);

    if( fd == -1 ) {
        perror("open");
        exit(1);
    }

    int buff_size = 4096;
    char *buff = malloc(buff_size);
    if( buff == NULL ) {
        perror("malloc");
        exit(2);
    }

    int num_bytes_read = read(fd, buff, buff_size);
    printf("num_bytes_read = %zu\n", num_bytes_read);
}

// fork and exec
void check_3() {
    pid_t rv = fork();
    if( rv == 0 ) {
        char *args[] = {"ls", "-l", NULL};
        execv("/usr/bin/ls", args);
        
        perror("execv"); // This line is executed only if execv failed
        exit(1);
    } else if ( rv > 0 ) {
        wait(NULL);
        printf("Child process %zu exited!\n", (unsigned long) rv);
    } 
}
