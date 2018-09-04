/* Simple integer arithmetic calculator according to the EBNF:

  ORIGINAL EBNF:
  <exp> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> *
  <factor> -> ( <exp> ) | Number

  EBNF WITH DIVISION AND MODULO:
  <exp> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> * | / | %
  <factor> -> ( <exp> ) | Number

  EBNF WITH EXPONENTIATION:
  <exp> -> <term> { <addop> <term>}
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> * | / | %
  <factor> -> <exponent> { <power> <factor> }
  <power> -> ^
  <exponent> -> ( <exp> ) | Number

  Inputs a line of text from stdin
  Outputs "Error" or the result
*/


#include <stdio.h>
#include <stdlib.h>

char token; /* global token variable */

/* function prototypes for recurisve calls */
int exp(void);
int term(void);
int factor(void);

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

  result = exp();
  if (token == '\n') /* check for end of line */
    printf("Result = %d\n",result);
  else error();
  return 0;
}

int exp(void)
{
  int temp = term();
  while ((token == '+') || (token=='-'))
  {
    switch(token) {
      case '+': match('+');
                temp += term();
                break;
     case '-': match('-');
               temp -= term();
               break;
    }
  }
  return temp;
}

int term(void)
{
  int temp = factor();
  while((token == '*') || (token=='/') || (token=='%'))
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
  }
  return temp;
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
  if (token == '(')
  {
    match('(');
    temp = exp();
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

