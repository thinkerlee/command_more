#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#define PAGELEN 24
#define LINELEN 512
#define TTY_PATH "/dev/tty"

void do_more(FILE *);
int see_more(FILE *);

int main( int argc, char ** argv)
{
	FILE *fp;
	if( argc == 1 )
	{
		do_more(stdin);	
	}
	else
	{
		while( --argc )
		{
			if((fp = fopen(* ++argv, "r" )) != NULL )	
			{
				do_more(fp);
				fclose(fp);
			}
			else
			{
				exit(-1);
			}
		}
	}
}

void do_more(FILE *fp)
{
	char line[LINELEN];
	int num_lines = 0;
	int see_more(FILE *), reply;
	FILE *fp_tty;
	fp_tty = fopen(TTY_PATH, "r" );
	if( fp_tty == NULL)
	{
		perror("open tty wrong");
		exit(0);
	}
	while( fgets( line, LINELEN, fp) )
	{
		if( num_lines == PAGELEN)
		{
			reply = see_more(fp_tty);
			if( reply == 0 )
			{
				break;	
			}
			num_lines -= reply;
		}
		if( fputs( line, stdout) == EOF )
		{
			exit(1);	
		}
		num_lines++;
	}
}

int see_more(FILE* cmd)
{
		int c;
		printf("\033[7m more? \033[m"); /*reverse on at vt100*/
		while((c = getc(cmd))!= EOF)
		{
			if( c == 'q' )
				return 0;
			if( c == ' ' )
				return PAGELEN;
			if( c == '\n')
				return 1;
		}
		return 0;
}
