#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include <time.h>
#include <string.h>


#define TIMESTRSIZE 1024

#define FMTSTRSIZE 1024



/**
 * -y : year(00-99)
 * -m : month 
 * -d : day
 * -H : hour
 * -M : minute
 * -S : second
 * -Y : year(4)
**/
//extern char *optarg;

int main(int argc, char *argv[])
{
    time_t stamp = 0;
    struct tm *tmp = NULL;
    char timestr[TIMESTRSIZE]={};
    char fmtstr[FMTSTRSIZE]={};
    int c;
    FILE *fp = stdout;

    stamp = time(NULL);
    tmp = localtime(&stamp);


    /**
     * ./mydate -H 24 -M -S
    */
    while((c = getopt(argc,argv,"-y:mdH:MS")) != -1)
    {
        printf("c=%c optind=%d optarg=%s\n",c,optind,optarg);
        switch (c)
        {
        case 1:
            if(fp == stdout)
            {
                fp = fopen(argv[optind - 1],"w");
                if(fp == NULL)
                {
                    perror("fopen()");
                    fp = stdout;
                }
            }
            break;
        case 'y':
            if(strcmp(optarg,"2") == 0)
            {
                strcat(fmtstr,"%y ");
            }
            else if(strcmp(optarg,"4") == 0)
            {
                strcat(fmtstr,"%Y ");
            }
                else
                {
                    fprintf(stderr,"Invalid argument of y---%s\n",optarg);
                }
            break;
        case 'm':
            strcat(fmtstr,"%m ");
            break;
        case 'd':
            strcat(fmtstr,"%d ");
            break;
        case 'H':
            //printf("optarg = %s\n",optarg);
            if(strcmp(optarg,"12") == 0)
                strcat(fmtstr,"%I(%P) ");
            else if(strcmp(optarg,"24") == 0)
                strcat(fmtstr,"%H ");
                else
                    fprintf(stderr,"Invalid argument of H\n");
            break;
        case 'M':
            strcat(fmtstr,"%M ");
            break;
        case 'S':
            strcat(fmtstr,"%S ");
            break;
        default:

            break;
        }

    }
    printf("fmtstr = %s\n",fmtstr);
    strcat(fmtstr,"\n");
    strftime(timestr,TIMESTRSIZE,fmtstr,tmp);
    
    fputs(timestr,fp);

    if(fp != stdout)
        fclose(fp);

    exit(0);
}