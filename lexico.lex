%{
#include "tokens.h"
#include "string.h"

char * copystring(char *text) {
    int length = strlen(text);

    char *copy = (char *) malloc(length + 1);
    if (copy == NULL) {
        printf("Falha ao alocar memória.");
        exit(1);
    }
    strcpy(copy, text);
    return copy;
}

char* copyescapes(char *text) {
    //TODO


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
\"(([\\][\"])|([^\"\n])+)*\"            { seminfo.s = copystring(yytext); return TK_STRING; }
[1-9][0-9]*|0[xX][0-9a-fA-F]+|o[0-7]*   { return TK_DEC; }
.                                       { return yytext[0]; }

%%


void print_tk_info(token_enum tk) {
    switch (tk)
    {
        case TK_DEC:
            printf("%d %s %s %s\n", tk, enum_to_text[tk - 1], yytext, seminfo.s);
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
