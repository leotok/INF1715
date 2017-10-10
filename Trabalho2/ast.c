#include "absSyntaxTree.h"

#include <stdlib.h>
#include <stdio.h>

void print_nodeLoop( ABS_node* node , int deepness );
static int printType;

// Return the int id of the type
int TYP_getID(TYP_typeEnum type) {
	return type;
}

int TYP_array(int type){
    return type + TYPEQTY;
}


// String Len
int UTIX_stringLen( char* str ) {
	int i = 0;
	
	while( str[i] != '\0' ) {
		i++;
	}
	
	return i;
}

// Check Allocation
void UTIX_checkAlloc( void* flag , const char* auxMsg  , char* auxRef ) {
	if( flag == NULL ) {
		fprintf(stderr, "\n\x1B[91mAllocation Error: %s" , auxMsg );
		
		if( auxRef != NULL ) {
			fprintf(stderr, "%s" , auxRef );
		}

		fprintf(stderr, "\x1B[0m\n" );
		exit(1);
	}
}

// String Copy
char* UTIX_stringCopy( char* str ) {
	int i;
	int sz = UTIX_stringLen( str ) + 1;
	char* new = (char*) malloc( sizeof(char) * sz );
	UTIX_checkAlloc( new , "String Copy Of => " , str );
	
	for( i = 0 ; i < sz ; i++ ) {
		new[i] = str[i];
	}
	
	return new;
}



ABS_node* createNode( int type , int tag ) {
	ABS_node* node;
	
	node = (ABS_node*) malloc( sizeof(ABS_node) );
	if( node == NULL ) {
		fprintf(stderr, "\n\x1B[91mAllocation Error: new node" );
		fprintf(stderr, "\x1B[0m\n" );
		exit(1);
	}
	
	// List Structure
	node->next = NULL;
	node->last = node;
	
	// Types
	node->type = type;
	node->tag = tag;
	
	return node;
}

// ----------------------------------------------------------------------------

ABS_node* ABS_literalInt(int value) {
	ABS_node* newNode;
	
    newNode = createNode( TYPE_EXP , LIT_INT );
        
	newNode->node.exp.data.literal.vInt = value;
	newNode->node.exp.type = INT;
	
	return newNode;
}


ABS_node* ABS_literalFloat(float value) {
	ABS_node* newNode;
	
	newNode = createNode( TYPE_EXP , LIT_FLOAT );
        
	newNode->node.exp.data.literal.vFloat = value;
	newNode->node.exp.type = FLOAT;
	
	return newNode;
}


ABS_node* ABS_literalString( char * str ) {
	ABS_node* newNode;
	
	newNode = createNode( TYPE_EXP , LIT_STRING );
	
	newNode->node.exp.data.literal.vString = UTIX_stringCopy( str );
	// newNode->node.exp.type = array(CHAR); // DEBUG
	
	return newNode;
}


ABS_node* ABS_id( char * name ) {
    ABS_node* newNode;
    
    newNode = createNode( TYPE_ID, ID );
    
	newNode->node.id.name = UTIX_stringCopy( name );
	
	return newNode;
}


ABS_node* ABS_typecast( int type , ABS_node* exp) {
	ABS_node * newNode;
		
    newNode = createNode( TYPE_EXP , CAST );	
		
	newNode->node.exp.type = type;
	newNode->node.exp.data.cast = exp;
	
	return newNode;
}


ABS_node* ABS_expOpr( int opr, ABS_node* exp1, ABS_node* exp2 ) {
	ABS_node* newNode;
	
	if (exp2 == NULL) {
        newNode = createNode( TYPE_EXP , EXP_UNOP );
    }
    else {
        newNode = createNode( TYPE_EXP , EXP_BINOP );
    }
    
    newNode->node.exp.type = -1;
    newNode->node.exp.data.operexp.opr = opr;
    newNode->node.exp.data.operexp.exp1 = exp1;
	newNode->node.exp.data.operexp.exp2 = exp2;

	return newNode;
}


ABS_node* ABS_expParented(ABS_node* exp) {
    ABS_node* newNode;
    
    newNode = createNode( TYPE_EXP , EXP_PAREN);
    
    newNode->node.exp.data.parenexp = exp;
    
    return newNode;
}


ABS_node* ABS_expVar( ABS_node* var ) {
	ABS_node* newNode;

    newNode = createNode( TYPE_EXP , EXP_VAR );
    
	newNode->node.exp.data.varexp = var;
	newNode->node.exp.type = -1;

	return newNode;
}


ABS_node* ABS_expNew( int type, ABS_node* exp ) {
	ABS_node* newNode;
	
	newNode = createNode( TYPE_EXP , EXP_NEW );

	newNode->node.exp.type = type;
	newNode->node.exp.data.newexp.exp = exp;

	return newNode;
}


ABS_node* ABS_expCall( ABS_node* exp1 , ABS_node* exp2 ) {
	ABS_node* newNode;

    newNode = createNode( TYPE_EXP , EXP_CALL );
    
    newNode->node.exp.data.callexp.exp1 = exp1;
    newNode->node.exp.data.callexp.exp2 = exp2;
    
	return newNode;
}


ABS_node* ABS_varArray(ABS_node* exp1, ABS_node* exp2) {
	ABS_node* newNode;

    newNode = createNode( TYPE_VAR , VAR_ARRAY );
    
	newNode->node.var.id = exp1;
	newNode->node.var.index = exp2;

	return newNode;
}


ABS_node* ABS_varMono(ABS_node* id){
	ABS_node* newNode;

    newNode = createNode( TYPE_VAR , VAR_MONO );
    
	newNode->node.var.id = id;
	newNode->node.var.index = NULL;
	
    return newNode;
}


ABS_node* ABS_declVar(int type, ABS_node* id){
	/* SUGAR
		Declarações do tipo: <type> <id1>,<id2>,<id3>...;
		serão transformadas em uma lista de declarações:
		<type> <id1>;
		<type> <id2>;
		<type> <id3>; ...
	*/

	ABS_node* declFirstNode = NULL;
	ABS_node* declLastNode = NULL;
	ABS_node* declCurrentNode;	
		
	ABS_node* idListCurrentNode;
	ABS_node* idListNextNode;
	
	char lastIdFlag = 0;
	
	idListCurrentNode = id;
	while( lastIdFlag == 0 ) {
		if( idListCurrentNode->next == NULL ) {
			lastIdFlag = 1;
		} 
		
		idListNextNode 				= idListCurrentNode->next;
		idListCurrentNode->next 	= NULL;
		idListCurrentNode->last 	= idListCurrentNode;
		
		declCurrentNode = createNode(TYPE_DECL,DEC_VAR);
		declCurrentNode->node.decl.vardecl.type = type;
		declCurrentNode->node.decl.vardecl.id = idListCurrentNode;	
		declCurrentNode->next = NULL;
		declCurrentNode->last = declCurrentNode;
		
		if( declFirstNode == NULL ) {
			declFirstNode = declCurrentNode;
		}
		
		if( declLastNode != NULL ) {
			declLastNode->next = declCurrentNode;
			declLastNode->last = declCurrentNode;
		}
		declLastNode = declCurrentNode;

		idListCurrentNode = idListNextNode;	
	}
	
	// Reparando o ponteiro para o ultimo da cadeia 
	declLastNode = declCurrentNode;
	declCurrentNode = declFirstNode;
	while( declCurrentNode !=  declLastNode ) {
		declCurrentNode->last = declLastNode;
		declCurrentNode = declCurrentNode->next;
	}

    return declFirstNode;
}


ABS_node* ABS_declFunc(int type, ABS_node* id, ABS_node* param, ABS_node* block) {
	ABS_node* newNode;
	
    newNode = createNode( TYPE_DECL , DEC_FUNC );

	newNode->node.decl.funcdecl.type = type;
	newNode->node.decl.funcdecl.id = id;
	newNode->node.decl.funcdecl.param = param;
	newNode->node.decl.funcdecl.block = block;

	return newNode;

}

// Commands -------------------------------------------------

ABS_node* ABS_cmdIf(ABS_node* exp, ABS_node* cmd1, ABS_node* cmd2 ) {
	ABS_node* newNode;

    newNode = createNode( TYPE_CMD , CMD_IF);
    
	newNode->node.cmd.ifcmd.exp = exp;
	newNode->node.cmd.ifcmd.cmd1 = cmd1;
	newNode->node.cmd.ifcmd.cmd2 = cmd2;	

	return newNode;
}


ABS_node* ABS_cmdWhile(ABS_node* exp, ABS_node* cmd) {
	ABS_node* newNode;

    newNode = createNode( TYPE_CMD , CMD_WHILE );
    
	newNode->node.cmd.whilecmd.exp = exp;
	newNode->node.cmd.whilecmd.cmd = cmd;

	return newNode;
}


ABS_node* ABS_cmdAttr( ABS_node* var , ABS_node* exp ) {
	ABS_node* newNode;
	
    newNode = createNode( TYPE_CMD , CMD_ATTR );
    
    newNode->node.cmd.attrcmd.var = var;
	newNode->node.cmd.attrcmd.exp = exp;
	
	return newNode;
}


ABS_node* ABS_cmdRet( ABS_node* exp ) {
	ABS_node* newNode;
    
    newNode = createNode( TYPE_CMD , CMD_RET );
    newNode->node.cmd.retcmd.exp = exp;
	return newNode;
}


ABS_node* ABS_cmdExp(ABS_node* exp) {
	ABS_node* newNode;

    newNode = createNode( TYPE_CMD , CMD_EXP );
    
	newNode->node.cmd.expcmd.exp = exp;

	return newNode;
}


ABS_node* ABS_block(ABS_node* decl, ABS_node* cmd ) {
	ABS_node* newNode;
	
    newNode = createNode( TYPE_CMD , CMD_BLOCK );
    
	newNode->node.cmd.blockcmd.decl = decl;
	newNode->node.cmd.blockcmd.cmd = cmd;
	
	return newNode;
}

// Lists --------------------------------------------------------

ABS_node* ABS_mergeList( ABS_node* list , ABS_node* element ) {
	ABS_node* ret = NULL;
	
	if (list == NULL) {
		ret = element;
	}
	else {
		list->last->next = element;
		list->last = element->last;
		ret = list;
	}
	
	return ret;
}


void newLine(int ident){
    printf("\n");
    int spaces = ident;
    while(spaces--) printf("\t");
}

// Tree Print -----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

void print_ident(int level) {
	int i;
	int color;
	
	if( printType == 1 ) {
	// Print to File
		printf("%5d",level);
	} else {
	// Print to screen
		color = level % 4;
		printf("\x1B[9%dm" , color+1);
	}


	// Identacao
	for( i = 0 ; i < level ; i++ ) {
		printf("..");
	}
	
	// Reseta cor
	if( printType == 2 ) {
		printf("\x1B[0m");
	}
}

void print_nodeType( ABS_node* node ) { 
	switch( node->type ) {
		case TYPE_ID:
			printf("ID");
			return;
		
		case TYPE_EXP:
			printf("EXP");
			return;
			
		case TYPE_DECL:
			printf("DECL");
			return;
			
		case TYPE_VAR:
			printf("VAR");
			return;
			
		case TYPE_CMD:
			printf("CMD");
			return;
			
		default:
			printf("UNKNOWN");
			return;
	}
}

void print_dataType( int type ) {
	int dim = 0;
	int i;

	printf("<");
	
	if ( type < 0 ) {
		printf("Unknown>");
		return;
	}
	else if ( type >= TYPEQTY ) { 
		while( type >= TYPEQTY ) {
			dim++;
			type -= TYPEQTY;
		}
	}

	switch( type ) {
		case CHAR:
			printf("Char");
			break;
		case INT:
			printf("Integer");
			break;
		case FLOAT:
			printf("Float");
			break;
		case VOID:
			printf("Void");
			break;
	}
	
	for( i = 0 ; i < dim ; i++ ) {
		printf("[]");
	}
	
	printf(">");
}

void print_operator( int type , int deepness ) {
	printf("\n");
	print_ident( deepness );
	
	switch( type ) {
		case TK_GE:
			printf(">=");
			break;
			
		case TK_LE:
			printf("<=");
			break;

		case TK_CE:
			printf("==");
			break;	
			
		case TK_NE:
			printf("!=");
			break;	
			
		case TK_AND:
			printf("&&");
			break;	
			
		case TK_OR:
			printf("||");
			break;	
		
		default:
			printf("%c",type);
			break;
	}
}

void print_nodeUnknown( ABS_node* node , int deepness ) {
	printf("[Unknown Node Type]");
}

void print_nodeId( ABS_node* node , int deepness ) {
	printf("%s" , node->node.id.name );
}
 

void print_nodeExp( ABS_node* node , int deepness ) {
	printf("[Exp]");
	//printf("tag: %d" , node->tag );
	deepness++;
	// printf("\n");
	// print_ident( deepness );
	// printf("Type: ");
	// print_dataType( node->node.exp.type );
			
	switch( node->tag ) {
		case EXP_NEW:
			print_dataType( node->node.exp.data.newexp.type );	
			print_nodeLoop( node->node.exp.data.newexp.exp , deepness );	
			break;
			
		case EXP_BINOP:
			print_nodeLoop( node->node.exp.data.operexp.exp1 , deepness );
			print_operator( node->node.exp.data.operexp.opr , deepness );
			print_nodeLoop( node->node.exp.data.operexp.exp2 , deepness );		
			break;
				
		case EXP_CALL:
			printf("\n");
			print_ident( deepness );
			printf("[Call]");
			print_nodeLoop( node->node.exp.data.callexp.exp1 , deepness + 1 );	
			print_nodeLoop( node->node.exp.data.callexp.exp2 , deepness + 1 );	
			break;
			
		case EXP_VAR:
			print_nodeLoop( node->node.exp.data.varexp , deepness );
			break;
			
		case EXP_PAREN:
			print_nodeLoop( node->node.exp.data.parenexp , deepness );	
			break;
			
		case EXP_UNOP:
			print_nodeLoop( node->node.exp.data.operexp.exp1 , deepness );
			print_operator( node->node.exp.data.operexp.opr , deepness );
			break;
			
		case LIT_INT:
			printf("\n");
			print_ident( deepness );
			printf("%d" , node->node.exp.data.literal.vInt );	
			break;
			
		case LIT_FLOAT:
			printf("\n");
			print_ident( deepness );
			printf("%g" , node->node.exp.data.literal.vFloat );	
			break;
			
		case LIT_STRING:
			printf("\n");
			print_ident( deepness );
			printf("str: %s" , node->node.exp.data.literal.vString );	
			break;
			
	}
}

void print_nodeVar( ABS_node* node , int deepness ) {
	printf("[Variable]");
	print_nodeLoop(  node->node.var.id , deepness + 1 );
	//print_nodeLoop(  node->node.var.index , deepness + 1 );
}


void print_nodeCmd( ABS_node* node , int deepness ) {
	switch( node->tag ) {	
		case CMD_IF:
			printf("[If]");
			print_nodeLoop( node->node.cmd.ifcmd.exp , deepness + 1 );
			print_nodeLoop( node->node.cmd.ifcmd.cmd1 , deepness + 1 );
			print_nodeLoop( node->node.cmd.ifcmd.cmd2 , deepness + 1 );			
			break;
			
		case CMD_WHILE:
			printf("[While]");
			print_nodeLoop( node->node.cmd.whilecmd.exp , deepness + 1 );
			print_nodeLoop( node->node.cmd.whilecmd.cmd , deepness + 1 );
			break;
			
		case CMD_ATTR:
			printf("[Attribution]");
			print_nodeLoop( node->node.cmd.attrcmd.var , deepness + 1 );
			print_nodeLoop( node->node.cmd.attrcmd.exp , deepness + 1 );
			break;

		case CMD_EXP:
			printf("[Expression]");
			print_nodeLoop( node->node.cmd.expcmd.exp , deepness + 1 );
			break;
			
		case CMD_BLOCK:
			printf("[Block]");
			print_nodeLoop( node->node.cmd.blockcmd.decl , deepness + 1 );
			print_nodeLoop( node->node.cmd.blockcmd.cmd , deepness + 1 );
			break;
			
		case CMD_RET:
			printf("[Return]");
			print_nodeLoop( node->node.cmd.expcmd.exp , deepness + 1 );
			break;
	}
}

void print_nodeVarDeclaration( ABS_node* node , int deepness ) {
	printf("[Variable Declaration]");
	deepness++;
	printf("\n");
	print_ident( deepness );
	//printf("Type: ");	
	print_dataType( node->node.decl.vardecl.type );	
	print_nodeLoop( node->node.decl.vardecl.id , deepness );	
}

void print_nodeFuncDeclaration( ABS_node* node , int deepness ) {
	printf("[Function Declaration]");
	deepness++;
	printf("\n");
	print_ident( deepness );
	print_dataType( node->node.decl.funcdecl.type );	
	print_nodeLoop( node->node.decl.funcdecl.id , deepness );
	print_nodeLoop( node->node.decl.funcdecl.param , deepness );
	print_nodeLoop( node->node.decl.funcdecl.block, deepness );		
}

void print_nodeDeclaration( ABS_node* node , int deepness ) { 
	switch( node->tag ) {
		case DEC_VAR:
			print_nodeVarDeclaration(  node , deepness );
			break;
		case DEC_FUNC:
			print_nodeFuncDeclaration(  node , deepness );
			break;
	}
}

void print_nodeInfo( ABS_node* node , int deepness ) {
	switch( node->type ) {
		case TYPE_ID:
			print_nodeId( node, deepness );
			return;
		
		case TYPE_EXP:
			print_nodeExp( node, deepness );
			return;
			
		case TYPE_DECL:
			print_nodeDeclaration( node, deepness );
			return;
			
		case TYPE_VAR:
			print_nodeVar( node, deepness );
			return;
			
		case TYPE_CMD:
			print_nodeCmd( node, deepness );
			return;
			
		default:
			print_nodeUnknown( node, deepness );
			return;
	}
}

void print_nodeLoop( ABS_node* node , int deepness ) {
	ABS_node* thisNode = node;
	
	while( thisNode != NULL ) {
		printf("\n");
		print_ident( deepness );
		
		//print_genericNodeInfo( thisNode , deepness );
		print_nodeInfo( thisNode , deepness );
		
		thisNode = thisNode->next;
	}
}



void ABS_print( int style ) {
	int level = 1;
	
	printType = style;
	
	print_nodeLoop( programNode , level );
	
	printf("\n");

}




