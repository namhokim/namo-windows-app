%{
/* ;으로 시작하는 것은 모두 주석으로 친다.즉,line comment다*/
#include <stdio.h>
#include "file_parser.h"

int GetfromURL(char *);
extern long fail_url;
long int total_url=0;
%}
URL http:\/\/[^\n\t ;]+
COMMENT ;[^\n]+[\n]
%%
{URL}	{   GetfromURL(yytext);
			total_url++;
		}
{COMMENT}	;	/*{ printf("COM=>%s\n", yytext);}*/
[\t ]+	/*공백은 무시*/	;
.	;
\n	;
%%
int yywrap(void)
{
	printf("## success URLs : %d / fail URLs : %d / total URLs : %d ##\n", total_url-fail_url, fail_url, total_url);
	
	return 1;
}
/*
void main(int argc, char *argv[])
{
	printf("argumnet counter: %d\n", argc);
	if(argc > 1) {               
		printf("filename: %s\n", argv[1]);
		yyin=fopen(argv[1], "r");
	}
	else return;
	
	yylex();
	
	return;
}
*/