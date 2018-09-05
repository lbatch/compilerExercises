/* Simple integer arithmetic calculator according to the EBNF:

  <exp> -> <term> { <addop> <term> }
  <addop> -> + | -
  <term> -> <factor> { <mulop> <factor> }
  <mulop> -> *
  <factor> -> ( <exp> ) | Number

  Inputs a line of text from stdin
  Outputs "Error" or the result

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char token; /* global token variable */

typedef enum {Plus, Minus,Times} OpKind;
typedef enum {OpK, ConstK} ExpKind;
typedef struct streenode
  {
    ExpKind kind;
    OpKind op;
    struct streenode *lchild, *rchild;
    int val;
  } STreeNode;
typedef STreeNode *SyntaxTree;

/* function prototypes for recursive calls */
SyntaxTree expression(void);
SyntaxTree term(void);
SyntaxTree factor(void);
int getResult(SyntaxTree);
SyntaxTree constructTree(void);

void error(void)
{
  fprintf(stderr,"Error\n");
  exit(1);
}

void match(char expectedToken)
{
  if (token == expectedToken)
    token = getchar();
  else
    error();
}

int main()
{
  int result;
  
  SyntaxTree resultTree;
  resultTree = constructTree(); 
 
  result = getResult(resultTree);
  if(token == '\n') /* check for end of line */
    printf("Result = %d\n", result);
  else
    error(); /* extraneous chars on line */
  
  return 0;
}

int getResult(SyntaxTree rt)
{
  if(rt->kind == ConstK)
    return rt->val;

  else if(rt->kind == OpK)
  {
    switch(rt->op) {
      case Plus: return getResult(rt->lchild) + getResult(rt->rchild);
                 break;
      case Minus: return getResult(rt->lchild) - getResult(rt->rchild);
                  break;
      case Times: return getResult(rt->lchild) * getResult(rt->rchild);
                  break;
    }
  }
}

SyntaxTree constructTree()
{
  SyntaxTree st = NULL;
  token = getchar();

  st = expression();
  return st;
}  

SyntaxTree expression(void)
{
  SyntaxTree tempNode;
  SyntaxTree currNode;

  tempNode = term();
  currNode = tempNode;
 
  while ((token == '+')||(token == '-'))
  {
    currNode = (SyntaxTree) malloc(sizeof(STreeNode));
    currNode->kind = OpK;
    currNode->lchild = tempNode;
    switch(token) {
      case '+': match('+');
                currNode->op = Plus;
                break;
      case '-': match('-');
                currNode->op = Minus;
                break;
   }
   currNode->rchild = term();
   tempNode = currNode;  
  }
  return currNode;
}

SyntaxTree term(void)
{
  SyntaxTree tempNode;
  SyntaxTree currNode;

  tempNode = factor();
  currNode = tempNode;

  while (token == '*')
  {
    currNode = (SyntaxTree) malloc(sizeof(STreeNode));
    currNode->kind = OpK;
    currNode->op = Times;
    match('*');
    currNode->lchild = tempNode;
    currNode->rchild = factor();
    tempNode = currNode;
  }
  return currNode;
}

SyntaxTree factor(void)
{
  SyntaxTree currNode;
  int temp;
  if(token == '(')
  {
    match('(');
    currNode = expression();
    match(')');
  }
  else if (isdigit(token))
  {
    currNode = (SyntaxTree) malloc(sizeof(STreeNode));
    currNode->kind = ConstK;

    ungetc(token, stdin);
    scanf("%d",&temp);
    currNode->val = temp;

    token = getchar();
  }
  else error();
  return currNode;
}







