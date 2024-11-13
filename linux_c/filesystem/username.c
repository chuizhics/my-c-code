#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <string.h>
#include <ctype.h>

static int isuint32str(const char *str, u_int32_t *res)
{
    int len = strlen(str);

    int i = 0;
    u_int64_t midresult = 0;
    for (i = 0; i < len; i++)
    {
        if (!(isdigit(str[i])))
            return 0;
        else
        {
            midresult = (midresult)*10 + (str[i] - '0');
            if(midresult > __UINT_LEAST32_MAX__)
            {
                return 0;
            }
        }
    }
    *res = (u_int32_t)midresult;
    return 1;
}

int main(int argc,char *argv[])
{   
    struct passwd *pwd = NULL;
    uid_t uid;

    if(argc != 2)
    {
        fprintf(stderr,"Usage:%s <uid>\n", argv[0]);
        exit(1);
    }
    
    if(!isuint32str(argv[1],&uid))
    {
        fprintf(stderr,"uid input error 1!\n");
        exit(1);
    }

    pwd = getpwuid(uid);
    if(pwd == NULL)
    {
        fprintf(stderr,"uid input error 2!\n");
        exit(1);
    }

    printf("uid:%u\tname=%s\tpasswd=%s\n",uid,pwd->pw_name,pwd->pw_passwd);


    exit(0);
}