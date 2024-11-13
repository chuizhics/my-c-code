#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define MEMSZIE 1024

int mystrcpy(char *dest, size_t capacity, const char *src)
{
    size_t src_len = 0;

    if (dest == NULL || src == NULL)
    {
        return -1;
    }
    src_len = strlen(src);
    if (src_len + 1 > capacity)
    {
        fprintf(stderr, "capacity is limit!\n");
        return -2;
    }
    else
    {
        strcpy(dest, src);
    }
    return 0;
}

int main()
{
    char *ptr;
    pid_t pid;

    ptr = mmap(NULL, MEMSZIE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        munmap(ptr, MEMSZIE);
        exit(1);
    }

    if (pid == 0) // child write
    {
        // strcpy(ptr,"Hello!");
        mystrcpy(ptr, MEMSZIE, "Hello!");
        // 释放子进程中的"mmap"
        munmap(ptr, MEMSZIE);
        exit(0);
    }
    else
    { // parent read
        wait(NULL);
        puts(ptr);
        munmap(ptr, MEMSZIE);
        exit(0);
    }
}