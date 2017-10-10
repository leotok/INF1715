#ifndef ABSTRACTSYNTAXTREE_HEADER
#define ABSTRACTSYNTAXTREE_HEADER

int TYP_getID(TYP_typeEnum type);
int TYP_array(int type);


typedef enum typeEnum {
	CHAR,
	INT,
	FLOAT,
	VOID,
    TYPEQTY 
} TYP_typeEnum;


// Tree node types
typedef enum nodeTypeEnum {
	TYPE_ID,
	TYPE_EXP,
	TYPE_DECL,
	TYPE_VAR,
	TYPE_CMD
	
} ABS_nodeTypeEnum;


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



typedef struct abs_node ABS_node;				

typedef struct nodeId ABS_nodeId;				
typedef struct nodeVar ABS_nodeVar;				
typedef struct nodeExp ABS_nodeExp;				

typedef union nodeDecl ABS_nodeDecl;			
typedef union nodeCMD ABS_nodeCMD;			



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
    ABS_node* id;
    ABS_node* index;
};


// Expression
struct nodeExp {
    int type;
    
    union {
        ABS_node* 	varexp;
        ABS_node* 	cast; // Futuro
        ABS_node* 	parenexp;
        

        union {
            int 	vInt;
            float 	vFloat;
            char* 	vString;
        } literal;
     
        struct {
            int 		opr;
            ABS_node* 	exp1;
            ABS_node* 	exp2;
        } operexp;

        struct {
            ABS_node* 	exp1;
            ABS_node* 	exp2;
        } callexp;

        // New Array Info
        struct {
            int 		type;
            ABS_node*	exp;
        } newexp;
        
    } data;
};




// Declarations
union nodeDecl {

	struct {
		int 		type;
		ABS_node* 	id;
    } vardecl;
    
	struct {
		int 		type;
		ABS_node* 	id;
		ABS_node* 	param;
		ABS_node* 	block;
	} funcdecl;
};



// Commands
union nodeCMD {
	
	struct {
		ABS_node* 	exp;
		ABS_node* 	cmd1;
		ABS_node* 	cmd2;
	} ifcmd;

	struct {
		ABS_node* 	exp;
		ABS_node* 	cmd;
	} whilecmd;

	struct {
		ABS_node* 	var;
		ABS_node* 	exp;
	} attrcmd;

	struct {
		ABS_node* 	exp;
	} retcmd;

	struct {
		ABS_node* 	decl;
		ABS_node* 	cmd;
	} blockcmd;
	
	struct {
		ABS_node* 	exp;
	} expcmd;
};




// Abstract Syntax Tree Node
struct abs_node {
	ABS_nodeTypeEnum 	type;
	ABS_unionTag 		tag;
	int 				line;

	// List Struture
	ABS_node* 	next;
	ABS_node* 	last;	

	union {
		ABS_nodeId 			id;
		ABS_nodeExp 		exp;
		ABS_nodeVar 		var;
		ABS_nodeDecl 		decl;
		ABS_nodeCMD 		cmd;
	} node;
};

// -------------

#include "monga.tab.h"
extern ABS_node* programNode;


// FUNCTIONS --------------------------------------------------------------------------

// Literals
ABS_node* ABS_literalInt(int value);
ABS_node* ABS_literalFloat(float value);
ABS_node* ABS_literalString(char * str);

// Id
ABS_node* ABS_id(char * name);

// Cast ( usar no futuro , costurar arvore )
ABS_node* ABS_typecast( int type , ABS_node* exp);

// Expressions
ABS_node* ABS_expOpr( int opr, ABS_node* exp1, ABS_node* exp2 );
ABS_node* ABS_expParented( ABS_node* exp );
ABS_node* ABS_expVar( ABS_node* var );
ABS_node* ABS_expCall( ABS_node* exp1 , ABS_node* exp2 );
ABS_node* ABS_expNew( int type, ABS_node* exp );

// Variables
ABS_node* ABS_varArray( ABS_node* exp1 , ABS_node* exp2 );
ABS_node* ABS_varMono( ABS_node* id );

// Declarations
ABS_node* ABS_declVar( int type , ABS_node* id );
ABS_node* ABS_declFunc( int type , ABS_node* id , ABS_node* param , ABS_node* block );

// Commands
ABS_node* ABS_cmdIf( ABS_node* exp , ABS_node* cmd1 , ABS_node* cmd2 );
ABS_node* ABS_cmdWhile( ABS_node* exp , ABS_node* cmd);
ABS_node* ABS_cmdAttr( ABS_node* var , ABS_node* exp );
ABS_node* ABS_cmdRet( ABS_node* exp );
ABS_node* ABS_cmdExp(ABS_node* exp);
ABS_node* ABS_block( ABS_node* decl , ABS_node* cmd );

// Lists 
ABS_node* ABS_mergeList( ABS_node* list , ABS_node* element );


// Print ABS
void ABS_print( int style );


#endif










