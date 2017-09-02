#include <stdio.h>
#include "tokens.h"


int main (void) {

  int token = 1;
  token = yylex();
  while (token) {
    switch (token)
    {
        case TK_DEC:
            printf("%d %d\n",token, seminfo.i);
            break;
        case TK_FLOAT:
            printf("%d %f\n",token, seminfo.f);
            break;
        case TK_STRING:
            printf("%d %s\n",token, seminfo.s);
            break;
        default:
            printf("%d\n",token);
            break;
    }
    token = yylex();
  }
  return 0;
}
