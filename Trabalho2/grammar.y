%{
#include <stdio.h>
#include <string.h>

int yylex();
void yyerror(const char *str);
int yywrap();

%}

%union{
  int i;
  char *s;
  float f;
}

%token	TK_INT
%token	TK_FLOAT
%token	TK_CHAR
%token	TK_VOID
%token	TK_NEW
%token	TK_IF
%token	TK_ELSE
%token	TK_WHILE
%token	TK_AS
%token	TK_RETURN
%token	TK_DEC
%token	TK_REAL
%token	TK_ID
%token	TK_EQUAL
%token  TK_NOTEQUAL
%token	TK_LESSEQUAL
%token	TK_GREATEREQUAL
%token	TK_AND
%token	TK_OR
%token	TK_STRING

%%

programa:              lista_definicoes                             {;};

lista_definicoes:      definicao                                    {;}
                   |   definicao lista_definicoes                   {;};

definicao:             def_variavel                                 {;}
                   |   def_funcao                                   {;};

def_variavel:          TK_ID ':' tipo ';'                           {;};

lista_def_var:         lista_def_var def_variavel                   {;}
                   |   /* vazio*/                                   {;};

tipo:                  tipo_primitivo                               {;}
                   |   tipo '[' ']'                                 {;};

tipo_primitivo:         TK_INT                                      {;}
                   |    TK_FLOAT                                    {;}
                   |    TK_CHAR                                     {;}
                   |    TK_VOID                                     {;};

def_funcao:             TK_ID '(' parametros ')' ':' tipo bloco     {;};

parametros:             lista_params                                {;}
                   |    /* vazio*/                                  {;};

lista_params:           parametro                                   {;}
                   |    parametro ',' parametros                    {;};

parametro:              TK_ID ':' tipo                              {;};

bloco:                  '{' lista_def_var lista_comandos '}'        {;};

lista_comandos:         lista_comandos comando                      {;}
                   |    /*vazio*/                                   {;};

comando:                bloco
                   |    expressao ';'                               {;}
                   |    '@' expressao ';'                           {;}
                   |    cmd_return                                  {;}
                   |    variavel '=' expressao ';'                  {;}
                   |    TK_IF  expLogica  bloco                     {;}
                   |    TK_IF  expLogica  bloco  TK_ELSE  bloco     {;}
                   |    TK_WHILE    expLogica    bloco              {;};


cmd_return:             TK_RETURN expressao ';'                     {;};

variavel:               TK_ID                                       {;}
                   |    expressao_base '[' expLogica ']'            {;};

chamada:                TK_ID '(' expressoes ')'                    {;};

expressoes:             lista_exp                                   {;}
                   |     /* vazio*/                                 {;};

lista_exp:              expLogica                                   {;}
                   |    expLogica ',' lista_exp                     {;};

expressao_base:          TK_DEC                                     {;}
                     |   TK_REAL                                    {;}
                     |   TK_STRING                                  {;}
                     |   variavel                                   {;}
                     |   '(' expLogica ')'                          {;}
                     |   chamada                                    {;};
                     
expressao: expNew                                                   {;}
          | expAs                                                   {;};

expAs: expLogica TK_AS tipo                                         {;};

expNew: TK_NEW tipo '[' expLogica ']'                               {;}
        | expLogica                                                 {;};

expLogica:      expLogica TK_OR expAnd                              {;}
                | expAnd                                            {;};

expAnd:         expCmp                                              {;}
                | expAnd TK_AND expCmp                              {;};

opCmp:   TK_EQUAL                                                   {;}
        | TK_NOTEQUAL                                               {;}
        | TK_LESSEQUAL                                              {;}
        |	TK_GREATEREQUAL                                     {;}
        | '<'                                                       {;}
        | '>'                                                       {;};

expCmp: expCmp opCmp expAdd                                         {;}
        | expAdd                                                    {;};

addOp: '+'                                                          {;}
      | '-'                                                         {;};

expAdd: expAdd addOp expMult                                        {;}
        | expMult                                                   {;};

multOp: '*'                                                         {;}
      | '/'                                                         {;};

expMult: expMult multOp expUnaria                                   {;}
        | expUnaria                                                 {;};


expUnaria:              expressao_base                              {;}
                | '-'   expressao_base                              {;}
                | '!'   expressao_base                              {;};



%%

void yyerror(const char *str) {
  printf("error: %s\n",str);
}

int yywrap() {
  return 1;
}

int main(void) {
  yyparse();
  return 0;
}
