#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
	FILE *fp;
	
	fp = fopen("tmp", "r");

	if(fp == NULL)
	{
		fprintf(stderr, "fopen():%s\n", strerror(errno));
		exit(1);
	}

	puts("OK!");
	exit(0);

}
