#include <stdio.h>
#include "tokens.h"

char *enum_to_text[] = {
    "TK_INT",
    "TK_FLOAT",
    "TK_CHAR",
    "TK_VOID",
    "TK_NEW",
    "TK_IF",
    "TK_ELSE",
    "TK_WHILE",
    "TK_AS",
    "TK_RETURN",
    "TK_DEC",
    "TK_REAL",
    "TK_ID",
    "TK_EQUAL",
    "TK_LESSEQUAL",
    "TK_GREATEREQUAL",
    "TK_AND",
    "TK_OR",
    "TK_STRING"
};

int main (void) {

  int token = 1;
  token = yylex();
  while (token) {
    printf("Line %d\t", yy_lines);
    
    if (token <= CHAR_MAX){
      printf("%c\n",token);
    }
    else {
      switch (token)
      {
        case TK_DEC:
            printf("%s %d\n",enum_to_text[token-CHAR_MAX-1], seminfo.i);
            break;
        case TK_REAL:
            printf("%s %f\n",enum_to_text[token-CHAR_MAX-1], seminfo.f);
            break;
        case TK_STRING:
            printf("%s %s\n",enum_to_text[token-CHAR_MAX-1], seminfo.s);
            break;
        case TK_ID:
            printf("%s %s\n",enum_to_text[token-CHAR_MAX-1], seminfo.s);
            break;
        default:
            printf("%s\n",enum_to_text[token-CHAR_MAX-1]);
            break;
    }
  }
  token = yylex();
}

  return 0;
}
