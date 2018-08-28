%{
/* Lex program to convert uppercase to
   lowercase except inside comments;
   adding nested comments
*/
#include <stdio.h>
#include <ctype.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

int nesting = 0;
%}


%%
[A-Z]  {
         putchar(tolower(yytext[0]));
       }
"/*"   {
         nesting++;
         char c;
         int done = FALSE;
         ECHO;
         do {
           while ((c=input()) != '*')
             putchar(c);
           putchar(c);
           while ((c=input()) == '*')
             putchar(c);
           putchar(c);
           if (c == '/')
           {
             nesting--;
             if(nesting == 0)
               done = TRUE;
           }
         } while(!done);
       }
%%

void main(void)
{ yylex(); }
