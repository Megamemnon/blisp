#ifndef BLISP_LEXPARSE
#define BLISP_LEXPARSE

#include <stdbool.h>

#define MAX_IDENTIFIER_LENGTH 30
#define MAX_STRING_LENGTH 256

typedef enum{ PAREN, OPERATOR, ATOM, 
  CONSTANT, VARIABLE, NUMBER, TEXT, LISTEN, COMMA, QUOTE} 
  BLisp_Token;

typedef struct BLISP_ASTNODE
{
  int serial;
  char *identifier;
  BLisp_Token type;
  struct BLISP_ASTNODE *down;
  struct BLISP_ASTNODE *next;
  struct BLISP_ASTNODE *nexttoken;
} astnode;

astnode *createAST(char *identifier, BLisp_Token type, int serial);
void freeAST(astnode *node);
astnode *tokenize(char *text);
astnode *term(astnode *token);
astnode *parse(astnode *tokens);
char *getTokenString(astnode *token);
char *getASTString(astnode *node, bool paren);
void printAST(astnode *node);

#endif 