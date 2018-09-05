/* Simple integer arithmetic calculator in LISP-like prefix form:

  BNF: 
  <lexp> -> Number | ( <op> <lexp-seq> )
  <op> -> + | - | *
  <lexp-seq> -> <lexp-seq> <lexp> | <lexp>

  Inputs a line of text from stdin
  Outputs "Error" or the result
*/


#include <stdio.h>
#include <stdlib.h>

char token; /* global token variable */

/* function prototypes for recurisve calls */
int lexp(void);
int lexpseq(char);

void error(void)
{
  fprintf(stderr,"Error\n");
  exit(1);
}

void match(char expectedToken)
{
  if (token == expectedToken)
    token = getchar();
  else error();

  if (token == ' ')
  {
    token = getchar();
  }
}

main()
{
  int result;
  token = getchar(); /* load token with first character for lookahead */

  result = lexp();
  if (token == '\n') /* check for end of line */
    printf("Result = %d\n",result);
  else error();
  return 0;
}

int lexp(void)
{
  int temp;

  if (token == '(')
  {
    match('(');
    if((token == '+') || (token == '-') || (token == '*'))
    {
       temp = lexpseq(token);
    }
    else error();
    match(')');
  }
  else if(isdigit(token))
  {
    ungetc(token,stdin);
    scanf("%d",&temp);
    token = getchar();
    if(token == ' ')
      token = getchar();
  }
  else error();
  
  return temp;
}

int lexpseq(char op)
{
  if(token == op)
    match(token);
  int temp = lexp();
  if((token != ')' && token != '\n'))
  {
     switch(op) {
       case '+': temp += lexpseq(op);
                 break;
       case '-': temp -= lexpseq(op);
                 break;
       case '*': temp *= lexpseq(op);
                 break;
     }
  }
  return temp; 
}



