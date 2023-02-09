#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "io.h"

char *getLinefromSTDIN(char *prompt){
  char *buf;
  buf=readline(prompt);
  if(strlen(buf)>0) add_history(buf);
  return buf;
}

void putLinetoSTDOUT(char *string){
  printf("%s", string);
  fflush(stdout);
}

void newLine(){
  printf("\n");
  fflush(stdout);
}

long getFileSize(const char *pathname){
  long sz;
  FILE *f;

  f = fopen(pathname, "r");
  if(f == NULL) return 0;
  fseek(f, 0L, SEEK_END);
  sz = ftell(f);
  fclose(f);
  return sz;
}

char *getLinesfromFILE(const char *pathname){
  char c;
  long sz = getFileSize(pathname);
  long i = 0;
  char *memfile = malloc(sz+1);
  if(memfile == NULL) return NULL;
  FILE *f = fopen(pathname, "r");
  if(f == NULL) return NULL;
  do
  {
    c = fgetc(f);
    memfile[i++] = c;
  } while (c != EOF);
  fclose(f);
  return memfile;
}

char *putLinestoFILE(const char *pathname, char *memfile, long bytes){
  char c;
  long sz = getFileSize(pathname);
  long i = 0;
  if(memfile == NULL) return NULL;
  FILE *f = fopen(pathname, "w");
  if(f == NULL) return NULL;
  do
  {
    c=memfile[i++];
    fputc(c, f);
  } while (i<bytes);
  fclose(f);
}

char *input(char * prompt, BLisp_Stream stream){
  switch(stream){
    case BLISP_STREAM_CONSOLE:
      return getLinefromSTDIN(prompt);
      break;
    case BLISP_STREAM_FILE:
      return getLinesfromFILE(prompt);
      break;
    default:
      return NULL;
      break;
  }
}

void output(char * memfile, char*pathname, long bytes, BLisp_Stream stream){
  switch(stream){
    case BLISP_STREAM_CONSOLE:
      putLinetoSTDOUT(memfile);
      break;
    case BLISP_STREAM_FILE:
      putLinestoFILE(pathname, memfile, bytes);
      break;
    default:
      break;
  }
}
