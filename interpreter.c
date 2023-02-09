
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include "lexparse.h"

astnode *run(astnode *astree){
  astnode *node=astree;
  astnode *n=node;
  char buffer[100];
  int bl=0;
  if(node->down){
    node->down=run(node->down);
  }
  if(node->next){
    node->next=run(node->next);
  }
  if(node->type==OPERATOR){
    if(!strcmp(node->identifier,"+")){
      float x=0;
      if(node->down){
        if(node->down->type==NUMBER){
          x+=strtof(node->down->identifier, NULL);
        }
      }
      n=node->next;
      while(n){
        if(n->type==NUMBER) x+=strtof(n->identifier, NULL);
        if(n->down){
          if(n->down->type==NUMBER){
            x+=strtof(n->down->identifier, NULL);
          }
        }
        n=n->next;
      }
      bl=sprintf(buffer, "%f", x);
      free(node->identifier);
      node->identifier=malloc(bl);
      strcpy(node->identifier, buffer);
      node->type=NUMBER;
      freeAST(node->down);
      node->down=NULL;
      freeAST(node->next);
      node->next=NULL;
    }
  }
  return node;
}