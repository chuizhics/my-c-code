#include <stdio.h>
#include <stdlib.h>

#include <glob.h>
#include <string.h>

#define PAT "/etc/a*.conf"

int errfunc_(const char *errpath, int errno)
{
    puts(errpath);
    fprintf(stderr,"ERROR MSG:%s\n",strerror(errno));
    return 0;
}

int main()
{
    int err, i;
    glob_t globres; 

    err = glob(PAT,0,errfunc_,&globres);

    if(err)
    {
        fprintf(stderr, "ERROR MSG:%s\n",strerror(err));
        exit(1);
    }

    for(i = 0; i < globres.gl_pathc;i++)
    {
        puts(globres.gl_pathv[i]);
    }

    globfree(&globres);

    exit(0);
}