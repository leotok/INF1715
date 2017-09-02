%option noinput
%option nounput
%option noyywrap
%{
#include "tokens.h"
#include "string.h"

static char* createstring(int length) {

  char *str = (char *) malloc(length);
  if (str == NULL) {
      printf("Falha ao alocar memória.");
      exit(1);
  }
  return str;
}

static char * copystring(char *text) {

    int length = strlen(text);
    char *copy = createstring(length+1);
    strcpy(copy, text);
    return copy;
}

static char* copyescapes(char *text) {

    int length = strlen(text);
    char *copy = createstring(length);
    int i,j, flag = 0;
    for (j = 0, i = 1; i < length - 1; i++) {

      if (flag) {
          switch(text[i]) {
            case '\\':
              copy[j] = '\\';
              j++;
              break;
            case 'n':
              copy[j] = '\n';
              j++;
              break;
            case 't':
              copy[j] = '\t';
              j++;
              break;
            case '\"':
              copy[j] = '\"';
              j++;
              break;
            default:
              printf("Escape Invalido");
              exit(1);
          }
        flag = 0;
      }
      else {
        if(text[i] == '\\') {
          flag = 1;
        }
        else {
          copy[j] = text[i];
          j++;
        }
      }

    }
    copy[j] = '\0';
    return copy;
}

SemInfo seminfo;

%}


%%

#[^\n]*                                 {}
"\n"                                    {}
" "                                     {}
"as"                                    { return TK_AS; }
"int"                                   { return TK_INT; }
"float"                                 { return TK_FLOAT; }
"char"                                  { return TK_CHAR; }
"void"                                  { return TK_VOID; }
"new"                                   { return TK_NEW; }
"if"                                    { return TK_IF; }
"else"                                  { return TK_ELSE; }
"while"                                 { return TK_WHILE; }
"return"                                { return TK_RETURN; }
"="		                                  { return '='; }
"!"		                                  { return '!'; }
"("		                                  { return '('; }
")"		                                  { return ')'; }
"{"		                                  { return '{'; }
"}"		                                  { return '}'; }
"["		                                  { return '['; }
"]"		                                  { return ']'; }
"+"		                                  { return '+'; }
"-"		                                  { return '-'; }
","		                                  { return ','; }
"@"                                     { return '@'; }
"*"		                                  { return '*'; }
"/"		                                  { return '/'; }
"=="                                    { return TK_EQUAL; }
"<="                                    { return TK_LESSEQUAL; }
">="                                    { return TK_GREATEREQUAL; }
"&&"                                    { return TK_AND; }
"||"                                    { return TK_OR; }
[a-zA-Z_][a-zA-Z0-9_]*                  { seminfo.s = copystring(yytext); return TK_ID; }
\"(\\.|[^\\"])*\"                       { seminfo.s = copyescapes(yytext); return TK_STRING; }


[1-9][0-9]*                             { seminfo.i = atoi(yytext); return TK_DEC; }
0[xX][0-9a-fA-F]+|o[0-7]*               { seminfo.i = strtol(yytext,NULL,0); return TK_DEC; }

[0-9]+[Ee][+-]?[0-9]+(f|F)?		          { seminfo.f = strtof(yytext,NULL); return TK_FLOAT;}
[0-9]*"."[0-9]+([Ee][+-]?[0-9]+)?(f|F)?	{ seminfo.f = strtof(yytext,NULL); return TK_FLOAT;}
[0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?(f|F)? { seminfo.f = strtof(yytext,NULL); return TK_FLOAT;}


.                                       { return yytext[0]; }

%%

/*
void print_tk_info(token_enum tk) {
    switch (tk)
    {
        case TK_DEC:
            printf("%d %s %s %d\n", tk, enum_to_text[tk - 1], yytext, seminfo.i);
            break;
        case TK_FLOAT:
            printf("%d %s %s %f\n", tk, enum_to_text[tk - 1], yytext, seminfo.f);
            break;
        default:
            printf("%d %s %s\n", tk, enum_to_text[tk - 1], yytext);
    }
}

int main() {
    token_enum tk = yylex();

    while (tk != 0)
    {
        print_tk_info(tk);
        tk = yylex();
    }

}
*/
