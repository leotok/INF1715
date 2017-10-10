%code requires {
  #include <stdio.h>
  #include <string.h>
  #include "ast.h"
}

%code {
  int   yylex(void);
  void  yyerror(const char *str);
  int   yywrap(void);

  ABS_node* programNode = NULL;
}

%{

%}

%union {
    char* 	vString;
    int 	vInt;	
    float 	vFloat;
 
    struct id {
        char* 	name;
        int 	lineNumber;
    } vId;

    ABS_node* vNode;	
}

%token	<vInt> TK_INT
%token	<vInt> TK_FLOAT
%token	<vInt> TK_CHAR
%token	<vInt> TK_VOID
%token	<vInt> TK_NEW
%token	<vInt> TK_IF
%token	<vInt> TK_ELSE
%token	<vInt> TK_WHILE
%token	<vInt> TK_AS
%token	<vInt> TK_RETURN
%token	<vInt> TK_ID
%token	<vInt> TK_EQUAL
%token  <vInt> TK_NOTEQUAL
%token	<vInt> TK_LESSEQUAL
%token	<vInt> TK_GREATEREQUAL
%token	<vInt> TK_AND
%token	<vInt> TK_OR
%token	<vInt> TK_DEC
%token	<vFloat> TK_REAL
%token	<vString> TK_STRING



%type <vInt> tipo_primitivo tipo
%type <vNode> programa def_variavel lista_def_var variavel
%type <vNode> expressao_base exp_unaria exp_mult exp_add exp_cmp exp_and expressao exp_or exp_as
%type <vNode> lista_exp lista_exp2 chamada comando lista_comandos 
%type <vNode> bloco lista_params parametros parametro
%type <vNode> def_funcao definicao lista_definicoes

%%

programa:               lista_definicoes                                  { programNode = $$; }
;

lista_definicoes:       /* vazio */                                       {;}
                    |   lista_definicoes definicao                        { $$ = ABS_mergeList($1, $2); }
;

definicao:              def_variavel                                      { $$ = $1; }
                    |   def_funcao                                        { $$ = $1; }
;

def_variavel:           TK_ID ':' tipo ';'                                { $$ = ABS_declVar($3, $1); }
;

tipo:                   tipo_primitivo                                    { $$ = $1 ;}
                    |   tipo '[' ']'                                      { $$ = TYP_array($1) ;}
;

tipo_primitivo:         TK_INT                                            { $$ = TYP_getID(CHAR); }
                    |   TK_FLOAT                                          { $$ = TYP_getID(INT); }
                    |   TK_CHAR                                           { $$ = TYP_getID(FLOAT); }
                    |   TK_VOID                                           {;}
;

def_funcao:             TK_ID '(' parametros ')' ':' tipo bloco           { $$ = ABS_declFunc($6, ABS_id($1.name), $3, $7); }
;

parametros:             /* vazio */                                       { $$ = NULL; }
                    |   lista_params                                      { $$ = $1; }
;

lista_params:           parametro                                         { $$ = $1; }
                    |   lista_params ',' parametro                        { $$ = ABS_mergeList($1,$3); }
;

parametro:              TK_ID ':' tipo                                    { $$ = ABS_declVar($3, ABS_id($1.name)); }
;

bloco:                  '{' lista_def_var lista_comandos '}'              { $$ = ABS_block($2, $3); }
;

lista_def_var:          /*vazio*/                                         { $$ = NULL; }
                    |   lista_def_var def_variavel                        { $$ = ABS_mergeList($1, $2); }
;

lista_comandos:         /*vazio*/                                         { $$ = NULL; }
                    |    comando  lista_comandos                          { $$ = ABS_mergeList($2, $1); }
;

comando:                bloco                                             { $$ = $1; }
                    |   '@' expressao ';'                                 {;}
                    |   TK_RETURN expressao ';'                           { $$ = ABS_cmdRet($2); }
                    |   TK_RETURN ';'                                     { $$ = ABS_cmdRet(NULL); }
                    |   variavel '=' expressao ';'                        { $$ = ABS_cmdAttr($1, $3); }
                    |   TK_IF  expressao  bloco                           { $$ = ABS_cmdIf($2, $3, NULL); }
                    |   TK_IF  expressao  bloco  TK_ELSE  bloco           { $$ = ABS_cmdIf($2, $3, $5); }
                    |   TK_WHILE    expressao    bloco                    { $$ = ABS_cmdWhile($2, $3); }
                    |   chamada ';'                                       { $$ = $1; }
;

variavel:               TK_ID                                             { $$ = ABS_varMono(ABS_id($1.name)); }
                    |   expressao_base '[' expressao ']'                  { $$ = ABS_varArray($1, $3); }
;

chamada:                TK_ID '(' lista_exp ')'                           { $$ = ABS_expCall(ABS_id($1.name), $3); }
;

lista_exp:              /* vazio*/                                        { $$ = NULL; }
                    |   lista_exp2                                        { $$ = $1; }
;

lista_exp2:             expressao                                         { $$ = $1; }
                    |   lista_exp2 ',' expressao                          { $$ = ABS_mergeList($1, $3); }
;

expressao:              exp_or                                            { $$ = $1; }
;

exp_or:                 exp_and                                           { $$ = $1; }
                    |   exp_or TK_OR exp_and                              { $$ = ABS_expOpr(TK_OR, $1, $3); }
;

exp_and:                exp_cmp                                           { $$ = $1; }
                    |   exp_and TK_AND exp_cmp                            { $$ = ABS_expOpr(TK_AND, $1, $3); }
;

exp_cmp:                exp_add                                           { $$ = $1; }
                    |   exp_cmp TK_EQUAL exp_add                          { $$ = ABS_expOpr(TK_EQUAL, $1, $3); }
                    |   exp_cmp TK_NOTEQUAL exp_add                       { $$ = ABS_expOpr(TK_NOTEQUAL, $1, $3); }
                    |   exp_cmp TK_LESSEQUAL exp_add                      { $$ = ABS_expOpr(TK_LESSEQUAL, $1, $3); }
                    |   exp_cmp TK_GREATEREQUAL exp_add                   { $$ = ABS_expOpr(TK_GREATEREQUAL, $1, $3); }
                    |   exp_cmp'<' exp_add                                { $$ = ABS_expOpr('<', $1, $3); }
                    |   exp_cmp '>' exp_add                               { $$ = ABS_expOpr('>', $1, $3); }
;

exp_add:                exp_mult                                          { $$ = $1; }
                    |   exp_add '+' exp_mult                              { $$ = ABS_expOpr('+', $1, $3); }
                    |   exp_add '-' exp_mult                              { $$ = ABS_expOpr('-', $1, $3); }
;

exp_mult:               exp_unaria                                        { $$ = $1; }
                    |   exp_mult '*' exp_unaria                           { $$ = ABS_expOpr('*', $1, $3); }
                    |   exp_mult '/' exp_unaria                           { $$ = ABS_expOpr('/', $1, $3); }
;

exp_unaria:             exp_as                                            { $$ = $1; }
                    |   '-'   exp_unaria                                  { $$ = ABS_expOpr('-', $2, NULL); }
                    |   '!'   exp_unaria                                  { $$ = ABS_expOpr('!', $2, NULL); }
;


exp_as:                 expressao_base                                    { $$ = $1; }
                    |   expressao_base TK_AS tipo                         {;}
;

expressao_base:         TK_DEC                                            { $$ = ABS_literalInt($1); }
                    |   TK_REAL                                           { $$ = ABS_literalFloat($1); }
                    |   TK_STRING                                         { $$ = ABS_literalString($1); }
                    |   variavel                                          { $$ = ABS_expVar($1); }
                    |   '(' expressao ')'                                 { $$ = ABS_expParented($2); }
                    |   chamada                                           { $$ = $1; }
                    |   TK_NEW tipo '[' expressao ']'                     { $$ = ABS_expNew($2, $4); }
;

%%

void yyerror(const char *str) {
  fprintf(stderr,"error: %s\n",str);
}

int yywrap(void) {
  return 1;
}

int main(void) {
  int ret;
	
	ret = yyparse();
	
	ABS_print(1);
	
	BUF_Free();
  return 0;
}
