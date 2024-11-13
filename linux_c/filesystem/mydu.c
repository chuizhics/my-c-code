#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <glob.h>


#define PATH_SIZE 1024

static int doloop(const char *path)
{
    //../filesystem/.   /aaa/bbb/cc/ddd/ss
    char *pos = NULL;
    pos = strrchr(path, '/');
    return strcmp(pos + 1, ".") != 0 && strcmp(pos + 1, "..") != 0;
}

static int64_t mydu(const char *path)
{
    static struct stat statres;
    glob_t globres;
    size_t i = 0;
    int64_t check = 0;
    int64_t sum = 0;
    char newpath_1[PATH_SIZE + 1];
    char newpath_2[PATH_SIZE + 1];
    if (lstat(path, &statres) != 0)
    {
        perror("lstat()");
        return -1;
    }
    if (!S_ISDIR(statres.st_mode))
    {
        return statres.st_blocks;
    }
    // printf("[%s:%d] path = %s\n", __FUNCTION__,__LINE__,path);

    strncpy(newpath_1, path, PATH_SIZE - 2);
    strcat(newpath_1, "/*");

    // printf("[%s:%d] newpath_1 = %s\n", __FUNCTION__,__LINE__,newpath_1);
    check = glob(newpath_1, 0, NULL, &globres);
    if (check != 0)
    {
        switch (check)
        {
        case GLOB_NOSPACE:
            fprintf(stderr, "newpath_1 = %s:glob() for running out of memory!\n", newpath_1);
            globfree(&globres);
            return -1;
        case GLOB_ABORTED:
            fprintf(stderr, "newpath_1 = %s:glob() for a read error!\n", newpath_1);
            globfree(&globres);
            return -1;
        case GLOB_NOMATCH:
            
            // 空目录或者当前目录下不存在隐藏文件.

            //fprintf(stderr, "newpath_1 = %s:glob() for no found matches.\n", newpath_1);
            break;
        default:
            fprintf(stderr, "newpath_1 = %s:glob() other error.\n", newpath_1);
            globfree(&globres);
            return -1;
        }
    }

    strncpy(newpath_2, path, PATH_SIZE - 3);
    strcat(newpath_2, "/.*");

    // printf("[%s:%d] newpath_2 = %s\n", __FUNCTION__,__LINE__,newpath_2);

    // return -1;
    check = glob(newpath_2, GLOB_APPEND, NULL, &globres);
    if (check != 0)
    {
        switch (check)
        {
        case GLOB_NOSPACE:
            fprintf(stderr, "newpath_2 = %s:glob() for running out of memory!\n",newpath_2);
            globfree(&globres);
            return -1;
        case GLOB_ABORTED:
            fprintf(stderr, "newpath_2 = %s:glob() for a read error!\n",newpath_2);
            globfree(&globres);
            return -1;
        case GLOB_NOMATCH:
            // 空目录或者当前目录下不存在隐藏文件.
            // fprintf(stderr, "newpath_2 = %s:glob() for no found matches.\n",newpath_2);
            break;
        default:
            fprintf(stderr, "newpath_2 = %s:glob() other error.\n",newpath_2);
            globfree(&globres);
            return -1;
        }
    }

    sum = statres.st_blocks;

    for (i = 0; i < globres.gl_pathc; i++)
    {
        // printf("[%s:%d] globres.gl_pathv[%ld] = %s\n", __FUNCTION__,__LINE__,i,globres.gl_pathv[i]);

        if (doloop(globres.gl_pathv[i]))
        {
            check = mydu(globres.gl_pathv[i]);
            if (check < 0)
            {
                globfree(&globres);
                return -1;
            }
            sum += check;
        }
    }

    globfree(&globres);

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
