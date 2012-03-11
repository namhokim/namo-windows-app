#include <lex.h>

#define YYLEXFAST
#line 1 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"

/* ;으로 시작하는 것은 모두 주석으로 친다.즉,line comment다*/
#include <stdio.h>
#include "file_parser.h"

int GetfromURL(char *);
extern long fail_url;
long int total_url=0;

#line 15 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
/* repeated because of possible precompiled header */
#include <lex.h>

#define YYLEXFAST
#include "file_parser.h"

#ifndef YYTEXT_SIZE
#define YYTEXT_SIZE 100
#endif
#ifndef YYUNPUT_SIZE
#define YYUNPUT_SIZE YYTEXT_SIZE
#endif

/* yytext */
static char YYNEAR yysatext[(YYTEXT_SIZE) + 1];		/* extra char for \0 */
char YYFAR *YYNEAR YYDCDECL yystext = yysatext;
char YYFAR *YYNEAR YYDCDECL yytext = yysatext;
int YYNEAR YYDCDECL yystext_size = (YYTEXT_SIZE);
int YYNEAR YYDCDECL yytext_size = (YYTEXT_SIZE);

/* yystatebuf */
#if (YYTEXT_SIZE) != 0
static int YYNEAR yysastatebuf[(YYTEXT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysstatebuf = yysastatebuf;
int YYFAR *YYNEAR YYDCDECL yystatebuf = yysastatebuf;
#else
int YYFAR *YYNEAR YYDCDECL yysstatebuf = NULL;
int YYFAR *YYNEAR YYDCDECL yystatebuf = NULL;
#endif

/* yyunputbuf */
#if (YYUNPUT_SIZE) != 0
static int YYNEAR yysaunputbuf[(YYUNPUT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = yysaunputbuf;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = yysaunputbuf;
#else
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = NULL;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = NULL;
#endif
int YYNEAR YYDCDECL yysunput_size = (YYUNPUT_SIZE);
int YYNEAR YYDCDECL yyunput_size = (YYUNPUT_SIZE);

/* backwards compatability with lex */
#ifdef input
#ifdef YYPROTOTYPE
int YYCDECL yyinput(void)
#else
int YYCDECL yyinput()
#endif
{
	return input();
}
#else
#define input yyinput
#endif

#ifdef output
#ifdef YYPROTOTYPE
void YYCDECL yyoutput(int ch)
#else
void YYCDECL yyoutput(ch)
int ch;
#endif
{
	output(ch);
}
#else
#define output yyoutput
#endif

#ifdef unput
#ifdef YYPROTOTYPE
void YYCDECL yyunput(int ch)
#else
void YYCDECL yyunput(ch)
int ch;
#endif
{
	unput(ch);
}
#else
#define unput yyunput
#endif

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn -rch		/* <warning: unreachable code> off */
#endif
#endif

#ifdef YYPROTOTYPE
int YYCDECL yylexaction(int action)
#else
int YYCDECL yylexaction(action)
int action;
#endif
{
	yyreturnflg = 1;
	switch (action) {
	case 1:
		{
#line 13 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"
   GetfromURL(yytext);
			total_url++;
		
#line 121 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
		}
		break;
	case 2:
		{
#line 16 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"
;
#line 128 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
		}
		break;
	case 3:
		{
#line 17 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"
/*공백은 무시*/	;
#line 135 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
		}
		break;
	case 4:
		{
#line 18 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"
;
#line 142 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
		}
		break;
	case 5:
		{
#line 19 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"
;
#line 149 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
		}
		break;
	default:
		yyassert(0);
		break;
	}
	yyreturnflg = 0;
	return 0;
}

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn .rch		/* <warning: unreachable code> to the old state */
#endif
#endif
#line 20 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.l"

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
#line 188 "D:\\data\\term4-1\\proj\\HongDa_close_icon\\file_parser.cpp"
YYCONST yymatch_t YYNEARFAR YYBASED_CODE YYDCDECL yymatch[] = {
	0
};

int YYNEAR YYDCDECL yytransitionmax = 119;
YYCONST yytransition_t YYNEARFAR YYBASED_CODE YYDCDECL yytransition[] = {
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 5, 1 },
	{ 7, 1 },
	{ 0, 15 },
	{ 0, 15 },
	{ 5, 5 },
	{ 0, 4 },
	{ 11, 9 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 5, 1 },
	{ 0, 0 },
	{ 0, 15 },
	{ 0, 0 },
	{ 5, 5 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 14, 13 },
	{ 15, 14 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 13, 12 },
	{ 4, 1 },
	{ 0, 0 },
	{ 0, 15 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 3, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 12, 10 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 8, 3 },
	{ 10, 8 }
};

YYCONST yystate_t YYNEARFAR YYBASED_CODE YYDCDECL yystate[] = {
	{ 0, 0, 0 },
	{ -6, 1, 0 },
	{ 1, 0, 0 },
	{ 0, 1, 4 },
	{ -9, 5, 4 },
	{ 0, 5, 3 },
	{ 0, 0, 4 },
	{ 0, 0, 5 },
	{ 0, 2, 0 },
	{ -9, 6, 0 },
	{ 0, 1, 0 },
	{ 0, 0, 2 },
	{ 0, 1, 0 },
	{ 0, 1, 0 },
	{ 0, 2, 0 },
	{ -16, 3, 0 },
	{ 15, 0, 1 }
};

YYCONST yybackup_t YYNEARFAR YYBASED_CODE YYDCDECL yybackup[] = {
	0,
	0,
	0,
	0,
	0,
	0
};

