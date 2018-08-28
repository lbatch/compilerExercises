%{
#include <stdio.h>
#include <ctype.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
%}

%%
"/*" { char c;
       int done = FALSE;
       fprintf(yyout,"%s", yytext);
       do
       { while ((c = input()) != '*')
           putc(toupper(c), yyout);
         putchar(c);
         while((c = input()) == '*')
           putc(c, yyout);
         if(c == '/') 
         {
           putc(c,yyout);
           done = TRUE;
         }
       } while (!done);
     }
.    { putc(yytext[0], yyout); }
%%

void main(int argc, char **argv)
{
 if(argc > 1)
 {
   yyin = fopen(argv[1], "r");
 }
 else
 {
   yyin = stdin;
 }
 yylex();
}
