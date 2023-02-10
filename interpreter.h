#ifndef BLISP_INTERPRETER
#define BLISP_INTERPRETER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexparse.h"


astnode *eval(astnode *astree);

#endif