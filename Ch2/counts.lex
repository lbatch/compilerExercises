%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
int lineCount = 0;
int wordCount = 0;
int charCount = 0;
%}

newline \n
whitespace [ \t\r]
letter [A-Za-z]
digit [0-9]
alphanum {letter}|{digit}
word {alphanum}+

%%
{newline} { 
            charCount++;
            lineCount++; 
          }
{word}  { charCount += strlen(yytext);
          wordCount++;
        }
. { charCount++; }
%%

void main(int argc, char **argv)
{
 if(argc > 1)
 {
   yyin = fopen(argv[1], "r");
 }
 else
 {
   printf("Please provide a file in the second argument.\n");
   exit(1);
 }
 yylex();

 printf("Line count: %d\n", lineCount);
 printf("Word count: %d\n", wordCount);
 printf("Character count: %d\n", charCount);

}
