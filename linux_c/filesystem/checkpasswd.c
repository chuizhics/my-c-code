#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>

/**
 * 
 * gcc checkpasswd.c -o checkpasswd -lcrypt
 * 
 * su root
*/

int main(int argc, char *argv[])
{
    char *crypted_passwd = NULL;
    char *input_passwd = NULL;
    struct spwd *shadownline = NULL;
    if(argc != 2)
    {
        fprintf(stderr,"Usage:%s <username>\n",argv[0]);
        exit(1);
    }

    input_passwd =  getpass("Password:");
    shadownline = getspnam(argv[1]);
    if(shadownline == NULL)
    {
        fprintf(stderr,"username does not exist OR permission denied\n");
        exit(1);
    }

    crypted_passwd = crypt(input_passwd,shadownline->sp_pwdp);

    if(strcmp(crypted_passwd,shadownline->sp_pwdp) == 0)
    {
        puts("OK!");
    }
    else
    {
        puts("Failure!");
    }

    exit(0);
}