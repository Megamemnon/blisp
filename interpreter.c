
#include "interpreter.h"



float prim_add(float a, float b){
  float result;
  result=a+b;
  return result;
}

typedef float(*funcfloat)(float, float);

funcfloat getPrimitiveFunc(char *atom){
  if(!strcmp("+",atom)) return prim_add;
  return NULL;
}

astnode *eval_float(astnode *astree, funcfloat func){
  astnode *node=astree;
  astnode *n=node;
  float a, b;
  char buffer[100];
  int bl=0;
  if(node->down){
    if(node->down->type==NUMBER){
      a=0;
      b=strtof(node->down->identifier, NULL);
      a=func(a,b);
    }
  }
  n=node->next;
  while(n){
    if(n->type==NUMBER){
      b=strtof(n->identifier, NULL);
      a=func(a,b);
    }
    if(n->down){
      if(n->down->type==NUMBER){
        b=strtof(n->down->identifier, NULL);
        a=func(a,b);
      }
    }
    n=n->next;
  }
  bl=sprintf(buffer, "%f", a);
  free(node->identifier);
  node->identifier=malloc(bl+1);
  strcpy(node->identifier, buffer);
  node->type=NUMBER;
  freeAST(node->down);
  node->down=NULL;
  freeAST(node->next);
  node->next=NULL;
  return node;
}

astnode *eval(astnode *astree){
  astnode *node=astree;
  astnode *n=NULL;
  if(node->down){
    node->down=eval(node->down);
  }
  if(node->next){
    node->next=eval(node->next);
  }
  if(node->type==ATOM){
    funcfloat f=getPrimitiveFunc(node->identifier);
    if(f!=NULL){
      node=eval_float(node, f);
    } 
  }
  if(node->type==PAREN){
    if(!node->down && node->next){
      n=node->next;
      node->next=NULL;
      freeAST(node);
      return n;
    }
    if(node->down && !node->next){
      n=node->down;
      node->down=NULL;
      freeAST(node);
      return n;
    }
  }
  return node;
}