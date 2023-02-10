#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "io.h"
#include "lexparse.h"
#include "interpreter.h"

#define DEBUG

int main(int argc, char const *argv[]){
  output("blisp\nCopyright (c) 2023 Brian O'Dell\n\n", NULL, 0, BLISP_STREAM_CONSOLE);
#ifdef DEBUG
  char *memfile=input("/home/brian/git/blisp/test", BLISP_STREAM_FILE);
#else
  if(argc != 2){
    output("usage: blisp programfile\n", NULL, 0, BLISP_STREAM_CONSOLE);
    return 1;
  }
#endif
  astnode *tokens=tokenize(memfile);
  char *tokenstr=getTokenString(tokens);
  output(tokenstr, NULL, 0, BLISP_STREAM_CONSOLE);
  newLine();
  astnode *nodes=parse(tokens);
  char *aststr=getASTString(nodes, true);
  output(aststr, NULL, 0, BLISP_STREAM_CONSOLE);
  newLine();
  astnode *result=eval(nodes);
  char *rtext=getASTString(result, true);
  output(rtext, NULL, 0, BLISP_STREAM_CONSOLE);
  newLine();
}