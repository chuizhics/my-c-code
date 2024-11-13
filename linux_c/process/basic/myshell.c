#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/wait.h>

#include <string.h>
#include <glob.h>

#define MAX_COMMAND_LENGTH 1024

#define DELIMS " \t\n"

struct cmd_st
{
    glob_t globres;
};


static void prompt(void)
{
    printf("myshell-0.1$ ");
}

static void parse(char *command, char *argv[])
{
//    char *argv[MAX_COMMAND_LENGTH];

    int i = 0;
   // see also strsep
    char *token = strtok(command, DELIMS);

    while (token != NULL)
    {
        argv[i++] = token;

        token = strtok(NULL, DELIMS);
    }

    argv[i] = NULL;
}

static void parse_2(char *command, struct cmd_st *res)
{
    int i = 0;
    char *token = strtok(command, DELIMS);

    while (token != NULL)
    {
        glob(token,GLOB_NOCHECK|(GLOB_APPEND * i),NULL,&res->globres);
        i = 1;
        token = strtok(NULL, DELIMS);
    }
   
}

int main()
{
    char command[MAX_COMMAND_LENGTH] = {};
    pid_t pid = 0;
    char* argv[MAX_COMMAND_LENGTH];
    struct cmd_st res;
    while (1)
    {

        prompt(); // 打印提示符

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            fprintf(stdout, "[%s:%d]fgets() error!", __FUNCTION__, __LINE__);
            exit(1);
        }

        // Remove newline character from end of command
        // strcspn() 函数计算 s 的初始段的长度，该段完全由不在拒绝("\n")中的字节组成。
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        //parse(command, argv);
        parse_2(command,&res);

        fflush(NULL);
        if(0)       //是内部命令
        {

        }
        else        //是外部命令
        {
            pid = fork();
            if (pid < 0)
            {
                perror("fork()");
                exit(1);
            }
            else if (pid == 0)   // child
            {
                //execvp(argv[0],argv);
                execvp(res.globres.gl_pathv[0],res.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            }
            else
            {                   // parent
                wait(NULL);
            }
        }
        
    }

    exit(0);
}
