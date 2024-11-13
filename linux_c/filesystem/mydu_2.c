#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <dirent.h>

#define PATH_SIZE 1024

static int64_t mydu(const char *path)
{
    DIR *dp = NULL;
    int64_t sum = 0;
    size_t pathlen = 0;
    size_t d_namelen = 0;
    static struct stat statres;
    struct dirent *readres = NULL;
    char newpath[PATH_SIZE + 1] = {};

    if (lstat(path, &statres) < 0)
    {
        //printf("[%s:%d] path = %s\n", __FUNCTION__,__LINE__,path);
        //perror("lstat()");
        fprintf(stderr,"lstat():%s path=%s\n",strerror(errno),path);
        return 0;
    }
    if (!S_ISDIR(statres.st_mode))
    {
        return statres.st_blocks;
    }
    pathlen = strlen(path);
    if (pathlen > PATH_SIZE)
    {
        fprintf(stderr, "Path too long!: %s\n", path);
        return 0;
    }
    dp = opendir(path);
    if (dp == NULL)
    {
        //perror("");
        fprintf(stderr,"opendir():%s path=%s\n",strerror(errno),path);
        return 0;
    }
    sum = statres.st_blocks;
    strcpy(newpath, path);
    while ((readres = readdir(dp)) != NULL)
    {
        if (strcmp(readres->d_name, ".") != 0 && strcmp(readres->d_name, "..") != 0)
        {
            d_namelen = strlen(readres->d_name);
            if (pathlen + d_namelen + 1> PATH_SIZE)
            {
                fprintf(stderr, "Path too long!: %s/%s\n", path,readres->d_name);
                closedir(dp);
                return 0;
            }
            strcat(newpath,"/");
            // strcat 遇到'\0'才开始拼接
            // F strcat(newpath + pathlen + 1, readres->d_name);
            strcpy(newpath+pathlen+1, readres->d_name);
            //printf("[%s:%d] newpath = %s\n", __FUNCTION__,__LINE__,newpath);
            sum += mydu(newpath);
        }
    }
    closedir(dp);
    return sum;
}

int main(int argc, char *argv[])
{
    int64_t blocks = 0;
    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s <path>\n", argv[0]);
        exit(1);
    }

    blocks = mydu(argv[1]);

    if (blocks < 0)
    {
        fprintf(stderr, "mydu: error\n");
        exit(1);
    }

    printf("%ld\t%s\n", blocks / 2, argv[1]);

    exit(0);
}
