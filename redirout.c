#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

    if (argc < 3) {
        printf("Error. 3 arguments required.");
        exit(0);
    }

    int fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    int pfd[2];
    pipe(pfd);
    pid_t pid = fork();

    if (pid == 0){
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(argv[2], argv + 2);
        exit(0);
    }

    close(pfd[1]);
    char *buf[2048];
    int bytes_read = read(pfd[0], buf, sizeof buf);

    while(bytes_read) {
        write(fd, buf, bytes_read);
        break;
    }
}