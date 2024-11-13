

// getline 是方言,需要自己实现.

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    FILE *stream = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    if (argc < 2)
    {
        // fprintf(stderr,"Usage....\n");
        // exit(1);
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    stream = fopen(argv[1], "r");
    if (stream == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
        // exit(1);
    }

    // ssize_t getline(char **lineptr, size_t *n, FILE *stream);
    while ((nread = getline(&line, &len, stream)) > 0)
    {
        printf("len = %ld\n",len);
        printf("Retrieved line of length %zu:\n", nread);
        fwrite(line, nread, 1, stdout);
    }

    free(line);
    fclose(stream);

    // exit(0);
    exit(EXIT_SUCCESS);
}