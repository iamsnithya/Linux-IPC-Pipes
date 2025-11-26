#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void server(int, int);
void client(int, int);

int main() {
    int p1[2], p2[2], pid;

    pipe(p1);
    pipe(p2);
    pid = fork();

    if (pid == 0) {
        // Child process - acts as SERVER
        close(p1[1]);  // Close write end of pipe1
        close(p2[0]);  // Close read end of pipe2
        server(p1[0], p2[1]);
        exit(0);
    } else {
        // Parent process - acts as CLIENT
        close(p1[0]);  // Close read end of pipe1
        close(p2[1]);  // Close write end of pipe2
        client(p1[1], p2[0]);
        wait(NULL);
    }
    return 0;
}

void server(int read_fd, int write_fd) {
    int n;
    char fname[200], buff[2000];

    // Read filename from pipe
    n = read(read_fd, fname, sizeof(fname));
    fname[n] = '\0';

    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        write(write_fd, "Cannot open file", 16);
    } else {
        n = read(fd, buff, sizeof(buff));
        write(write_fd, buff, n);
        close(fd);
    }
}

void client(int write_fd, int read_fd) {
    int n;
    char fname[200], buff[2000];

    printf("ENTER THE FILE NAME : ");
    scanf("%s", fname);

    printf("CLIENT SENDING THE REQUEST .... PLEASE WAIT\n");

    write(write_fd, fname, strlen(fname));

    n = read(read_fd, buff, sizeof(buff));
    buff[n] = '\0';

    printf("THE RESULTS OF CLIENTS ARE ......\n");
    printf("%s\n", buff);
}// C Program that illustrate communication between two process using unnamed pipes
