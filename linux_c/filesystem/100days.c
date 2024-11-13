#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#define TIMESTRSIZE 1024

int main()
{
    time_t stamp = 0;
    struct tm *tmp = NULL;
    char timestr[TIMESTRSIZE]={};
    stamp = time(NULL);
    tmp = localtime(&stamp);

    // %F == %Y-%m-%d
    strftime(timestr,TIMESTRSIZE,"Now:%F",tmp);
    puts(timestr);

    tmp->tm_mday += 100;
    (void)mktime(tmp);

    strftime(timestr,TIMESTRSIZE,"100 days later:%F",tmp);
    puts(timestr);

    exit(0);
}