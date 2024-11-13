#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>

#define DIRNAME "/etc"

int main()
{
    DIR *dp = NULL;

    struct dirent *readres = NULL;

    dp = opendir(DIRNAME);
    if(dp == NULL)
    {
        perror("opendir()");
        exit(1);
    }


    while((readres = readdir(dp)) != NULL)
    {
        printf("ino=%12lu\tname=%18s\toff=%24ld\treclen=%4hx\ttpye=%c\t\n",
        readres->d_ino,readres->d_name,readres->d_off,readres->d_reclen,readres->d_type);
    }

    closedir(dp);

    exit(0);
}