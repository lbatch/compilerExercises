/* Simple integer arithmetic calculator according to the EBNF:

  ORIGINAL EBNF:
  <exp> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> *
  <factor> -> ( <exp> ) | Number

  GRAMMAR WITHOUT LEFT RECURSION:
  <exp> -> <term> <exp2>
  <exp2> -> <addop> <term> <exp2> | nil
  <addop> -> + | -
  <term> -> <factor> <term2>
  <term2> -> <mulop> <factor> <term2> | nil
  <mulop> -> *
  <factor> -> ( <exp> ) | Number

  Inputs a line of text from stdin
  Outputs "Error" or the result
*/


#include <stdio.h>
#include <stdlib.h>

char token; /* global token variable */

/* function prototypes for recurisve calls */
int expression(void);
int expression2(int);
int term(void);
int term2(int);
int factor(void);
int exponent(void);
int final(void);

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
}

main()
{
  int result;
  token = getchar(); /* load token with first character for lookahead */

  result = expression();
  if (token == '\n') /* check for end of line */
    printf("Result = %d\n",result);
  else error();
  return 0;
}

int expression(void)
{
  int temp = term();
  return expression2(temp);
}

int expression2(int temp)
{
  if ((token == '+') || (token=='-'))
  {
    switch(token) {
      case '+': match('+');
                temp += term();
                break;
     case '-': match('-');
               temp -= term();
               break;
    }
    return expression2(temp);
  }
  else return temp;
}

int term(void)
{
  int temp = factor();
  return term2(temp);
}

int term2(int temp)
{
  if((token == '*') || (token=='/') || (token=='%'))
  {
    switch(token) {
      case '*': match('*');
                temp *= factor();
                break;
      case '/': match('/');
                temp /= factor();
                break;
      case '%': match('%');
                temp %= factor();
                break;
    }
    return term2(temp);
  }
  else return temp;
}

int factor(void)
{
  int temp = exponent();
  int base;
  int pow;
  while(token == '^')
  {
    match('^');
    base = temp;
    pow = exponent();
    for(int i = 1; i < pow; i++)
    {
      temp *= base;
    }
  }
  return temp;
}

int exponent(void)
{
  int temp;
  if(token == '-')
  {
    match('-');
    temp = 0 - final();
  }
  else temp = final();

  return temp;
}

int final(void)
{
  int temp;
  if (token == '(')
  {
    match('(');
    temp = expression();
    match(')');
  }
  else if (isdigit(token))
  {
    ungetc(token,stdin);
    scanf("%d",&temp);
    token = getchar();
  }
  else error();
  return temp;
}


