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

typedef enum {Plus, Minus, Times} OpKind;
typedef enum {ConstK, ListK} ExpKind;
typedef struct streenode
{
  ExpKind kind;
  OpKind op;
  struct streenode *child, *sibling;
  int val;
} STreeNode;
typedef STreeNode *SyntaxTree;

/* function prototypes for recursive calls */
SyntaxTree lexp(void);
SyntaxTree lexpseq(void);
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
  else error();

  if (token == ' ')
  {
    token = getchar();
  }
}

main()
{
  int result;
  SyntaxTree resultTree;
  resultTree = constructTree();

  result = getResult(resultTree);

  if (token == '\n') /* check for end of line */
    printf("Result = %d\n",result);
  else error();
  return 0;
}

int getResult(SyntaxTree rt)
{
  int temp;

  if(rt->kind == ConstK)
    return rt-> val;

  else if(rt->kind == ListK)
  {
    temp = getResult(rt->child);
    switch(rt->op) {
      case Plus: 
        rt = rt->child;
        if(rt->sibling)
        {
          temp += getResult(rt->sibling);     
        }
        break;
      case Minus:
        rt = rt->child;
        if(rt->sibling)
        {
          temp -= getResult(rt->sibling);
        }
        break;
      case Times:
        rt = rt->child;
        if(rt->sibling)
        {
          temp *= getResult(rt->sibling);
        }
     }
  }
  else error();
  return temp;
}

SyntaxTree constructTree()
{
  SyntaxTree st = NULL;
  token = getchar();

  st = lexp();
  return st;
}

SyntaxTree lexp(void)
{
  SyntaxTree currNode = (SyntaxTree) malloc(sizeof(STreeNode));

  int temp;

  if (token == '(')
  {
    match('(');
    if((token == '+') || (token == '-') || (token == '*'))
    {
       currNode->kind = ListK;
       switch(token) {
         case('+'): currNode->op = Plus;
                    break;
         case('-'): currNode->op = Minus;
                    break;
         case('*'): currNode->op = Times;
                    break;
       }
       match(token);
       currNode->child = lexpseq();
    }
    else error();
    match(')');
  }
  else if(isdigit(token))
  {
    ungetc(token,stdin);
    scanf("%d",&temp);
    
    currNode = (SyntaxTree) malloc(sizeof(STreeNode));
    currNode->kind = ConstK;
    currNode->val = temp;
    
    token = getchar();
    if(token == ' ')
      token = getchar();
  }
  else error();
  
  return currNode;
}

SyntaxTree lexpseq(void)
{
  SyntaxTree currNode = (SyntaxTree) malloc(sizeof(STreeNode));

  currNode = lexp();
  
  if((token != ')' && token != '\n'))
  {
     currNode->sibling = lexpseq();
  }
  return currNode; 
}



