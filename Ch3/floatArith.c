/* Simple integer arithmetic calculator according to the EBNF:

  EBNF FOR INT:
  <exp> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> * | / | %
  <factor> -> <exponent> { <power> <factor> }
  <power> -> ^
  <exponent> -> [ <minus> ] <final>
  <final> -> ( <exp> ) | Number

  EBNF FOR FLOAT:
  <expression> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> * | / 
  <factor> -> <exponent> { <power> <factor> }
  <power> -> ^
  <exponent> -> [ <minus> ] <final>
  <final> -> ( <expression> ) | <number>
  <number> -> <digit>{<digit>}.<digit>{<digit>}

  Inputs a line of text from stdin
  Outputs "Error" or the result
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

char token; /* global token variable */

/* function prototypes for recurisve calls */
float expression(void);
float term(void);
float factor(void);
float exponent(void);
float final(void);
float number(void);

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
  float result;
  token = getchar(); /* load token with first character for lookahead */

  result = expression();
  if (token == '\n') /* check for end of line */
    printf("Result = %f\n",result);
  else error();
  return 0;
}

float expression(void)
{
  float temp = term();
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

float term(void)
{
  float temp = factor();
  float mod;
  int tempInt;
  int modInt;
  while((token == '*') || (token=='/') || (token=='%'))
  {
    switch(token) {
      case '*': match('*');
                temp *= factor();
                break;
      case '/': match('/');
                temp /= factor();
                break;
    }
  }
  return temp;
}

float factor(void)
{
  float temp = exponent();
  while(token == '^')
  {
    match('^');
    
    temp = (float) pow((double) temp,(double) factor());
  }
  return temp;
}

float exponent(void)
{
  float temp;
  if(token == '-')
  {
    match('-');
    temp = 0 - final();
  }
  else temp = final();

  return temp;
}

float final(void)
{
  int digit;
  float decPlace = 0.1;
  float temp = 0;
  if (token == '(')
  {
    match('(');
    temp = expression();
    match(')');
  }
  else if(isdigit(token))
  {
    temp = number();
  }
  else error();
  return temp;
}

float number(void)
{
  int digit;
  float decPlace = 0.1;
  float temp = 0;

  while(isdigit(token))
  {
    ungetc(token,stdin);
    temp *= 10;
    scanf("%1d", &digit);
    temp += digit;
    token = getchar();
  }
  if(token == '.')
  {
    match('.');
    while(isdigit(token))
    {
      ungetc(token,stdin);
      scanf("%1d",&digit);
      temp += ((float)digit * decPlace);
      decPlace /= 10;
      token = getchar();
    }
  }
  return temp;
}
