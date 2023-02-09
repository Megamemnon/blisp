#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexparse.h"
#include "io.h"



astnode *createAST(char *identifier, BLisp_Token type, int serial){
  astnode *a=malloc(sizeof(astnode));
  a->type=type;
  a->identifier=malloc(strlen(identifier)+1);
  strcpy(a->identifier, identifier);
  a->serial=serial;
  a->down=NULL;
  a->next=NULL;
  a->nexttoken=NULL;
  return a;
}

void freeAST(astnode *node){
  if(!node) return;
  if(node->down) freeAST(node->down);
  if(node->next) freeAST(node->next);
  free(node);
}

astnode *tokenize(char *text){
  if(!text) return NULL;
  astnode *root=NULL;
  astnode *current=NULL;
  int i=0;
  int textlen=strlen(text);
  bool inword=false;
  bool inop=false;
  bool innum=false;
  char identifier[MAX_IDENTIFIER_LENGTH+1];
  int identindex=0;
  BLisp_Token wordtype;
  astnode *tnode=NULL;
  while(i<textlen){
    char c=text[i];
    char nc=0;
    if(i<textlen) nc=text[i];
    if(c==' ' || c=='\n' || c=='\t' || c=='(' || c==')' 
     || c=='[' || c==']' || c=='{' || c=='}' || c==',' 
     || c=='"' || c=='#'){
      if(inword){
        identifier[identindex]=0;
        tnode=createAST(identifier, wordtype, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
        inword=false;
      } 
      if(inop){
        identifier[identindex]=0;
        tnode=createAST(identifier, OPERATOR, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
        inop=false;
      }
      if(innum && !isdigit(nc)){
        identifier[identindex]=0;
        tnode=createAST(identifier, NUMBER, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
        innum=false;
      }
    }
    if(c=='(' || c==')'){
      char id[2];
      id[0]=c;
      id[1]='\0';
      tnode=createAST(id, PAREN, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
    }
    else if(c=='[' || c==']' || c=='{' || c=='}'){
      char id[2];
      id[0]=c;
      id[1]='\0';
      tnode=createAST(id, LISTEN, 0);      
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
    }
    else if(c==','){
      char id[2];
      id[0]=c;
      id[1]='\0';
      tnode=createAST(id, COMMA, 0);      
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
    }
    else if(c=='\''){
      char id[2];
      id[0]=c;
      id[1]='\0';
      tnode=createAST(id, QUOTE, 0);      
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
    }
    else if(c=='#'){
      while(c!='\n' && i<textlen) c=text[++i];
    }
    else if(c=='"'){
      char buffer[MAX_STRING_LENGTH];
      int bi=0;
      int quotecount=0;
      while(quotecount<2){
        buffer[bi++]=c;
        if(c=='"') quotecount++;
        if(i+1>=textlen){
          buffer[bi++]='"';
          buffer[bi]=0;
        }
        else {
          c=text[++i];
        }
      }
      i--;
      tnode=createAST(buffer, TEXT, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
    }
    else if(!innum && !inword && !inop && c=='-' && isdigit(nc)){
      innum=true;
      identindex=0;
      identifier[identindex++]=c;
      identifier[identindex++]=nc;
      i++;
    }
    else if(innum && c=='.' && isdigit(nc)){
      identifier[identindex++]=c;
      identifier[identindex++]=nc;
      i++;
    }
    else if(isdigit(c)){
      if(innum){
        identifier[identindex++]=c;
      }
      else{
        if(inop){
          identifier[identindex]=0;
          tnode=createAST(identifier, OPERATOR, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
          inop=false;
        }
        if(inword){
          identifier[identindex]=0;
          tnode=createAST(identifier, wordtype, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
          inword=false;
        }
        innum=true;
        identindex=0;
        identifier[identindex++]=c;
      }
    }
    else if(isalnum(c)){
      if(inword){
        identifier[identindex++]=c;
      }
      else{
        if(inop){
          identifier[identindex]=0;
          tnode=createAST(identifier, OPERATOR, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
          inop=false;
        }
        if(innum){
          identifier[identindex]=0;
          tnode=createAST(identifier, NUMBER, 0);
        if(!root){
          root=tnode;
          current=root;
        } 
        else {
          current->next=tnode;
          current=current->next;
        }
          innum=false;
        }
        inword=true;
        if(isupper(c)){
          wordtype=VARIABLE;
        }
        else {
          wordtype=CONSTANT;
        }
        identindex=0;
        identifier[identindex++]=c;
      }
    }
    else if(c!=' ' && c!='\n' && c!='\t'){
      if(inop){
        identifier[identindex++]=c;
      }
      else {
        if(inword){
          identifier[identindex]=0;
          tnode=createAST(identifier, wordtype, 0);
          if(!root){
            root=tnode;
            current=root;
          } 
          else {
            current->next=tnode;
            current=current->next;
          }
          inword=false;
        }
        if(innum){
          identifier[identindex]=0;
          tnode=createAST(identifier, NUMBER, 0);
          if(!root){
            root=tnode;
            current=root;
          } 
          else {
            current->next=tnode;
            current=current->next;
          }
          innum=false;
        }
        inop=true;
        identindex=0;
        identifier[identindex++]=c;
      }
    }
    i++;
  }
  return root;
}

astnode *term(astnode *token){
  static int serial=0;
  astnode *myterm=NULL;
  if(token->type==PAREN && token->identifier[0]=='('){
    token=token->next;
    myterm=createAST(token->identifier, OPERATOR, serial++);
    astnode *mytermnext=myterm;
    token=token->next;
    while(token){
      if(token->type==PAREN && token->identifier[0]==')'){
        myterm->nexttoken=token->next;
        return myterm;
      }
      if(token->type!=PAREN){
        BLisp_Token t=ATOM;
        if(token->type==NUMBER) t=NUMBER;
        if(token->type==TEXT) t=TEXT;
        mytermnext->next=createAST(token->identifier, t, serial++);
        mytermnext=mytermnext->next;
        token=token->next;
      }
      if(token->type==PAREN && token->identifier[0]=='('){
        mytermnext->down=term(token);
        token=mytermnext->down->nexttoken;
        mytermnext->down->nexttoken=NULL;
      }
    }
  }
  //TODO ERROR - Left Parenthesis expected
  return NULL;
}

astnode *parse(astnode *tokens){
  astnode *root = NULL;
  astnode *current=NULL;
  astnode *token=tokens;
  while(token){
    current=term(token);
    if(!root) root=current;
    token=current->nexttoken;
    current->nexttoken=NULL;
  }
  return root;
}

char *getTokenString(astnode *token){
  if(!token) return NULL;
  char *thisstr=malloc(strlen(token->identifier)+2);
  strcpy(thisstr, token->identifier);
  strcat(thisstr, " ");
  char *nextstr=getTokenString(token->next);
  char *returnstr=NULL;
  if(nextstr){
    returnstr=malloc(strlen(thisstr)+strlen(nextstr)+1);
    strcpy(returnstr, thisstr);
    strcat(returnstr, nextstr);
    free(thisstr);
    free(nextstr);
  }
  else{
    returnstr=thisstr;
  }
  return returnstr;
}

char *getASTString(astnode *node, bool paren){
  if(!node) return NULL;
  int s=strlen(node->identifier)+2;
  char *thisstr=malloc(s);
  strcpy(thisstr, node->identifier);
  strcat(thisstr, " ");
  if(paren){
    s+=1;
    char *thstr=malloc(s);
    strcpy(thstr, "(");
    strcat(thstr, thisstr);
    free(thisstr);
    thisstr=thstr;
  }
  char *downstr=getASTString(node->down, true);
  if(node->down) s+=strlen(downstr)+2;
  char *nextstr=getASTString(node->next, false);
  if(node->next) s+=strlen(nextstr);
  char *returnstr=malloc(s);
  strcpy(returnstr, thisstr);
  if(node->down){
    strcat(returnstr, downstr);
    free(downstr);
  }
  if(node->next){
    strcat(returnstr, nextstr);
    free(nextstr);
  }
  if(paren){
    s+=1;
    char *thstr=malloc(s);
    strcpy(thstr, returnstr);
    strcat(thstr, ")");
    free(returnstr);
    returnstr=thstr;
  }
  return returnstr;
}