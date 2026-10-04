#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (100 * 1024 * 1024)

int main(void)
{
    char *data = malloc(SIZE);

    if (data == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < SIZE; i++)
    {
        data[i] = 1;
    }

    printf("Parent PID: %d\n", getpid());
    printf("Allocated and initialized 100 MB.\n");
    printf("Press Enter to fork...\n");
    getchar();

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(data);
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        printf("Press Enter to modify memory...\n");
        getchar();

        for (size_t i = 0; i < SIZE; i += 4096)
        {
            data[i] = 2;
        }

        printf("Child modified one byte every 4096 bytes.\n");
        printf("Press Enter to exit...\n");
        getchar();

        free(data);
        return 0;
    }
    else
    {
        printf("\nParent waiting for child...\n");
        wait(NULL);

        free(data);

        printf("Parent: child finished.\n");
    }

    return 0;
}
