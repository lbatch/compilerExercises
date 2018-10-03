/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the TINY compiler */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include <stdio.h>
#include <stdlib.h>

/* states in scanner DFA */
typedef enum
   { START,INASSIGN,INCOMMENT,INLESS,INGREATER,INNUM,INID,DONE }
   StateType;

/* lexeme of identifier or reserved word */
char* tokenString;
int curTokenSize = 10;

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[256]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void)
{ if (!(linepos < bufsize))
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    { if (EchoSource) fprintf(listing,"%4d: %s",lineno,lineBuf);
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"and",AND},{"else",ELSE},{"end",END},{"if",IF},{"not",NOT},
      {"or",OR},{"read",READ},{"repeat",REPEAT},{"then",THEN},
      {"until",UNTIL},{"write",WRITE}};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup (char * s)
{ 
  int l = 0;
  int r = MAXRESERVED - 1;

  while(l <= r)
  {
    int m = l + (r-l)/2;
   
    if(!strcmp(reservedWords[m].str,s))
    {
      return reservedWords[m].tok;
    }

    if(strcmp(reservedWords[m].str,s) < 0)
      l = m + 1;

    else
      r = m - 1;   
  }
  return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the 
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   tokenString = calloc(curTokenSize, sizeof(char));
   /* holds current token to be returned */
   TokenType currentToken;
   /* current state - always begins at START */
   StateType state = START;
   int commentLevel = 0;
   /* flag to indicate save to tokenString */
   int save;
   while (state != DONE)
   { 
     int c = getNextChar();
     save = TRUE;
     switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM;
         else if (isalpha(c))
           state = INID;
         else if (c == ':')
           state = INASSIGN;
         else if ((c == ' ') || (c == '\t') || (c == '\n'))
           save = FALSE;
         else if (c == '{')
         { 
           save = FALSE;
           state = INCOMMENT;
           commentLevel++;
         }
         else
         { state = DONE;
           switch (c)
           { case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case '=':
               currentToken = EQ;
               break;
             case '<':
               state = INLESS;
               break;
             case '>':
               state = INGREATER;
               break;
             case '+':
               currentToken = PLUS;
               break;
             case '-':
               currentToken = MINUS;
               break;
             case '*':
               currentToken = TIMES;
               break;
             case '/':
               currentToken = OVER;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ';':
               currentToken = SEMI;
               break;
             default:
               currentToken = ERROR;
               break;
           }
         }
         break;
       case INLESS:
         state = DONE;
         switch (c)
         {
           case EOF:
             save = FALSE;
             currentToken = ENDFILE;
             break;
           case '>':
             currentToken = NE;
             break;
           case '=':
             currentToken = LE;
             break;
           default:
             ungetNextChar();
             save = FALSE;
             currentToken = LT;
             break;
         }
         break;
      case INGREATER:
        state = DONE;
        switch (c)
        {
          case EOF:
            save = FALSE;
            currentToken = ENDFILE;
            break;
          case '=':
            currentToken = GE;
            break;
          default:
            ungetNextChar();
            save = FALSE;
            currentToken = GT;
            break;
        }
        break; 
       case INCOMMENT:
         save = FALSE;
         if (c == EOF)
         { state = DONE;
           currentToken = ENDFILE;
         }
         else if (c == '{')
         {
           commentLevel++;
         }
         else if (c == '}')
         {
           commentLevel--;
           if(commentLevel == 0)
             state = START;
         }
         break;
       case INASSIGN:
         state = DONE;
         if (c == '=')
           currentToken = ASSIGN;
         else
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           currentToken = ERROR;
         }
         break;
       case INNUM:
         if (!isdigit(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
       case INID:
         if (!isalpha(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: /* should never happen */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if(tokenStringIndex == curTokenSize)
     {
       curTokenSize *= 2;
       tokenString = (char *) realloc(tokenString, curTokenSize);
     }
     if (save)
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

