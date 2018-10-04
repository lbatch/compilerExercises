/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * compare_exp(void);
static TreeNode * bool_term(void);
static TreeNode * bool_factor(void);
static TreeNode * bool_val(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected, char * message)
{ if (token == expected) token = getToken();
  else {
    fprintf(listing, "Expected token: ");
    printToken(expected,"");
    fprintf(listing, "Received token: ");
    printToken(token,tokenString);
    syntaxError(message);
  }
}

TreeNode * stmt_sequence(void)
{ TreeNode * t = statement();
  TreeNode * p = t;
  while ((token!=ENDFILE) && (token!=END) &&
         (token!=ELSE) && (token!=UNTIL))
  { TreeNode * q;
    match(SEMI, "Missing semicolon");
    q = statement();
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

TreeNode * statement(void)
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;
    case REPEAT : t = repeat_stmt(); break;
    case ID : t = assign_stmt(); break;
    case READ : t = read_stmt(); break;
    case WRITE : t = write_stmt(); break;
    default : syntaxError("Unexpected token -> ");
              printToken(token,tokenString);
              token = getToken();
              break;
  } /* end case */
  return t;
}

TreeNode * if_stmt(void)
{ TreeNode * t = newStmtNode(IfK);
  match(IF, "Invalid if clause"); /* Should not happen unless if_stmt incorrectly called */
  if (t!=NULL) t->child[0] = compare_exp();
  match(THEN, "Missing 'then' statements in 'if' structure");
  if (t!=NULL) t->child[1] = stmt_sequence();
  if (token==ELSE) {
    match(ELSE, "Missing 'else'"); /* Should not happen because of prior test */
    if (t!=NULL) t->child[2] = stmt_sequence();
  }
  match(END, "Missing end of if statement");
  return t;
}

TreeNode * repeat_stmt(void)
{ TreeNode * t = newStmtNode(RepeatK);
  match(REPEAT, "Invalid repeat clause"); /* should not happen unless repeat_stmt incorrectly called */
  if (t!=NULL) t->child[0] = stmt_sequence();
  match(UNTIL, "Until loop missing terminating condition"); 
  if (t!=NULL) t->child[1] = compare_exp();
  return t;
}

TreeNode * assign_stmt(void)
{ TreeNode * t = newStmtNode(AssignK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID, "Assign statement must begin with identifier");
  match(ASSIGN, "Missing assign token");
  if (t!=NULL) t->child[0] = simple_exp();
  return t;
}

TreeNode * read_stmt(void)
{ TreeNode * t = newStmtNode(ReadK);
  match(READ, "Invalid read clause"); /* should not happen unless read_stmt incorrectly called */
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID, "Read requires a destination identifier");
  return t;
}

TreeNode * write_stmt(void)
{ TreeNode * t = newStmtNode(WriteK);
  match(WRITE, "Invalid write clause"); /* should not happen unless write_stmt incorrectly called */
  if (t!=NULL) t->child[0] = simple_exp();
  return t;
}

TreeNode * compare_exp(void)
{ TreeNode * t = bool_term();
  while(token == OR)
  {
    TreeNode * p = newExpNode(OpK);
    if(p != NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token, ""); /* matching self, should not throw error */
      t->child[1] = bool_term();
    }
  }
  return t;
}

TreeNode * bool_term(void)
{
  TreeNode * t = bool_factor();
  while(token == AND)
  {
    TreeNode * p = newExpNode(OpK);
    if(p != NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token, ""); /* matching self, should not throw error */
      t->child[1] = bool_factor();
    }
  }
  return t;
}

TreeNode * bool_factor(void)
{
  TreeNode * p = newExpNode(OpK);
  if(token == NOT)
  {
    p->attr.op = token;
    match(token, ""); /* matching self, should not throw error */
    p->child[0] = bool_val();
  }
  else
  {
    p = bool_val();
  }
  return p;
}

TreeNode * bool_val(void)
{ TreeNode * t = NULL;
  switch (token) {
    case LPAREN :
      match(LPAREN, "Invalid parenthetical");
      t = compare_exp();
      match(RPAREN, "Missing closing parenthesis");
      break;
    default:
      t = simple_exp();
      TreeNode *p = newExpNode(OpK);
      if(p != NULL) {
        p->child[0] = t;
        p->attr.op = token;
        t = p;
      }
      match(token, ""); /* matching self, should not throw error */
      if(t != NULL)
        t->child[1] = simple_exp();
      break;
    }
  return t;
}



TreeNode * simple_exp(void)
{ TreeNode * t = term();
  while ((token==PLUS)||(token==MINUS))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token, ""); /* matching self, should not throw error */
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode * term(void)
{ TreeNode * t = factor();
  while ((token==TIMES)||(token==OVER))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token, ""); /* matching self, should not throw error */
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode * factor(void)
{ TreeNode * t = NULL;
  switch (token) {
    case NUM :
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atoi(tokenString);
      match(NUM, ""); /* switching on token, should not throw error */
      break;
    case ID :
      t = newExpNode(IdK);
      if ((t!=NULL) && (token==ID))
        t->attr.name = copyString(tokenString);
      match(ID, ""); /* switching on token, should not throw error */
      break;
    case LPAREN :
      match(LPAREN, "invalid parenthetical");
      t = simple_exp();
      match(RPAREN, "missing closing parenthesis");
      break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
    }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly 
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t;
  token = getToken();
  t = stmt_sequence();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
