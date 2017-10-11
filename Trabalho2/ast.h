#ifndef ABSTRACTSYNTAXTREE_HEADER
#define ABSTRACTSYNTAXTREE_HEADER


typedef enum typeEnum {
	CHAR,
	INT,
	FLOAT,
	VOID,
    TYPEQTY 
} TYP_typeEnum;

int TYP_getID(TYP_typeEnum type);
int TYP_array(int type);


// Tree node types
typedef enum nodeTypeEnum {
	TYPE_ID,
	TYPE_EXP,
	TYPE_DECL,
	TYPE_VAR,
	TYPE_CMD
	
} AST_nodeTypeEnum;


// Union Tags
typedef enum unionTag {	
	ID,
	CAST,

	EXP_BINOP,
	EXP_CALL,
	EXP_VAR,
	EXP_PAREN,
	EXP_UNOP,
	EXP_NEW,    

	LIT_INT,
	LIT_FLOAT,
	LIT_STRING,

	VAR_MONO,
	VAR_ARRAY,

	DEC_VAR,
	DEC_FUNC,

	CMD_IF,
	CMD_WHILE,
	CMD_ATTR,
	CMD_EXP,
	CMD_BLOCK,
	CMD_RET
	
} ABS_unionTag;



typedef struct aST_node AST_node;				

typedef struct nodeId AST_nodeId;				
typedef struct nodeVar AST_nodeVar;				
typedef struct nodeExp AST_nodeExp;				

typedef union nodeDecl AST_nodeDecl;			
typedef union nodeCMD AST_nodeCMD;			



// Types
struct typNodeType {
	int size;
};


// Id
struct nodeId {
	char* name;
};

// Variable
struct nodeVar {
    AST_node* id;
    AST_node* index;
};


// Expression
struct nodeExp {
    int type;
    
    union {
        AST_node* 	varexp;
        AST_node* 	cast; // Futuro
        AST_node* 	parenexp;
        

        union {
            int 	vInt;
            float 	vFloat;
            char* 	vString;
        } literal;
     
        struct {
            int 		opr;
            AST_node* 	exp1;
            AST_node* 	exp2;
        } operexp;

        struct {
            AST_node* 	exp1;
            AST_node* 	exp2;
        } callexp;

        // New Array Info
        struct {
            int 		type;
            AST_node*	exp;
        } newexp;
        
    } data;
};




// Declarations
union nodeDecl {

	struct {
		int 		type;
		AST_node* 	id;
    } vardecl;
    
	struct {
		int 		type;
		AST_node* 	id;
		AST_node* 	param;
		AST_node* 	block;
	} funcdecl;
};



// Commands
union nodeCMD {
	
	struct {
		AST_node* 	exp;
		AST_node* 	cmd1;
		AST_node* 	cmd2;
	} ifcmd;

	struct {
		AST_node* 	exp;
		AST_node* 	cmd;
	} whilecmd;

	struct {
		AST_node* 	var;
		AST_node* 	exp;
	} attrcmd;

	struct {
		AST_node* 	exp;
	} retcmd;

	struct {
		AST_node* 	decl;
		AST_node* 	cmd;
	} blockcmd;
	
	struct {
		AST_node* 	exp;
	} expcmd;
};




// Abstract Syntax Tree Node
struct aST_node {
	AST_nodeTypeEnum 	type;
	ABS_unionTag 		tag;
	int 				line;

	// List Struture
	AST_node* 	next;
	AST_node* 	last;	

	union {
		AST_nodeId 			id;
		AST_nodeExp 		exp;
		AST_nodeVar 		var;
		AST_nodeDecl 		decl;
		AST_nodeCMD 		cmd;
	} node;
};

// -------------

#include "grammar.tab.h"
extern AST_node* programNode;


// FUNCTIONS --------------------------------------------------------------------------

// Literals
AST_node* ABS_literalInt(int value);
AST_node* ABS_literalFloat(float value);
AST_node* ABS_literalString(char * str);

// Id
AST_node* ABS_id(char * name);

// Cast ( usar no futuro , costurar arvore )
AST_node* ABS_typecast( int type , AST_node* exp);

// Expressions
AST_node* ABS_expOpr( int opr, AST_node* exp1, AST_node* exp2 );
AST_node* ABS_expParented( AST_node* exp );
AST_node* ABS_expVar( AST_node* var );
AST_node* ABS_expCall( AST_node* exp1 , AST_node* exp2 );
AST_node* ABS_expNew( int type, AST_node* exp );

// Variables
AST_node* ABS_varArray( AST_node* exp1 , AST_node* exp2 );
AST_node* ABS_varMono( AST_node* id );

// Declarations
AST_node* ABS_declVar( int type , AST_node* id );
AST_node* ABS_declFunc( int type , AST_node* id , AST_node* param , AST_node* block );

// Commands
AST_node* ABS_cmdIf( AST_node* exp , AST_node* cmd1 , AST_node* cmd2 );
AST_node* ABS_cmdWhile( AST_node* exp , AST_node* cmd);
AST_node* ABS_cmdAttr( AST_node* var , AST_node* exp );
AST_node* ABS_cmdRet( AST_node* exp );
AST_node* ABS_cmdExp(AST_node* exp);
AST_node* ABS_block( AST_node* decl , AST_node* cmd );

// Lists 
AST_node* ABS_mergeList( AST_node* list , AST_node* element );


// Print ABS tree
void ABS_print_tree();


#endif










