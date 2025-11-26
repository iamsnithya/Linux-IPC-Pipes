
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "myfifo"

void server(char filename[]);
void client();

int main()
{
    char filename[50];
    pid_t pid;

    printf("ENTER THE FILE NAME :");
    scanf("%s", filename);

    // Create FIFO if not exists
    mkfifo(FIFO_FILE, 0666);

    pid = fork();

    if (pid > 0)
    {
        sleep(1); // Give time for client to start
        server(filename);
    }
    else if (pid == 0)
    {
        client();
    }
    else
    {
        perror("fork");
        exit(1);
    }

    return 0;
}

/* SERVER PROCESS - writes file content to FIFO */
void server(char filename[])
{
    int fd1, fd2;
    char buffer[1000];
    FILE *fp;

    printf("CLIENT SENDING THE REQUEST .... PLEASE WAIT\n");

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    fd1 = open(FIFO_FILE, O_WRONLY);
    if (fd1 < 0)
    {
        perror("Error opening FIFO");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        write(fd1, buffer, strlen(buffer));
    }

    fclose(fp);
    close(fd1);
}

/* CLIENT PROCESS - reads from FIFO and prints to screen */
void client()
{
    int fd2;
    char buffer[1000];
    ssize_t n;

    fd2 = open(FIFO_FILE, O_RDONLY);
    if (fd2 < 0)
    {
        perror("Error opening FIFO");
        exit(1);
    }

    printf("THE RESULTS OF CLIENTS ARE ......\n");

    while ((n = read(fd2, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    close(fd2);
}
