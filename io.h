#ifndef BLISP_IO
#define BLISP_IO

typedef enum { BLISP_STREAM_CONSOLE, BLISP_STREAM_FILE} BLisp_Stream;

char *input(char *prompt, BLisp_Stream stream);
void output(char * memfile, char*pathname, long bytes, BLisp_Stream stream);
void newLine();

#endif 