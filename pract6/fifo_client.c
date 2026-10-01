#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define MAX_MSG 256

typedef struct
{
    pid_t client_pid;
    char message[MAX_MSG];
} Request;

typedef struct
{
    char response[MAX_MSG];
} Response;

int main(void)
{
    int server_fd, client_fd;
    char client_fifo[100];
    char message[MAX_MSG];

    Request request;
    Response response;

    pid_t pid = getpid();

    /* Create unique FIFO for this client */
    snprintf(client_fifo,
             sizeof(client_fifo),
             "/tmp/client_%d_fifo",
             pid);

    if (mkfifo(client_fifo, 0666) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Client PID: %d\n", pid);
    printf("Enter message: ");

    if (fgets(message, sizeof(message), stdin) == NULL)
    {
        unlink(client_fifo);
        return 1;
    }

    /* Remove newline */
    message[strcspn(message, "\n")] = '\0';

    request.client_pid = pid;

    strncpy(request.message,
            message,
            MAX_MSG - 1);

    request.message[MAX_MSG - 1] = '\0';

    /* Open server FIFO */
    server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("Unable to open server FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    /* Send request */
    if (write(server_fd, &request, sizeof(request)) == -1)
    {
        perror("write");
        close(server_fd);
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    close(server_fd);

    printf("Message sent to server.\n");

    /* Open client's FIFO for response */
    client_fd = open(client_fifo, O_RDONLY);

    if (client_fd == -1)
    {
        perror("Unable to open client FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    /* Receive response */
    if (read(client_fd, &response, sizeof(response)) > 0)
    {
        printf("Server Response: %s\n",
               response.response);
    }

    close(client_fd);

    /* Remove client's FIFO */
    unlink(client_fifo);

    return 0;
}
