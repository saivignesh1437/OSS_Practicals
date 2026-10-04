#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
    FILE *src;
    FILE *dest;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long file_size;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    src = fopen(argv[1], "rb");
    if (src == NULL)
    {
        perror("fopen source");
        return 1;
    }

    dest = fopen(argv[2], "wb");
    if (dest == NULL)
    {
        perror("fopen destination");
        fclose(src);
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, src)) > 0)
    {
        if (fwrite(buffer, 1, bytes_read, dest) != bytes_read)
        {
            perror("fwrite");
            fclose(src);
            fclose(dest);
            return 1;
        }
    }

    if (ferror(src))
    {
        perror("fread");
        fclose(src);
        fclose(dest);
        return 1;
    }

    if (fseek(src, 0, SEEK_END) == 0)
    {
        file_size = ftell(src);

        if (file_size != -1)
        {
            printf("Source file size: %ld bytes\n", file_size);
        }
    }

    fclose(src);
    fclose(dest);

    printf("File copied successfully.\n");

    return 0;
}
