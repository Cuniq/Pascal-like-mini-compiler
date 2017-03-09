/**
*
* Thanasis Argyroudis
* AM: 2215
* Phase: Paragwgh telikou kwdika
*
*/

#pragma warning(disable: 4996)
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define dup2 _dup2
#define lseek _lseek
#define fileno _fileno
#else
#include <unistd.h>
#endif


#define STR_EOF         "EOF"
#define NEW_LINE        '\n'
#define BLANK           ' '
#define CHAR_DIVISION   '/'
#define STAR            '*'
#define COLON           ':'
#define END_STRING      '\0'
#define CHAR_LESS       '<'
#define CHAR_EQUALS     '='
#define CHAR_MORE       '>'

#define WORD_ALLOC_SIZE 32
#define NAME_SIZE       64
#define CURRENT_LINE    (( long )__LINE__)

#define MIN_TEMP_VAR_SIZE   3 // At least 3 bytes for 'T', '_' and '/0' and we need more based on the number of digits of the number.

#define NUM_MIN (-32768)
#define NUM_MAX 32767

// ------------------- ERRORS
//General errors
#define FILE_NOT_FOUND                      (-2)
#define TOO_MANY_ARGUMENTS                  (-8)
#define ALLOCATION_ERROR                    (-40)
#define UNDEFINED_VAR                       (-42)
//Lectical errors
#define CHARACTER_INSIDE_NUMERIC_CONST      (-3)
#define NUMBER_OUT_OF_RANGE                 (-4)
#define COMMENT_NOT_CLOSING                 (-5)
#define UNFINISHED_ASSIGN                   (-6)
#define UNKNOWN_CHARACTER                   (-7)
//Syntactical errors
#define INVALID_ID                          (-9)
#define EXPECTED_PROGRAM                    (-10)
#define EXPECTED_OPEN_CURLY_BRACKET         (-11)
#define EXPECTED_CLOSE_CURLY_BRACKET        (-12)
#define EXPECTED_ENDDECLARE                 (-13)
#define EXPECTED_VAR_AFTER_COMMA            (-14)
#define EXPECTED_PROCEDURE_OR_FUNCTION      (-15)
#define EXPECTED_VALID_PARAMATER_TYPE       (-17)
#define EXPECTED_STATEMENT                  (-18)
#define EXPECTED_ASSIGNMENT	                (-19)
#define EXPECTED_PLUS_OR_MINUS              (-20)
#define EXPECTED_MULTI_OR_CHAR_DIVISION     (-22)
#define EXPECTED_FACTOR                     (-23)
#define EXPECTED_CONSTANT                   (-24)
#define EXPECTED_RELATIONAL_OPERATOR        (-25)
#define EXPECTED_OPEN_BRACKET               (-26)
#define EXPECTED_CLOSE_BRACKET              (-27)
#define EXPECTED_OPEN_PARATHENSIS           (-28)
#define EXPECTED_CLOSE_PARATHENSIS          (-29)
#define EXPECTED_WHILE                      (-30)
#define EXPECTED_DO                         (-31)
#define EXPECTED_EXIT                       (-32)
#define EXPECTED_RETURN                     (-33)
#define EXPECTED_PRINT                      (-34)
#define EXPECTED_INCASE                     (-35)
#define EXPECTED_WHEN                       (-36)
#define EXPECTED_FORCASE                    (-37)
#define EXPECTED_CALL                       (-38)
#define NAME_STARTS_WITH_DIGIT              (-39)
#define INVALID_EXIT	                    (-41)

#define AND                 1
#define DECLARE             2
#define DO                  3
#define ELSE                4
#define ENDDECLARE          5
#define EXIT                6
#define PROCEDURE           7
#define FUNCTION            8
#define PRINT               9
#define INCASE              10
#define IF                  11
#define IN                  12
#define INOUT               13
#define NOT                 14
#define FORCASE             15
#define PROGRAM             16
#define OR                  17
#define RETURN              18
#define WHILE               19
#define COPY                20
#define WHEN                21
#define ALNUM               22 // [a-Z][0-9]
#define LESS                23 // <
#define LESSEQ              24 // <=
#define MORE                25 // >
#define MOREEQ              26 // >=
#define EQUALS              27 // =
#define ASSIGN              28 // :=
#define PLUS                29
#define MINUS               30
#define MULTIPLICATION      31
#define SEMICOLON           32
#define COMMA               33
#define OPEN_PARATHENSIS    34 // (
#define CLOSE_PARATHENSIS   35 // )
#define OPEN_CURLY_BRACKET  36 // {
#define CLOSE_CURLY_BRACKET 37 // }
#define OPEN_BRACKET        38 // [
#define CLOSE_BRACKET       39 // ]
#define DIFFERENT           40 // <>
#define CALL                41
#define DIVISION            42

// -------------------------- Structs

typedef struct genquad_s {
	long id;
	char* operation;
	char* x;
	char* y;
	char* z;
} genquad_t;

typedef struct list_node_s {
	genquad_t* genquad;
	struct list_node_s *next;
} node;

#define head_node node** const
#define quadID genquad->id
#define quadOP genquad->operation
#define quadX genquad->x
#define quadY genquad->y
#define quadZ genquad->z

typedef enum argumentMode_e { CV = 100, REF, CP } argMode;
typedef enum argumentType_e { INT = 200 /*, CHAR, DOUBLE,...  */ } argType;
typedef enum entityType_e { VAR = 300, FUNC, CONST, ARG, TEMP } entType;
typedef enum subprogramType { SUB_PROCED = 400, SUB_FUNC } subType;
#define varType argType

typedef struct recordScope_s {
	struct recordEntity_s* entityList;
	long nestingLevel;
	struct recordScope_s* enclosingScope;
	int offset;
} rScope;

typedef struct recordArgument_s {
	argMode parMode;
	argType type;
	struct recordArgument_s* next;
} rArgument;

typedef struct variable_s {
	varType type;
	int offset;
} variable;

typedef struct function_s {
	subType type;
	int startQuad;
	rArgument* head;
	int frameLength;
} function;

typedef struct constant_s {
	char* value;
} constant;

typedef struct argument_s {
	argMode mode;
	int offset;
} argument;

typedef struct temporary_s {
	int offset;
} tempVar;

typedef struct recordEntity_s {
	char* name;
	entType type;
	union {
		variable uVar;
		function uFunc;
		constant uConst;
		argument uArg;
		tempVar uTemp;
	} uType;
	struct recordEntity_s* next;
} rEntity;

// ----------------- Prototypies
void lexicalAnalyst();
void intedifyWord( const char* );
void syntaxError( int );
void lexicalError( int );
void error( int, long );
void checkIfLegalName();
void ungetWord();
int isoperator( int );
int isnumber( char* );
int iscompare( int );
void getInputForFilename();
char* parseFilenameFromInput( const char* );
// -------- Grammar prototypies
void syntacticAnalyst();
void program();
void block( const char* );
// Declarations
void declarations();
void varlist();
// Subprograms
void subprogragms();
void func();
void funcbody( const char* );
void formalpars();
void formalparlist();
void formalparitem();
// Sequence
void sequence( head_node );
void statement( head_node );
void assignment_statement();
void if_statement( head_node );
void while_statement();
void do_while_statement();
void exit_statement();
void incase_statement();
void forcase_statement();
void call_statement();
void return_statement();
void print_statement();
void brack_or_stat( head_node );
void brackets_seq( head_node );
void elsepart( head_node );
void actualpars();
void actualparlist();
void actualparitem();
void condition( head_node, head_node );
void boolterm( head_node, head_node );
void boolfactor( node**, node** );
void expression( char** );
void optional_sign();
void term( char** );
void factor( char** );
void idtails( const char**, char** );
void relational_oper();
void mul_oper();
void add_oper();
// -------- Supporting Subroutines
char* nextquad();
genquad_t* genquad( const char*, const char*, const char*, const char* );
const char* newtemp();
node* emptylist();
node* makelist( genquad_t* );
node* merge( head_node, head_node );
void freeList( head_node );
void backpatch( head_node, char* ); // frees the list
void addList( head_node, genquad_t* ); // adds node at the end of the list
void printList( head_node );
// -------- Symbol table
void push(); // For scope records. Add one scope record at the top of stack
void closeScope(); // Pop()
void addScopeList( rEntity** const ); //Adds record entity at the list of the top scope entity
void addEntityArgument( rArgument** const ); //Adds argument record at the list of [struct function] of the top scope entity (if top entity has not a funcion entity it adds at the previous one)
void printTable(); // Prints the whole symbol table
void updateTopFunctionsStartingQuad(); //This function is called before the beggining of sequence() (inside block()) each function when their first quad is generated.
void symbolTableError( int, char* );
rEntity** searchTable( char*, long* );
rEntity** findLastFuntion();
// -------- Final code
void generateFinalCode( head_node );
void generateREF( char*, int );
void generateCALL( char*, int );
void generateCP();
void generateCPReturn();
void gnlvcode( char* );
void loadvr( char*, char* );
void storerv( char*, char* );
int hasCPParamater();


// ----------------------- Globals
// General Globals
static short countLine = 1; // Decide if the next line counts or not. Because when we ungetWord() we may go back one line 
static char* fileName = NULL;
static FILE* fp = NULL;
static long g_line = 1;
static char* program_name = NULL;
// Lexical Globals
static char* token = NULL;
static int id = 0;
// Intermediate code Globals
static long gm_genquadID = 100;
static node* gm_mainList = NULL;
static short gm_insideLoop = 0; // Determine if we are inside a loop
// Symbol table
static rScope* gst_scope = NULL; // Head of scope stack
static rScope* tos = NULL; // Top of stack
static long gst_nestingLevel = 1;
static int mainFrameLength = 0;
//static int cpAdressRegister = 4; // Starts from 4 to 253
//------------------------

int main( int argc, char** argv ) {
	FILE* outfp = NULL;

	if( argc>2 ) {
		error( TOO_MANY_ARGUMENTS, 0l );
	}
	else if( argc==2 ) {
		fileName = argv[1];

		outfp = fopen( "out.txt", "w" );
		if( ( fp = fopen( fileName, "r" ) )==NULL )
			error( FILE_NOT_FOUND, 0l );

		dup2( fileno( outfp ), STDOUT_FILENO );
		syntacticAnalyst();
	}
	else {

		getInputForFilename();
		outfp = fopen( "out.txt", "w" );
		if( ( fp = fopen( fileName, "r" ) )==NULL )
			error( FILE_NOT_FOUND, 0l );

		dup2( fileno( outfp ), STDOUT_FILENO );
		syntacticAnalyst();
		//printList( &gm_mainList );

	}

	return 0;
}

void generateFinalCode( head_node list ) {
	rEntity* lastFunc = NULL;
	node* traversal = NULL;
	char* functionName = NULL;
	int varNumber = 0;
	long dummy = 1;
	short canReturn = 0; // Define if we call procedure or function. If it is procedure and we try to assign it to a var, error.
	short hasReturn = 0; // A function MUST have a return statement
	short isMainBlock = 0;
	short hasCP = 0; // Determine if that function has a CP argument

	if( *list==NULL ) {
		fprintf( stderr, "Can not generate final code list is empty!\n" );
		return;
	}

	//hasCP = hasCPParamater();

	traversal = *list;

	while( traversal!=NULL ) {

		if( !strcmp( traversal->quadOP, "begin_block" ) ) {
			lastFunc = *findLastFuntion( );
			if( lastFunc->type==FUNC && lastFunc->uType.uFunc.type==SUB_FUNC ) {
				if( ( functionName = malloc( sizeof *functionName * strlen( traversal->quadX ) ) )==NULL )
					error( ALLOCATION_ERROR, CURRENT_LINE );
				strcpy( functionName, traversal->quadX );
				hasReturn = 0; // It is a function must return
			}
			else
				hasReturn = 1; // It is not we don't care
		}

		if( strcmp( traversal->quadOP, "begin_block" )&&strcmp( traversal->quadOP, "end_block" )&&!isMainBlock ) { 	//Do not generate label for begin_block, end_block and main
			printf( "L%ld: ", traversal->quadID );
		}

		if( !strcmp( traversal->quadX, program_name ) ) //Check if we are in main
			isMainBlock = 1;
		else
			isMainBlock = 0;

		if( !strcmp( traversal->quadOP, "jump" ) ) {

			printf( "jmp L%s\n", traversal->quadZ );

		}
		else if( !strcmp( traversal->quadOP, "+" ) || !strcmp( traversal->quadOP, "-" ) || !strcmp( traversal->quadOP, "*" ) || !strcmp( traversal->quadOP, "/" ) ) {

			loadvr( traversal->quadX, "1" );
			loadvr( traversal->quadY, "2" );
			if( !strcmp( traversal->quadOP, "+" ) )
				printf( "addi R[3], R[1], R[2]\n" );
			else if( !strcmp( traversal->quadOP, "-" ) )
				printf( "subi R[3], R[1], R[2]\n" );
			else if( !strcmp( traversal->quadOP, "/" ) )
				printf( "divi R[3], R[1], R[2]\n" );
			else if( !strcmp( traversal->quadOP, "*" ) )
				printf( "muli R[3], R[1], R[2]\n" );
			storerv( "3", traversal->quadZ );

		}
		else if( !strcmp( traversal->quadOP, ":=" ) ) {

			loadvr( traversal->quadX, "1" );
			storerv( "1", traversal->quadZ );

		}
		else if( iscompare( traversal->quadOP[0] ) ) {

			loadvr( traversal->quadX, "1" );
			loadvr( traversal->quadY, "2" );
			printf( "cmpi R[1], R[2]\n" );
			if( strlen( traversal->quadOP )>1 ) {

				if( !strcmp( traversal->quadOP, ">=" ) )
					printf( "jbe L%s\n", traversal->quadZ );
				else if( !strcmp( traversal->quadOP, "<=" ) )
					printf( "jae L%s\n", traversal->quadZ );
				else if( !strcmp( traversal->quadOP, "<>" ) )
					printf( "jne L%s\n", traversal->quadZ );

			}
			else {

				if( !strcmp( traversal->quadOP, ">" ) )
					printf( "jb L%s\n", traversal->quadZ );
				else if( !strcmp( traversal->quadOP, "<" ) )
					printf( "ja L%s\n", traversal->quadZ );
				else if( !strcmp( traversal->quadOP, "=" ) )
					printf( "je L%s\n", traversal->quadZ );

			}

		}
		else if( !strcmp( traversal->quadOP, "out" ) ) {
			loadvr( traversal->quadX, "1" );
			printf( "out R[1]\n" );
		}
		else if( !strcmp( traversal->quadOP, "retv" ) ) {
			hasReturn = 1;
			loadvr( traversal->quadX, "1" );
			printf( "movi R[255], M[R[0]+8]\n" );
			printf( "movi M[R[255]], R[1]\n" );
		}
		else if( !strcmp( traversal->quadOP, "halt" ) ) {
			printf( "halt\n" );
		}
		else if( !strcmp( traversal->quadOP, "par" ) ) {

			while( !strcmp( traversal->quadOP, "par" ) ) {

				if( !strcmp( traversal->quadY, "CV" ) ) {
					loadvr( traversal->quadX, "255" );
					printf( "movi M[R[0]+%d], R[255]\n", tos->offset+varNumber*4+12 );
				}
				else if( !strcmp( traversal->quadY, "REF" ) ) {
					generateREF( traversal->quadX, varNumber );
				}
				else if( !strcmp( traversal->quadY, "CP" ) ) {
					loadvr( traversal->quadX, "255" );
					printf( "movi M[R[0]+%d], R[255]\n", tos->offset+varNumber*4+12 );
					//generateCP( traversal->quadX );
				}
				else { //RET

					printf( "movi R[255], R[0]\n" );
					printf( "movi R[254], %d\n", ( *searchTable( traversal->quadX, &dummy ) )->uType.uTemp.offset );
					printf( "addi R[255], R[255], R[254]\n" );
					printf( "movi M[R[0]+%d], R[255]\n", tos->offset+8 );
					canReturn = 1;
				}

				varNumber++;
				traversal = traversal->next;
			}
			varNumber = 0;
			continue;

		}
		else if( !strcmp( traversal->quadOP, "end_block" ) ) {
			//if( hasCP )
				//generateCPReturn();
			if( !hasReturn ) {
				fprintf( stderr, "Functions must return a value! Function name: %s\n", functionName );
				exit( traversal->quadID );
			}


			if( strcmp( traversal->quadX, program_name ) )
				printf( "jmp M[R[0]]\n" );
		}
		else if( !strcmp( traversal->quadOP, "call" ) ) {
			generateCALL( traversal->quadX, canReturn );
			canReturn = 0;
		}
		else if( !strcmp( traversal->quadOP, "begin_block" ) ) {
			//Nothing
		}
		else {
			fprintf( stderr, "Not implemented instruction: %s\n", traversal->quadOP );
			exit( traversal->quadID );
		}

		traversal = traversal->next;
	}

	freeList( &gm_mainList );

}

void generateREF( char* varName, int varNumber ) {
	rEntity* entity = NULL;
	long nesting = 0;

	entity = *searchTable( varName, &nesting );

	if( entity->type==FUNC ) {
		fprintf( stderr, "Can not load a function\n" );
		exit( entity->type );
	}

	if( nesting==gst_nestingLevel-1 ) {
		if( entity->type==VAR ) {

			printf( "movi R[255], R[0]\n" );
			printf( "movi R[254], %d\n", entity->uType.uVar.offset );
			printf( "addi R[255], R[254], R[255]\n" );
			printf( "movi M[R[0]+%d], R[255]\n", tos->offset+varNumber*4+12 );

		}
		else if( entity->type==ARG ) {

			if( entity->uType.uArg.mode==CV||entity->uType.uArg.mode==CP ) {

				printf( "movi R[255], R[0]\n" );
				printf( "movi R[254], %d\n", entity->uType.uArg.offset );
				printf( "addi R[255], R[254], R[255]\n" );
				printf( "movi M[R[0]+%d], R[255]\n", tos->offset+varNumber*4+12 );

			}
			else if( entity->uType.uArg.mode==REF ) {

				printf( "movi R[255], R[0]\n" );
				printf( "movi R[254], %d\n", entity->uType.uArg.offset );
				printf( "addi R[255], R[254], R[255]\n" );
				printf( "addi R[1], M[R[255]]\n" );
				printf( "movi M[R[0]+%d], R[1]\n", tos->offset+varNumber*4+12 );

			}
		}

	}
	else {

		if( entity->type==VAR||entity->type==ARG&&( entity->uType.uArg.mode==CV||entity->uType.uArg.mode==CP ) ) {

			gnlvcode( varName );
			printf( "movi M[R[0]+%d], R[255]\n", tos->offset+varNumber*4+12 );

		}
		else if( entity->type==ARG && entity->uType.uArg.mode==REF ) {

			gnlvcode( varName );
			printf( "addi R[1], M[R[255]]\n" );
			printf( "movi M[R[0]+%d], R[1]\n", tos->offset+varNumber*4+12 );

		}

	}

}

void generateCALL( char* function, int isReturning ) {
	rEntity* entity = NULL;
	long nesting = 0;

	entity = *searchTable( function, &nesting );

	if( entity->type!=FUNC ) {
		fprintf( stderr, "You can call only function or procedures!\n" );
		exit( entity->type );
	}

	if( isReturning && entity->uType.uFunc.type==SUB_PROCED ) {
		fprintf( stderr, "Procedures can not return value!\n" );
		exit( entity->uType.uFunc.type );
	}

	if( nesting==gst_nestingLevel-2 ) { // brother
		printf( "movi R[255], M[4+R[0]]\n" );
		printf( "movi M[%d+R[0]], R[255]\n", tos->offset+4 );
	}
	else if( nesting==gst_nestingLevel-1 ) { // father
		printf( "movi M[%d+R[0]], R[0]\n", tos->offset+4 );
	}
	else {
		fprintf( stderr, "You can not call that function! Error at line: %ld\n", g_line );
	}

	printf( "movi R[255], %d\n", tos->offset );
	printf( "addi R[0], R[255], R[0]\n" );
	printf( "movi R[255], $\n" );
	printf( "movi R[254], 15\n" );
	printf( "addi R[255], R[255], R[254]\n" );
	printf( "movi M[R[0]], R[255]\n" );
	printf( "jmp L%d\n", entity->uType.uFunc.startQuad+10 );
	printf( "movi R[255], %d\n", tos->offset );
	printf( "subi R[0], R[0], R[255]\n" );

}

void generateCP( char* varName ) {
	rEntity* entity = NULL;
	long nesting = 0;

	entity = *searchTable( varName, &nesting );

	if( entity->type==FUNC ) {
		fprintf( stderr, "Can not load a function\n" );
		exit( entity->type );
	}

	if( nesting==gst_nestingLevel-1 ) {
		if( entity->type==VAR ) {

			printf( "movi R[255], R[0]\n" );
			printf( "movi R[254], %d\n", entity->uType.uVar.offset );
			printf( "addi R[255], R[254], R[255]\n" );
//			printf( "movi M[R[%d]], R[255]\n", cpAdressRegister );

		}
		else if( entity->type==ARG ) {

			if( entity->uType.uArg.mode==CV||entity->uType.uArg.mode==CP ) {

				printf( "movi R[255], R[0]\n" );
				printf( "movi R[254], %d\n", entity->uType.uArg.offset );
				printf( "addi R[255], R[254], R[255]\n" );
	//			printf( "movi M[R[%d]], R[255]\n", cpAdressRegister );

			}
			else if( entity->uType.uArg.mode==REF ) {

				printf( "movi R[255], R[0]\n" );
				printf( "movi R[254], %d\n", entity->uType.uArg.offset );
				printf( "addi R[255], R[254], R[255]\n" );
				printf( "addi R[1], M[R[255]]\n" );
		//		printf( "movi M[R[%d]], R[1]\n", cpAdressRegister );

			}
		}

	}
	else {

		if( entity->type==VAR||entity->type==ARG&&( entity->uType.uArg.mode==CV||entity->uType.uArg.mode==CP ) ) {

			gnlvcode( varName );
			printf( "movi M[R[%d]]], R[255]\n", cpAdressRegister );

		}
		else if( entity->type==ARG && entity->uType.uArg.mode==REF ) {

			gnlvcode( varName );
			printf( "addi R[1], M[R[255]]\n" );
			printf( "movi M[R[%d]], R[1]\n", cpAdressRegister );

		}

	}

//	cpAdressRegister++;
}

void generateCPReturn() {
	rEntity* traversal = tos->entityList;

	while( traversal!=NULL ) {
		if( traversal->type==ARG && traversal->uType.uArg.mode==CP ) {
			loadvr( traversal->name, "255" );
			printf( "movi R[254], M[R[%d]]]\n", cpAdressRegister++ );
			printf( "movi M[R[254]], R[255]\n" );
		}
		traversal = traversal->next;
	}

}

int hasCPParamater() {
	rEntity* traversal = NULL;

	traversal = tos->entityList;
	while( traversal!=NULL ) {

		if( traversal->type==ARG && traversal->uType.uArg.mode==CP ) {
			return 1;
		}

		traversal = traversal->next;
	}
	return 0;
}

void gnlvcode( char* varName ) {
	long nesting = 0;
	int offset = 0;

	offset = ( *searchTable( varName, &nesting ) )->uType.uVar.offset;

	printf( "movi R[255], M[R[0]+4]\n" );
	for( ; nesting<( gst_nestingLevel-2 ); nesting++ )
		printf( "movi R[255], M[R[255]+4]\n" );
	printf( "movi R[254], %d\n", offset );
	printf( "addi R[255], R[255], R[254]\n" );

}

void loadvr( char* varName, char* registerName ) {
	rEntity* entity = NULL;
	long nesting = 0;

	if( isnumber( varName ) ) {
		printf( "movi R[%s], %d\n", registerName, atoi( varName ) );
		return;
	}

	entity = *searchTable( varName, &nesting );

	if( entity->type==FUNC ) {
		fprintf( stderr, "Can not load a function! Error at line: %ld\n", g_line );
		exit( entity->type );
	}

	if( entity->type==CONST ) {

		printf( "movi R[%s], %d\n", registerName, atoi( entity->uType.uConst.value ) );

	}
	else if( entity->type==VAR && nesting==1 ) {

		printf( "movi R[%s], M[%d]\n", registerName, 600+entity->uType.uVar.offset );

	}
	else if( nesting==gst_nestingLevel-1 ) {

		if( entity->type==ARG ) {
			if( entity->uType.uArg.mode==CV ) {
				printf( "movi R[%s], M[R[0]+%d]\n", registerName, entity->uType.uVar.offset );
			}
			else if( entity->uType.uArg.mode==REF ) {
				printf( "movi R[255], M[R[0]+%d]\n", entity->uType.uVar.offset );
				printf( "movi R[%s], M[R[255]]\n", registerName );
			}
			else {// CP
				printf( "movi R[%s], M[R[0]+%d]\n", registerName, entity->uType.uVar.offset );
			}
		}
		else {

			if( entity->type==VAR )
				printf( "movi R[%s], M[R[0]+%d]\n", registerName, entity->uType.uVar.offset );
			else // TEMP
				printf( "movi R[%s], M[R[0]+%d]\n", registerName, entity->uType.uTemp.offset );

		}

	}
	else if( nesting<gst_nestingLevel-1 ) {

		if( entity->type==VAR ) {
			gnlvcode( varName );
			printf( "movi R[%s], M[R[255]]\n", registerName );
		}
		else if( entity->type==ARG ) {
			if( entity->uType.uArg.mode==CV ) {
				gnlvcode( varName );
				printf( "movi R[%s], M[R[255]]\n", registerName );
			}
			else if( entity->uType.uArg.mode==REF ) {
				gnlvcode( varName );
				printf( "movi R[255], M[R[255]]\n" );
				printf( "movi R[%s], M[R[255]]\n", registerName );
			}
			else {// CP
				gnlvcode( varName );
				printf( "movi R[%s], M[R[255]]\n", registerName );
			}
		}

	}
	else { // Huh??

		fprintf( stderr, "LOADVR: Global nesting: %ld, Var nest: %ld, entity->Type: %d, line: %ld\n", gst_nestingLevel-1, nesting, entity->type, g_line );
		exit( entity->type );

	}

}

void storerv( char* registerName, char* varName ) {
	rEntity* entity = NULL;
	long nesting = 0;

	entity = *searchTable( varName, &nesting );

	if( entity->type==FUNC ) {
		fprintf( stderr, "Can not save a function! Error at line: %ld\n", g_line );
		exit( entity->type );
	}

	if( entity->type==VAR && nesting==1 ) {

		printf( "movi M[%d], R[%s]\n", 600+entity->uType.uVar.offset, registerName );

	}
	else if( nesting==gst_nestingLevel-1 ) {

		if( entity->type==ARG ) {
			if( entity->uType.uArg.mode==CV ) {
				printf( "movi M[R[0]+%d], R[%s]\n", entity->uType.uVar.offset, registerName );
			}
			else if( entity->uType.uArg.mode==REF ) {
				printf( "movi R[255], M[R[0]+%d]\n", entity->uType.uVar.offset );
				printf( "movi M[R[255]], R[%s]\n", registerName );
			}
			else {// CP
				printf( "movi M[R[0]+%d], R[%s]\n", entity->uType.uVar.offset, registerName );
			}
		}
		else {

			if( entity->type==VAR )
				printf( "movi M[R[0]+%d], R[%s]\n", entity->uType.uVar.offset, registerName );
			else
				printf( "movi M[R[0]+%d], R[%s]\n", entity->uType.uTemp.offset, registerName );

		}

	}
	else if( nesting<gst_nestingLevel-1 ) {

		if( entity->type==VAR ) {
			gnlvcode( varName );
			printf( "movi M[R[255]], R[%s]\n", registerName );
		}
		else if( entity->type==ARG ) {
			if( entity->uType.uArg.mode==CV ) {
				gnlvcode( varName );
				printf( "movi M[R[255]], R[%s]\n", registerName );
			}
			else if( entity->uType.uArg.mode==REF ) {
				gnlvcode( varName );
				printf( "movi R[255], M[R[255]]\n" );
				printf( "movi M[R[255]], R[%s]\n", registerName );
			}
			else {// CP
				gnlvcode( varName );
				printf( "movi M[R[255]], R[%s]\n", registerName );
			}
		}

	}
	else { // Huh??

		fprintf( stderr, "STORERV: Global nesting: %ld, Var nest: %ld, entity->Type: %d, line: %ld\n", gst_nestingLevel-1, nesting, entity->type, g_line );
		exit( entity->type );

	}
}

void push() {
	rScope** traversal = NULL;
	rScope* newScope = NULL;

	if( gst_scope==NULL ) {
		if( ( gst_scope = malloc( sizeof *gst_scope ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );

		gst_scope->entityList = NULL;
		gst_scope->nestingLevel = gst_nestingLevel++;
		gst_scope->enclosingScope = NULL;
		gst_scope->offset = 12;
		tos = gst_scope;
	}
	else {
		traversal = &gst_scope;
		while( ( *traversal )->enclosingScope!=NULL )
			traversal = &( *traversal )->enclosingScope;

		if( ( newScope = malloc( sizeof *gst_scope ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );

		newScope->entityList = NULL;
		newScope->nestingLevel = gst_nestingLevel++;
		newScope->enclosingScope = NULL;
		newScope->offset = 12;
		tos = newScope;
		( *traversal )->enclosingScope = newScope;

	}
}

void closeScope() {
	rScope** scopeTraversal = NULL;
	rScope** previousScope = NULL;
	rEntity** entityTraversal = NULL;
	rEntity** temp = NULL;
	int framelength = 0;

	printList( &gm_mainList );
	generateFinalCode( &gm_mainList );

	if( gst_scope==NULL ) {
		printf( "Stack is empty!\n" );
		return;
	}

	if( gst_scope->enclosingScope==NULL ) { //Only one line
		entityTraversal = &gst_scope->entityList;
		while( *entityTraversal!=NULL ) {
			free( ( *entityTraversal )->name );
			temp = &( *entityTraversal )->next;
			if( *temp==NULL ) {
				break;
			}
			else
				temp = &( *entityTraversal )->next->next;
			free( *entityTraversal );
			entityTraversal = temp;
		}
		free( tos );
		tos = NULL;
		gst_scope = NULL;
		gst_nestingLevel--;
		return;
	}

	// More than one rows
	scopeTraversal = &gst_scope;
	while( *scopeTraversal!=tos ) {
		previousScope = scopeTraversal;
		scopeTraversal = &( *scopeTraversal )->enclosingScope;
	}

	framelength = tos->offset;

	entityTraversal = &( *scopeTraversal )->entityList;

	while( *entityTraversal!=NULL ) {
		free( ( *entityTraversal )->name );
		temp = &( *entityTraversal )->next;
		if( *temp==NULL ) {
			break;
		}
		else
			temp = &( *entityTraversal )->next->next;
		free( *entityTraversal );
		entityTraversal = temp;
	}

	free( tos );
	tos = *previousScope; //Top row is being destroyed. We need a new top 
	tos->enclosingScope = NULL;
	gst_nestingLevel--;

	//We cleared top row and we need to update the new top row framelength
	entityTraversal = &( *previousScope )->entityList;
	temp = NULL;
	while( *entityTraversal!=NULL ) {
		if( ( *entityTraversal )->type==FUNC )
			temp = entityTraversal;
		entityTraversal = &( *entityTraversal )->next;
	}

	( *temp )->uType.uFunc.frameLength = framelength;

}

void updateTopFunctionsStartingQuad() {
	rEntity** lastFunc = findLastFuntion();

	if( ( *lastFunc )->uType.uFunc.startQuad<0 ) // It may has changed before
		( *lastFunc )->uType.uFunc.startQuad = gm_genquadID-10;

}

rEntity** findLastFuntion() {
	rScope** scopeTraversal = &gst_scope;
	rScope** previousScope = NULL;
	rEntity** entityTraversal = NULL;
	rEntity** lastFunc = NULL;

	while( *scopeTraversal!=tos ) {
		previousScope = scopeTraversal;
		scopeTraversal = &( *scopeTraversal )->enclosingScope;
	}

	if( previousScope!=NULL ) {
		entityTraversal = &( *previousScope )->entityList;
	}
	else {
		entityTraversal = &( *scopeTraversal )->entityList;
	}

	while( *entityTraversal!=NULL ) {
		if( ( *entityTraversal )->type==FUNC )
			lastFunc = entityTraversal;
		entityTraversal = &( *entityTraversal )->next;
	}

	return lastFunc;
}

void addScopeList( rEntity** const entity ) {
	rEntity** traversal = NULL;

	if( ( *entity )->type==VAR ) {
		( *entity )->uType.uVar.offset = tos->offset;
		tos->offset += 4;
	}
	else if( ( *entity )->type==ARG ) {
		( *entity )->uType.uArg.offset = tos->offset;
		tos->offset += 4;
	}
	else if( ( *entity )->type==TEMP ) {
		( *entity )->uType.uTemp.offset = tos->offset;
		tos->offset += 4;
	}
	else if( ( *entity )->type==FUNC ) {
		( *entity )->uType.uFunc.frameLength = -1;
		( *entity )->uType.uFunc.startQuad = -999;

	}

	if( tos->entityList==NULL ) {
		tos->entityList = *entity;
		return;
	}
	traversal = &tos->entityList;

	while( *traversal!=NULL )
		traversal = &( *traversal )->next;

	*traversal = *entity;

}

void addEntityArgument( rArgument** const argument ) {
	rScope** scopeTraversal = &gst_scope;
	rScope** previousScope = NULL;
	rEntity** entityTraversal = NULL;
	rArgument** argTraversal = NULL;

	if( gst_scope==NULL ) {
		printf( "Stack is empty\n" );
		return;
	}

	while( ( *scopeTraversal )->enclosingScope!=NULL ) { //Find two top rows in the symbol table
		previousScope = scopeTraversal;
		scopeTraversal = &( *scopeTraversal )->enclosingScope;
	}
	entityTraversal = &( *scopeTraversal )->entityList;

	while( *entityTraversal!=NULL&&( *entityTraversal )->type!=FUNC )// Search if there are function calls at the top row
		entityTraversal = &( *entityTraversal )->next;

	if( *entityTraversal!=NULL ) { //Found function at the top of stack
		argTraversal = &( *entityTraversal )->uType.uFunc.head;
		if( *argTraversal==NULL )
			*argTraversal = *argument;
		else {
			while( *argTraversal!=NULL )
				argTraversal = &( *argTraversal )->next;
			*argTraversal = *argument;
		}
		return;
	}

	if( *entityTraversal==NULL && *previousScope==NULL ) { // No function calls
		printf( "No function calls are made\n" );
		return;
	}
	//Check at previous level ( Must be one there for sure )
	entityTraversal = &( *previousScope )->entityList;

	while( *entityTraversal!=NULL&&( *entityTraversal )->type!=FUNC )
		entityTraversal = &( *entityTraversal )->next;

	if( *entityTraversal!=NULL ) {
		argTraversal = &( *entityTraversal )->uType.uFunc.head;
		if( *argTraversal==NULL )
			*argTraversal = *argument;
		else {
			while( *argTraversal!=NULL )
				argTraversal = &( *argTraversal )->next;
			*argTraversal = *argument;
		}
	}


}

rEntity** searchTable( char* name, long* nestLevel ) {
	rScope** scopeTraversal = &gst_scope;
	rEntity** entityTraversal = NULL;
	rEntity** lastOccurance = NULL;
	long nesting = 0;

	while( *scopeTraversal!=NULL ) {
		entityTraversal = &( *scopeTraversal )->entityList;

		while( *entityTraversal!=NULL ) {
			if( strcmp( ( *entityTraversal )->name, name )==0 ) {
				lastOccurance = entityTraversal;
				nesting = ( *scopeTraversal )->nestingLevel;
			}
			entityTraversal = &( *entityTraversal )->next;
		}

		scopeTraversal = &( *scopeTraversal )->enclosingScope;
	}

	*nestLevel = nesting;
	if( lastOccurance==NULL )
		symbolTableError( UNDEFINED_VAR, name );

	return lastOccurance;
}

void printTable() {
	rScope* scopeTraversal = gst_scope;
	rEntity* entityTraversal = NULL;
	rArgument* argTraversal = NULL;

	if( scopeTraversal==NULL ) {
		printf( "Symbol table is empty!\n\n\n" );
		return;
	}

	while( scopeTraversal!=NULL ) {
		printf( "(%ld)---Entities List--->", scopeTraversal->nestingLevel );

		entityTraversal = scopeTraversal->entityList;
		while( entityTraversal!=NULL ) {

			switch( entityTraversal->type ) {
				case VAR:
					printf( "[Variable %s: %d]--->", entityTraversal->name, entityTraversal->uType.uVar.offset );
					break;
				case FUNC:
					if( entityTraversal->uType.uFunc.type==SUB_FUNC ) {
						if( entityTraversal->uType.uFunc.frameLength<0 )
							printf( "[Function %s: %d]--", entityTraversal->name, entityTraversal->uType.uFunc.startQuad );
						else
							printf( "[Function %s: %d (fl:%d)]--", entityTraversal->name, entityTraversal->uType.uFunc.startQuad, entityTraversal->uType.uFunc.frameLength );
					}
					else {
						if( entityTraversal->uType.uFunc.frameLength<0 )
							printf( "[Procedure %s: %d]--", entityTraversal->name, entityTraversal->uType.uFunc.startQuad );
						else
							printf( "[Procedure %s: %d (fl:%d)]--", entityTraversal->name, entityTraversal->uType.uFunc.startQuad, entityTraversal->uType.uFunc.frameLength );
					}
					argTraversal = entityTraversal->uType.uFunc.head;
					while( argTraversal!=NULL ) {
						if( argTraversal->parMode==CV )
							printf( "/CV\\--" );
						else if( argTraversal->parMode==REF )
							printf( "/REF\\--" );
						else
							printf( "/CP\\--" );
						argTraversal = argTraversal->next;
					}
					printf( ">" );
					break;
				case CONST:
					printf( "[Constant %s: %s]--->", entityTraversal->name, entityTraversal->uType.uConst.value );
					break;
				case ARG:
					if( entityTraversal->uType.uArg.mode==CV )
						printf( "[CV %s: %d]--->", entityTraversal->name, entityTraversal->uType.uArg.offset );
					else if( entityTraversal->uType.uArg.mode==REF )
						printf( "[REF %s: %d]--->", entityTraversal->name, entityTraversal->uType.uArg.offset );
					else
						printf( "[CP %s: %d]--->", entityTraversal->name, entityTraversal->uType.uArg.offset );
					break;
				case TEMP:
					printf( "[Temp %s: %d]--->", entityTraversal->name, entityTraversal->uType.uTemp.offset );
			}

			entityTraversal = entityTraversal->next;
		}

		printf( "\n" );
		printf( "||\n" ); printf( "||\n" ); printf( "||\n" ); printf( "\\" ); printf( "/\n" );
		scopeTraversal = scopeTraversal->enclosingScope;

	}

	printf( "---------------------------------------------\n\n\n" );

}

void addList( head_node list, genquad_t* quad ) {
	node* temp = NULL;
	node* newNode = NULL;

	if( ( newNode = malloc( sizeof *newNode ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	newNode->genquad = quad;
	newNode->next = NULL;

	if( *list==NULL ) {
		*list = newNode; // Make it head
	}
	else {
		temp = *list;
		while( temp->next!=NULL )
			temp = temp->next;
		temp->next = newNode;
	}

}

char* nextquad() {
	char* longToString = NULL;

	if( ( longToString = malloc( sizeof(long)* sizeof( long ) ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	sprintf( longToString, "%ld", gm_genquadID );
	return longToString; //Needs to be cleared
}

void freeList( head_node list ) {
	node* traversal = NULL;
	node* temp = NULL;

	traversal = *list;

	while( traversal!=NULL ) {
		free( traversal->quadOP );
		free( traversal->quadX );
		free( traversal->quadY );
		free( traversal->quadZ );
		free( traversal->genquad );
		temp = traversal;
		traversal = traversal->next;
		temp->next = NULL;
	}

	gm_mainList = NULL;
}

genquad_t* genquad( const char* op, const char* x, const char* y, const char* z ) {
	genquad_t* nextGenquad = malloc( sizeof *nextGenquad );

	if( nextGenquad==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	if( ( nextGenquad->operation = malloc( sizeof *nextGenquad->operation * strlen( op )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	if( ( nextGenquad->x = malloc( sizeof *nextGenquad->x * strlen( x )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	if( ( nextGenquad->y = malloc( sizeof *nextGenquad->y * strlen( y )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	if( ( nextGenquad->z = malloc( sizeof *nextGenquad->z * strlen( z )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );


	nextGenquad->id = gm_genquadID; gm_genquadID += 10;
	strcpy( nextGenquad->operation, op );
	strcpy( nextGenquad->x, x );
	strcpy( nextGenquad->y, y );
	strcpy( nextGenquad->z, z );

	addList( &gm_mainList, nextGenquad );

	return nextGenquad;
}

const char* newtemp() {
	static long gm_tempVarID = 0;
	char buffer[sizeof(long)* sizeof( long )]; //Long enought buffer to handle the total number of digits of a LONG_MAX. We are using it to convert long to char
	char* temp = NULL;
	int i = 0, n = 0;
	rEntity* tempEntity;

	n = sprintf( buffer, "%ld", gm_tempVarID++ ); // n now has the number of digits of gm_tempVarID

	if( ( temp = malloc( sizeof *temp * ( MIN_TEMP_VAR_SIZE+n ) ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	*temp = 'T';
	*( temp+1 ) = '_';
	for( i = 0; i<n; i++ )
		*( temp+2+i ) = buffer[i];
	*( temp+2+n ) = '\0';

	if( ( tempEntity = malloc( sizeof *tempEntity ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	if( ( tempEntity->name = malloc( strlen( temp )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	strcpy( tempEntity->name, temp );
	tempEntity->next = NULL;
	tempEntity->type = TEMP;
	addScopeList( &tempEntity );

	return temp; // Memory must be freed later.
}

node* emptylist() {
	//just visual
	return NULL;
}

node* makelist( genquad_t* quad ) {
	node* temp = NULL;

	if( ( temp = malloc( sizeof *temp ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	temp->genquad = quad; // Do NOT free this because it will freed from the main list too
	temp->next = NULL;

	return temp;
}

node* merge( head_node firstList, head_node secondList ) {
	node* traversal = NULL;

	if( *firstList==NULL && *secondList==NULL )
		return NULL;
	if( *firstList==NULL && *secondList!=NULL )
		return (node*)*secondList;
	if( *firstList!=NULL && *secondList==NULL )
		return (node*)*firstList;

	traversal = *firstList;

	while( traversal->next!=NULL )
		traversal = traversal->next; // Go at the end of the first list

	traversal->next = *secondList;

	return *firstList;
}

void backpatch( head_node list, char* patch ) {
	node* traversal = NULL;
	node* temp = NULL;

	if( *list==NULL )
		return;

	traversal = *list;

	while( traversal!=NULL ) {
		if( traversal->genquad!=NULL ) {
			if( ( traversal->quadZ = realloc( traversal->quadZ, strlen( patch )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( traversal->quadZ, patch );
		}
		temp = traversal->next;
		free( traversal ); // do NOT free genquad in this list because it will be freed in main list too
		traversal = temp;
	}

	*list = NULL;
}

void printList( head_node list ) {
	node* traversal = NULL;

	if( *list==NULL ) {
		printf( "Empty list!\n" );
		return;
	}
	traversal = *list;

	while( traversal->genquad!=NULL ) {
		printf( "( \"%ld\" \"%s\" \"%s\" \"%s\" \"%s\" ) \n", traversal->quadID, traversal->quadOP, traversal->quadX, traversal->quadY, traversal->quadZ );
		if( ( traversal = traversal->next )==NULL )
			break;
	}
}

void syntacticAnalyst() {
	program();
}

void program() {
	lexicalAnalyst(); // First time must return 'program'
	if( id==PROGRAM ) {

		lexicalAnalyst(); // Next word must be alnum (the id)
		if( id==ALNUM ) {
			checkIfLegalName();

			if( ( program_name = malloc( strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( program_name, token );

			push();
			printf( "L1: jmp L0 \n" );
			block( token );

		}
		else
			syntaxError( INVALID_ID );

	}
	else
		syntaxError( EXPECTED_PROGRAM );

}

void block( const char* name ) {
	char* stackName;
	static long inMainBlock = 0; // MUST be 1 to be in main block. Main block sets it to 1 and must find it 1.

	inMainBlock++; // Every call of block increase it by one and decrease by one at the end. 

	if( ( stackName = malloc( sizeof *stackName * strlen( name )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	strcpy( stackName, name );

	lexicalAnalyst(); // Next word must be {
	if( id==OPEN_CURLY_BRACKET ) {
		declarations();
		subprogragms();

		genquad( "begin_block", stackName, "_", "_" );
		updateTopFunctionsStartingQuad();
		sequence( NULL );

		lexicalAnalyst(); // Next word must be }
		if( id!=CLOSE_CURLY_BRACKET )
			syntaxError( EXPECTED_CLOSE_CURLY_BRACKET );

		if( inMainBlock==1 ) {
			genquad( "halt", "_", "_", "_" );
			mainFrameLength = gst_scope->offset; //Main offset
			printf( "L0: " );
		}


		genquad( "end_block", stackName, "_", "_" );
		closeScope();
		inMainBlock--;
	}
	else
		syntaxError( EXPECTED_OPEN_CURLY_BRACKET );
}

void declarations() {
	lexicalAnalyst(); // Next word may be declare and if not give it back ( because it may be blank )
	if( id==DECLARE ) {
		varlist();

		lexicalAnalyst(); // Next word must be enddeclare

		if( id!=ENDDECLARE )
			syntaxError( EXPECTED_ENDDECLARE );
	}
	else
		ungetWord();
}

void varlist() {
	rEntity* varEntity = NULL;
	lexicalAnalyst(); // Next word must be a legal var name and if not give it back
	if( id==ALNUM ) {
		checkIfLegalName();

		if( ( varEntity = malloc( sizeof *varEntity ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		if( ( varEntity->name = malloc( strlen( token )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( varEntity->name, token );
		varEntity->type = VAR;
		varEntity->next = NULL;
		varEntity->uType.uVar.type = INT;
		addScopeList( &varEntity );

		lexicalAnalyst(); // Next word may be ',' and if not give it back
		while( id==COMMA ) {
			lexicalAnalyst(); // Next word must be a legal var name
			if( id==ALNUM ) {
				checkIfLegalName();

				if( ( varEntity = malloc( sizeof *varEntity ) )==NULL )
					error( ALLOCATION_ERROR, CURRENT_LINE );
				if( ( varEntity->name = malloc( strlen( token )+1 ) )==NULL )
					error( ALLOCATION_ERROR, CURRENT_LINE );
				strcpy( varEntity->name, token );
				varEntity->type = VAR;
				varEntity->next = NULL;
				varEntity->uType.uVar.type = INT;
				addScopeList( &varEntity );

			}
			else
				syntaxError( EXPECTED_VAR_AFTER_COMMA );
			lexicalAnalyst(); // Next word may be ',' and if not give it back
		}
		ungetWord();
	}
	else
		ungetWord();
}

void subprogragms() {
	lexicalAnalyst(); // Just want to see if func starts. If not give it back
	if( id==PROCEDURE||id==FUNCTION ) {
		ungetWord(); // Give word back and let func do its job. We just checking here.
		func();
		subprogragms(); // See if there are more funcs
	}
	else
		ungetWord();
}

void func() {
	subType funcType = SUB_PROCED;
	rEntity* subEntity = NULL;

	lexicalAnalyst(); // Next word must be procedure or function
	if( id==PROCEDURE||id==FUNCTION ) {
		if( id==FUNCTION )
			funcType = SUB_FUNC;

		lexicalAnalyst(); // Next word must be valid name
		if( id==ALNUM ) {
			checkIfLegalName();

			if( ( subEntity = malloc( sizeof *subEntity ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			if( ( subEntity->name = malloc( strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( subEntity->name, token );
			subEntity->type = FUNC;
			subEntity->next = NULL;
			subEntity->uType.uFunc.type = funcType;
			subEntity->uType.uFunc.head = NULL;
			addScopeList( &subEntity );

			funcbody( token );
		}
		else
			syntaxError( INVALID_ID );
	}
	else
		syntaxError( EXPECTED_PROCEDURE_OR_FUNCTION );
}

void funcbody( const char* name ) {
	char* stackName;
	if( ( stackName = malloc( sizeof *stackName * strlen( name )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	strcpy( stackName, name );

	push(); // We are going inside a function 
	formalpars();
	block( stackName );
}

void formalpars() {
	lexicalAnalyst(); // Next word must be ( 
	if( id==OPEN_PARATHENSIS ) {
		lexicalAnalyst(); // Next word must be ) or parameter
		if( id==CLOSE_PARATHENSIS ) {
			return;
		}
		else {
			ungetWord(); // We just checking here
			formalparlist();
			lexicalAnalyst(); // Next word must be ) 
			if( id!=CLOSE_PARATHENSIS )
				syntaxError( EXPECTED_CLOSE_PARATHENSIS );
		}

	}
	else
		syntaxError( EXPECTED_OPEN_PARATHENSIS );
}

void formalparlist() {
	formalparitem();
	lexicalAnalyst(); // Next word must be ',' or give it back
	if( id==COMMA )
		formalparlist();
	else
		ungetWord();
}

void formalparitem() {
	argMode mode = CV;
	rArgument* arg = NULL;
	rEntity* varEntity = NULL;

	lexicalAnalyst(); // Next word must be in inout or copy or give back
	if( id==IN||id==INOUT||id==COPY ) {
		if( id==INOUT )
			mode = REF;
		else if( id==COPY )
			mode = CP;

		if( ( arg = malloc( sizeof *arg ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		arg->parMode = mode;
		arg->type = INT;
		arg->next = NULL;
		addEntityArgument( &arg ); //Arguments will be added at previous level

		lexicalAnalyst(); // Next word must be a valid id
		if( id==ALNUM ) {
			checkIfLegalName();

			if( ( varEntity = malloc( sizeof *varEntity ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			if( ( varEntity->name = malloc( strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( varEntity->name, token );
			varEntity->type = ARG;
			varEntity->next = NULL;
			varEntity->uType.uArg.mode = mode;
			addScopeList( &varEntity ); // We created a new scope before

		}
		else
			syntaxError( INVALID_ID );
	}
	else
		syntaxError( EXPECTED_VALID_PARAMATER_TYPE );
}

void sequence( head_node exitList ) {
	statement( exitList );
	lexicalAnalyst(); // Must be ';' or give it back
	while( id==SEMICOLON ) {
		statement( exitList );
		lexicalAnalyst(); // Must be ';' or give it back
	}

	ungetWord();
}

void statement( head_node exitList ) {
	lexicalAnalyst(); // Check which statement is and give it back so the equivalent function do its job
	ungetWord(); //we just checking here give it back ( see inside ungetWord NOTE )

	if( id==ALNUM ) { // Assignment
		assignment_statement();
	}
	else if( id==IF ) {
		if_statement( exitList );
	}
	else if( id==WHILE ) {
		while_statement();
	}
	else if( id==DO ) {
		do_while_statement();
	}
	else if( id==EXIT ) {
		exit_statement( exitList );
	}
	else if( id==INCASE ) {
		incase_statement();
	}
	else if( id==FORCASE ) {
		forcase_statement();
	}
	else if( id==CALL ) {
		call_statement();
	}
	else if( id==RETURN ) {
		return_statement();
	}
	else if( id==PRINT ) {
		print_statement();
	}
	else {
		//Blank statement
	}
}

void assignment_statement() {
	char* exp = NULL;
	char* idName = NULL;


	lexicalAnalyst(); // Must be a var 
	if( id==ALNUM ) {
		checkIfLegalName();

		if( ( idName = malloc( sizeof *idName * strlen( token )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( idName, token );

		lexicalAnalyst(); // Next word must be ':='
		if( id==ASSIGN ) {
			expression( &exp );
			genquad( ":=", exp, "_", idName );
		}
		else
			syntaxError( EXPECTED_ASSIGNMENT );
	}
	else
		syntaxError( INVALID_ID );

	free( exp );
	free( idName );
}

void if_statement( head_node exitList ) {
	node* Btrue = emptylist();
	node* Bfalse = emptylist();
	node* ifList = emptylist();
	char* patch = NULL; // To keep malloced memory

	lexicalAnalyst(); // Must be 'if'
	if( id==IF ) {
		lexicalAnalyst(); // Must be '('
		if( id!=OPEN_PARATHENSIS )
			syntaxError( EXPECTED_OPEN_PARATHENSIS );

		condition( &Btrue, &Bfalse );

		lexicalAnalyst(); // Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );
	}

	backpatch( &Btrue, ( patch = nextquad() ) );
	free( patch );

	brack_or_stat( exitList );
	ifList = makelist( genquad( "jump", "_", "_", "_" ) );
	backpatch( &Bfalse, ( patch = nextquad() ) );
	free( patch );

	elsepart( exitList );
	backpatch( &ifList, ( patch = nextquad() ) );
	free( patch );

}

void brack_or_stat( head_node exitList ) {
	lexicalAnalyst(); // Must be '{' or valid statement
	ungetWord(); // We just checking here
	if( id==OPEN_CURLY_BRACKET )
		brackets_seq( exitList );
	else // statement
		statement( exitList );
}

void brackets_seq( head_node exitList ) {
	lexicalAnalyst(); // Must be '{'
	if( id==OPEN_CURLY_BRACKET ) {

		sequence( exitList );

		lexicalAnalyst(); // Must be '}'
		if( id!=CLOSE_CURLY_BRACKET )
			syntaxError( EXPECTED_CLOSE_CURLY_BRACKET );
	}
	else
		syntaxError( EXPECTED_OPEN_CURLY_BRACKET );
}

void elsepart( head_node exitList ) {
	lexicalAnalyst(); // Must be 'else' or give it back
	if( id==ELSE )
		brack_or_stat( exitList );
	else
		ungetWord();
}

void while_statement() {
	node* exitList = emptylist();
	node* Btrue = emptylist();
	node* Bfalse = emptylist();
	char* jumpBack = NULL;
	char* patch = NULL;
	gm_insideLoop = 1;

	lexicalAnalyst();// Must be 'while'
	if( id==WHILE ) {
		lexicalAnalyst();//Must be '('
		if( id!=OPEN_PARATHENSIS )
			syntaxError( EXPECTED_OPEN_PARATHENSIS );

		jumpBack = nextquad();
		condition( &Btrue, &Bfalse );

		lexicalAnalyst();// Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );

		backpatch( &Btrue, ( patch = nextquad() ) );
		free( patch );

		brack_or_stat( &exitList );

		genquad( "jump", "_", "_", jumpBack );
		backpatch( &Bfalse, ( patch = nextquad() ) );
		backpatch( &exitList, patch );

		free( jumpBack );
		free( patch );
	}
	else
		syntaxError( EXPECTED_WHILE );

	gm_insideLoop = 0;
}

void do_while_statement() {
	char* jumpTrue = NULL;
	char* patch = NULL;
	node* Btrue = emptylist();
	node* Bfalse = emptylist();
	node* exitList = emptylist();
	gm_insideLoop = 1;

	lexicalAnalyst();// Must be 'do'
	if( id==DO ) {

		jumpTrue = nextquad();
		brack_or_stat( &exitList );

		lexicalAnalyst(); // Must be 'while'
		if( id!=WHILE )
			syntaxError( EXPECTED_WHILE );

		lexicalAnalyst(); // Must be '('
		if( id!=OPEN_PARATHENSIS )
			syntaxError( EXPECTED_OPEN_PARATHENSIS );

		condition( &Btrue, &Bfalse );
		backpatch( &Btrue, jumpTrue );
		backpatch( &Bfalse, ( patch = nextquad() ) );
		backpatch( &exitList, patch );

		free( patch );
		free( jumpTrue );

		lexicalAnalyst(); // Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );

	}
	else
		syntaxError( EXPECTED_DO );

	gm_insideLoop = 0;
}

void exit_statement( head_node exitList ) {
	node* temp = NULL;
	lexicalAnalyst(); // Must be 'exit'
	if( id==EXIT ) {
		if( gm_insideLoop!=1 )
			syntaxError( INVALID_EXIT );
		temp = makelist( genquad( "jump", "_", "_", "_" ) );
		*exitList = merge( exitList, &temp );
	}
	else
		syntaxError( EXPECTED_EXIT );
}

void incase_statement() {
	node* Btrue = emptylist();
	node* Bfalse = emptylist();
	char* incaseStart = NULL;
	const char* flag = NULL;
	char* patch = NULL;

	lexicalAnalyst(); //Must be 'incase'
	if( id==INCASE ) {
		incaseStart = nextquad();
		genquad( ":=", "0", "_", ( flag = newtemp() ) );

		lexicalAnalyst(); // Must be '{'
		if( id!=OPEN_CURLY_BRACKET )
			syntaxError( EXPECTED_OPEN_CURLY_BRACKET );

		lexicalAnalyst(); //Must be 'when' or give it back
		while( id==WHEN ) {
			lexicalAnalyst(); //Must be '('
			if( id!=OPEN_PARATHENSIS )
				syntaxError( EXPECTED_OPEN_PARATHENSIS );
			condition( &Btrue, &Bfalse );
			lexicalAnalyst(); //Must be ')'
			if( id!=CLOSE_PARATHENSIS )
				syntaxError( EXPECTED_CLOSE_PARATHENSIS );

			backpatch( &Btrue, ( patch = nextquad() ) );
			free( patch );
			brack_or_stat( NULL );
			genquad( ":=", "1", "_", flag );
			backpatch( &Bfalse, ( patch = nextquad() ) );
			free( patch );

			lexicalAnalyst(); //Must be 'when' or give it back
		}
		ungetWord();

		genquad( "=", flag, "1", incaseStart );

		lexicalAnalyst();// Must be '}'
		if( id!=CLOSE_CURLY_BRACKET )
			syntaxError( EXPECTED_CLOSE_CURLY_BRACKET );
	}
	else
		syntaxError( EXPECTED_INCASE );

	free( (void*)flag );
	free( incaseStart );
}

void forcase_statement() {
	node* Btrue = emptylist();
	node* Bfalse = emptylist();
	char* forcaseStart = NULL;
	char* patch = NULL;

	lexicalAnalyst(); //Must be 'forcase'
	if( id==FORCASE ) {
		forcaseStart = nextquad();
		lexicalAnalyst(); // Must be '{'
		if( id!=OPEN_CURLY_BRACKET )
			syntaxError( EXPECTED_OPEN_CURLY_BRACKET );

		lexicalAnalyst(); //Must be 'when' or give it back
		while( id==WHEN ) {
			lexicalAnalyst(); //Must be '('
			if( id!=OPEN_PARATHENSIS )
				syntaxError( EXPECTED_OPEN_PARATHENSIS );
			condition( &Btrue, &Bfalse );
			lexicalAnalyst(); //Must be ')'
			if( id!=CLOSE_PARATHENSIS )
				syntaxError( EXPECTED_CLOSE_PARATHENSIS );

			backpatch( &Btrue, ( patch = nextquad() ) );
			free( patch );
			brack_or_stat( NULL );
			genquad( "jump", "_", "_", forcaseStart );
			backpatch( &Bfalse, ( patch = nextquad() ) );
			free( patch );


			lexicalAnalyst(); //Must be 'when' or give it back
		}
		ungetWord();

		lexicalAnalyst();// Must be '}'
		if( id!=CLOSE_CURLY_BRACKET )
			syntaxError( EXPECTED_CLOSE_CURLY_BRACKET );
	}
	else
		syntaxError( EXPECTED_FORCASE );

	free( forcaseStart );
}

void call_statement() {
	char* callName = NULL;

	lexicalAnalyst(); //Must be 'call'
	if( id==CALL ) {
		lexicalAnalyst(); //Must be a valid id
		if( id==ALNUM ) {
			checkIfLegalName();
			if( ( callName = malloc( sizeof *callName * strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( callName, token );
		}
		else
			syntaxError( INVALID_ID );

		actualpars();
		genquad( "call", callName, "_", "_" );

	}
	else
		syntaxError( EXPECTED_CALL );

	free( callName );
}

void return_statement() {
	char* exp = NULL;

	lexicalAnalyst(); // Must be 'return'
	if( id==RETURN ) {
		lexicalAnalyst(); // Must be '('
		if( id!=OPEN_PARATHENSIS )
			syntaxError( EXPECTED_OPEN_PARATHENSIS );

		expression( &exp );
		genquad( "retv", exp, "_", "_" );

		lexicalAnalyst(); // Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );
	}
	else
		syntaxError( EXPECTED_RETURN );

	free( exp );
}

void print_statement() {
	char* exp = NULL;

	lexicalAnalyst(); // Must be 'print'
	if( id==PRINT ) {
		lexicalAnalyst(); // Must be '('
		if( id!=OPEN_PARATHENSIS )
			syntaxError( EXPECTED_OPEN_PARATHENSIS );

		expression( &exp );
		genquad( "out", exp, "_", "_" );
		free( exp );

		lexicalAnalyst();// Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );
	}
	else
		syntaxError( EXPECTED_PRINT );
}

void actualpars() {
	lexicalAnalyst(); // Must be ( 
	if( id==OPEN_PARATHENSIS ) {

		lexicalAnalyst(); // Must be ) or give it back
		if( id==CLOSE_PARATHENSIS )
			return;
		else
			ungetWord();

		actualparlist();
		lexicalAnalyst(); // Must be )
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );
	}
	else
		syntaxError( EXPECTED_OPEN_PARATHENSIS );
}

void actualparlist() {
	// First go through of all parameters and then generate quads
	char** xQuads = NULL;
	char** yQuads = NULL;
	char* xquad = NULL;
	char* yquad = NULL;
	int i = 1, j = 0;

	actualparitem( &xquad, &yquad );

	if( ( xQuads = malloc( sizeof *xQuads * i ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	if( ( yQuads = malloc( sizeof *yQuads * i ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	if( ( xQuads[0] = malloc( sizeof *xQuads[0]*strlen( xquad )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	if( ( yQuads[0] = malloc( sizeof *yQuads[0]*strlen( yquad )+1 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );
	strcpy( xQuads[0], xquad );
	strcpy( yQuads[0], yquad );
	free( xquad );
	free( yquad );

	lexicalAnalyst(); // Must be ',' or give it back
	while( id==COMMA ) {
		actualparitem( &xquad, &yquad );

		i++;
		if( ( xQuads = realloc( xQuads, sizeof *xQuads * i ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		if( ( yQuads = realloc( yQuads, sizeof *yQuads * i ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		if( ( xQuads[i-1] = malloc( sizeof *xQuads[i-1]*strlen( xquad )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		if( ( yQuads[i-1] = malloc( sizeof *yQuads[i-1]*strlen( yquad )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( xQuads[i-1], xquad );
		strcpy( yQuads[i-1], yquad );
		free( xquad );
		free( yquad );

		lexicalAnalyst(); // Must be ',' or give it back
	}

	for( ; j < i; j++ ) {
		genquad( "par", xQuads[j], yQuads[j], "_" );
	}

	for( ; i > 0; i-- ) {
		free( xQuads[i-1] );
		free( yQuads[i-1] );
	}
	free( xQuads );
	free( yQuads );

	ungetWord();
}

void actualparitem( char** xquad, char** yquad ) {
	char* exp = NULL;
	if( ( *yquad = malloc( sizeof **yquad*4 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	lexicalAnalyst(); // Must be 'in' or 'inout' or 'copy' 
	if( id==IN ) {

		expression( &exp );

		if( ( *xquad = malloc( sizeof **xquad * strlen( exp )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );

		strcpy( *xquad, exp );
		strcpy( *yquad, "CV" );
		free( exp );

	}
	else if( id==INOUT ) {
		lexicalAnalyst(); // Must be a legal ID
		if( id==ALNUM ) {
			checkIfLegalName();

			if( ( *xquad = malloc( sizeof **xquad * strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );

			strcpy( *xquad, token );
			strcpy( *yquad, "REF" );
		}
	}
	else if( id==COPY ) {
		lexicalAnalyst(); // Must be a legal ID
		if( id==ALNUM ) {
			checkIfLegalName();

			if( ( *xquad = malloc( sizeof **xquad * strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );

			strcpy( *xquad, token );
			strcpy( *yquad, "CP" );
		}
	}
	else
		syntaxError( EXPECTED_VALID_PARAMATER_TYPE );

}

void condition( head_node Btrue, head_node Bfalse ) {
	node* Qtrue = emptylist();
	node* Qfalse = emptylist();
	char* patchLine = NULL;

	boolterm( &Qtrue, &Qfalse );
	*Btrue = Qtrue;
	*Bfalse = Qfalse;

	lexicalAnalyst();// must word must be 'or' if give it back
	while( id==OR ) {
		patchLine = nextquad();
		backpatch( Bfalse, patchLine );
		free( patchLine );
		condition( &Qtrue, &Qfalse );
		*Btrue = merge( Btrue, &Qtrue );
		*Bfalse = Qfalse;
		lexicalAnalyst();// must word must be 'or' if give it back
	}

	ungetWord();
}

void boolterm( head_node Qtrue, head_node Qfalse ) {
	node* Rtrue = emptylist();
	node* Rfalse = emptylist();

	boolfactor( &Rtrue, &Rfalse );
	*Qtrue = Rtrue;
	*Qfalse = Rfalse;

	lexicalAnalyst();// must word must be 'and' if give it back
	while( id==AND ) {
		backpatch( Qtrue, nextquad() );
		boolfactor( &Rtrue, &Rfalse );
		*Qfalse = merge( Qfalse, &Rfalse );
		*Qtrue = Rtrue;
		lexicalAnalyst();// must word must be 'and' if give it back
	}

	ungetWord();
}

void boolfactor( node** Rtrue, node** Rfalse ) {
	node* swapNode = NULL;
	char* exp1 = NULL;
	char* exp2 = NULL;
	char* relop = NULL;

	lexicalAnalyst(); // Must be 'not' or '[' or valid expression
	if( id==NOT ) { // swap true and false
		lexicalAnalyst(); // Must be '['

		if( id!=OPEN_BRACKET )
			syntaxError( EXPECTED_OPEN_BRACKET );

		condition( Rtrue, Rfalse );
		swapNode = *Rtrue;
		*Rtrue = *Rfalse;
		*Rfalse = swapNode;

		lexicalAnalyst(); // Must be ']'
		if( id!=CLOSE_BRACKET )
			syntaxError( EXPECTED_CLOSE_BRACKET );
	}
	else if( id==OPEN_BRACKET ) {
		condition( Rtrue, Rfalse );

		lexicalAnalyst(); // Must be ']'
		if( id!=CLOSE_BRACKET )
			syntaxError( EXPECTED_CLOSE_BRACKET );
	}
	else { // expression 
		ungetWord();

		expression( &exp1 );
		relational_oper(); // Token is relop now
		if( ( relop = malloc( sizeof *relop*3 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( relop, token );
		expression( &exp2 );

		*Rtrue = makelist( genquad( relop, exp1, exp2, "_" ) );
		*Rfalse = makelist( genquad( "jump", "_", "_", "_" ) );
	}

	free( relop );
	free( exp1 );
	free( exp2 );
}

void expression( char** returnVal ) {
	char* op = NULL;
	char* firstTerm = NULL;
	char* secondTerm = NULL;
	const char* temp = NULL;

	if( ( op = malloc( sizeof *op*2 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	optional_sign(); //If token is now '-' we keep it ( we don't care about + )
	if( id==MINUS ) {
		if( ( firstTerm = malloc( sizeof *firstTerm*2 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( firstTerm, token );
	}

	term( &firstTerm );

	lexicalAnalyst(); // Must be '+' or '-' else give it back
	while( id==PLUS||id==MINUS ) {
		ungetWord(); // We just checking here give it back and let add_oper do its job
		add_oper();
		strcpy( op, token );

		temp = newtemp();
		term( &secondTerm );
		genquad( op, firstTerm, secondTerm, temp );

		strcpy( firstTerm, temp );

		free( (void*)temp );
		free( secondTerm );
		secondTerm = NULL;

		lexicalAnalyst(); // Must be '+' or '-' else give it back
	}
	if( *returnVal==NULL ) {
		if( ( *returnVal = malloc( sizeof **returnVal*strlen( firstTerm )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
	}
	else { //It may has '-' at the beggining so we need to keep it (realloc)
		if( ( *returnVal = realloc( *returnVal, sizeof **returnVal*strlen( firstTerm )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
	}
	strcpy( *returnVal, firstTerm );

	ungetWord();
	free( op );
	//free( firstTerm );
}

void term( char** term ) {
	char* op = NULL;
	char* firstTerm = NULL;
	char* secondTerm = NULL;
	const char* temp = NULL;

	if( ( op = malloc( sizeof *op*2 ) )==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	factor( &firstTerm ); // get first term from factor

	lexicalAnalyst(); // Must be '*' or '/' or give it back
	while( id==MULTIPLICATION||id==DIVISION ) {
		ungetWord(); //We just checking let mul_oper do its job
		mul_oper();
		strcpy( op, token ); // Token now has the operator

		temp = newtemp();
		factor( &secondTerm ); //get second term
		genquad( op, firstTerm, secondTerm, temp );

		strcpy( firstTerm, temp );
		free( (void*)temp );

		lexicalAnalyst(); // Must be '*' or '/' or give it back
	}

	if( *term==NULL ) {
		if( ( *term = malloc( sizeof **term*strlen( firstTerm )+1 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcpy( *term, firstTerm );
	}
	else { //It has malloced for the '-'
		if( ( *term = realloc( *term, sizeof **term*strlen( firstTerm )+2 ) )==NULL )
			error( ALLOCATION_ERROR, CURRENT_LINE );
		strcat( *term, firstTerm ); // '-' is that first position
	}



	ungetWord();
	free( op );
	//free( firstTerm );
	free( secondTerm );

}

void factor( char** term ) {
	const char* retVal = NULL;

	lexicalAnalyst(); // Must be const or '(' or ID
	if( id==ALNUM ) {
		if( isalpha( token[0] ) ) { // Must be valid ID

			if( ( *term = realloc( *term, sizeof **term * strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( *term, token ); // Keep current id

			idtails( &retVal, term ); // If it is a function term is function's ID

			if( retVal!=NULL ) { // It was a function call we keep the return val
				if( ( *term = realloc( *term, sizeof **term * strlen( retVal )+1 ) )==NULL )
					error( ALLOCATION_ERROR, CURRENT_LINE );
				strcpy( *term, retVal );
			}

		}
		else { // then must be constant
			if( ( *term = realloc( *term, sizeof **term * strlen( token )+1 ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );
			strcpy( *term, token );
		}
	}
	else if( id==OPEN_PARATHENSIS ) {
		expression( term );
		lexicalAnalyst(); // Must be ')'
		if( id!=CLOSE_PARATHENSIS )
			syntaxError( EXPECTED_CLOSE_PARATHENSIS );
	}
	else
		syntaxError( EXPECTED_FACTOR );

	free( (void*)retVal );
}

void optional_sign() {
	lexicalAnalyst();// Next word must be '+' or '-' else give it back
	if( id==PLUS||id==MINUS ) {
		ungetWord(); //We just checking give it back and let add_oper do its job
		add_oper();
	}
	else
		ungetWord();
}

void idtails( const char** retVal, char** funcID ) {
	lexicalAnalyst(); // Must be ( or give it back
	ungetWord(); // We just checking here
	if( id==OPEN_PARATHENSIS ) {
		actualpars();
		*retVal = newtemp();
		genquad( "par", *retVal, "ret", "_" );
		genquad( "call", *funcID, "_", "_" );
	}
}

void relational_oper() {
	lexicalAnalyst();
	if( id!=EQUALS && id!=LESS && id!=MORE && id!=LESSEQ && id!=MOREEQ && id!=DIFFERENT )
		syntaxError( EXPECTED_RELATIONAL_OPERATOR );

}

void mul_oper() {
	lexicalAnalyst();// Must be '*' or '/'
	if( id!=MULTIPLICATION && id!=DIVISION )
		syntaxError( EXPECTED_MULTI_OR_CHAR_DIVISION );
}

void add_oper() {
	lexicalAnalyst(); // Next word must be '+' or '-'
	if( id!=MINUS && id!=PLUS )
		syntaxError( EXPECTED_PLUS_OR_MINUS );

}

void syntaxError( int errcode ) {
	fprintf( stderr, " -------- Error at token \"%s\" -------- \n ", token );

	if( errcode==EXPECTED_PROGRAM ) {
		fprintf( stderr, "Expected program, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==INVALID_ID ) {
		fprintf( stderr, "Invalid ID, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_OPEN_CURLY_BRACKET ) {
		fprintf( stderr, "Expected opening curly bracket, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_CLOSE_CURLY_BRACKET ) {
		fprintf( stderr, "Expected closing curly bracket, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_ENDDECLARE ) {
		fprintf( stderr, "Expected enddeclare, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_VAR_AFTER_COMMA ) {
		fprintf( stderr, "Expected a valid name of var, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_PROCEDURE_OR_FUNCTION ) {
		fprintf( stderr, "Expected a function declaration, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_VALID_PARAMATER_TYPE ) {
		fprintf( stderr, "Expected a valid paramater type, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_STATEMENT ) {
		fprintf( stderr, "Expected statement, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_ASSIGNMENT ) {
		fprintf( stderr, "Expected assignment, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_PLUS_OR_MINUS ) {
		fprintf( stderr, "Expected '+' or '-', error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_MULTI_OR_CHAR_DIVISION ) {
		fprintf( stderr, "Expected '*' or '/', error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_FACTOR ) {
		fprintf( stderr, "Expected factor, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_CONSTANT ) {
		fprintf( stderr, "Expected constant, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_RELATIONAL_OPERATOR ) {
		fprintf( stderr, "Expected relational operator, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_OPEN_BRACKET ) {
		fprintf( stderr, "Expected a bracket to open, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_CLOSE_BRACKET ) {
		fprintf( stderr, "Expected a bracket to close, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_OPEN_PARATHENSIS ) {
		fprintf( stderr, "Expected a parathensis to open, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_CLOSE_PARATHENSIS ) {
		fprintf( stderr, "Expected a parathensis to close, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_WHILE ) {
		fprintf( stderr, "Expected while, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_DO ) {
		fprintf( stderr, "Expected do, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_EXIT ) {
		fprintf( stderr, "Expected do, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_RETURN ) {
		fprintf( stderr, "Expected return, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_PRINT ) {
		fprintf( stderr, "Expected print, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_INCASE ) {
		fprintf( stderr, "Expected incase, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_WHEN ) {
		fprintf( stderr, "Expected when, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_FORCASE ) {
		fprintf( stderr, "Expected forcase, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==EXPECTED_CALL ) {
		fprintf( stderr, "Expected call, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==NAME_STARTS_WITH_DIGIT ) {
		fprintf( stderr, "Name starts with digit, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==INVALID_EXIT ) {
		fprintf( stderr, "Exit statement can be only inside loops, error at line %ld , in file %s", g_line, fileName );
	}
	else {
		fprintf( stderr, "COMPILER ERROR!!!!! UNDIFINED ERROR CODE? ERROR ID: %d, error at line %ld , in file %s", errcode, g_line, fileName );
	}


	fprintf( stderr, " \n" );

	exit( errcode );

}

void lexicalAnalyst() {
	int character;
	int pos = 0, timesAllocated = 0;
	char oneCharString[2] = { ' ', '\0' }; // convert one char to str
	char twoCharString[3] = { ' ', ' ', '\0' }; // convert 2 chars to str
	char *nextWord = malloc( sizeof *nextWord * WORD_ALLOC_SIZE * ++timesAllocated );

	if( nextWord==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	// Take next char 
	character = fgetc( fp );

	//-------------------------------------------
	// SKIP SPACES
	//-------------------------------------------
	while( isspace( character ) ) {
		if( character==NEW_LINE && countLine )
			g_line++;
		character = fgetc( fp );
	}
	countLine = 1;
	//-------------------------------------------
	// FOUND EOF
	//-------------------------------------------
	if( character==EOF ) {
		intedifyWord( STR_EOF );
		return;
	}

	//-------------------------------------------
	// FOUND ALPHANUMERIC ( starting with letter )
	//-------------------------------------------
	if( isalpha( character ) ) {
		nextWord[pos++] = character;
		while( isalnum( character = fgetc( fp ) ) ) {
			nextWord[pos++] = character;

			if( pos>=WORD_ALLOC_SIZE*timesAllocated )
			if( ( nextWord = realloc( nextWord, sizeof *nextWord * WORD_ALLOC_SIZE * ++timesAllocated ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );


		}
		nextWord[pos] = END_STRING;

		intedifyWord( nextWord );
		fseek( fp, -1, SEEK_CUR );

		free( nextWord );  //Clear memory for next word
		nextWord = NULL;
		return;
	}

	//-------------------------------------------
	// FOUND NUMERIC CONSTANT
	//-------------------------------------------
	if( isdigit( character ) ) {
		nextWord[pos++] = character;
		while( isdigit( character = fgetc( fp ) ) ) {
			nextWord[pos++] = character;

			if( pos>=WORD_ALLOC_SIZE*timesAllocated )
			if( ( nextWord = realloc( nextWord, sizeof *nextWord * WORD_ALLOC_SIZE * ++timesAllocated ) )==NULL )
				error( ALLOCATION_ERROR, CURRENT_LINE );


		}

		if( isalpha( character ) ) // EXAMPLE: 32352[a-Z]
			lexicalError( CHARACTER_INSIDE_NUMERIC_CONST );

		nextWord[pos] = END_STRING;

		if( atoi( nextWord ) < NUM_MIN||atoi( nextWord ) > NUM_MAX )
			lexicalError( NUMBER_OUT_OF_RANGE );

		intedifyWord( nextWord );
		fseek( fp, -1, SEEK_CUR );

		free( nextWord ); //Clear memory for next word
		nextWord = NULL;
		return;
	}

	//-------------------------------------------
	// FOUND / (May be comment)
	//-------------------------------------------
	if( character==CHAR_DIVISION ) {
		if( fgetc( fp )==STAR ) { // Comment found
			while( 1 ) {
				character = fgetc( fp );

				if( character==NEW_LINE ) // Must count lines inside comments
					g_line++;
				if( character==EOF )
					lexicalError( COMMENT_NOT_CLOSING );
				if( character==STAR ) {
					if( fgetc( fp )==CHAR_DIVISION ) // Comment ends here
						break;
					else
						fseek( fp, -1, SEEK_CUR );
				}

			}
			// Just ignore comment do not intedify
			lexicalAnalyst();
		}
		else { // DIVISION
			oneCharString[0] = CHAR_DIVISION;
			intedifyWord( oneCharString );
			fseek( fp, -1, SEEK_CUR );
		}
		return;
	}

	//-------------------------------------------
	// FOUND OPERATOR
	//-------------------------------------------
	if( isoperator( character ) ) {
		oneCharString[0] = character;
		intedifyWord( oneCharString );
		return;
	}

	//-------------------------------------------
	// FOUND < ( May be <>, <= )
	//-------------------------------------------
	if( character==CHAR_LESS ) {
		character = fgetc( fp );
		if( character==CHAR_MORE ) { // <>
			twoCharString[0] = CHAR_LESS;
			twoCharString[1] = CHAR_MORE;
			intedifyWord( twoCharString );
			return;
		}
		else if( character==CHAR_EQUALS ) { //<=
			twoCharString[0] = CHAR_LESS;
			twoCharString[1] = CHAR_EQUALS;
			intedifyWord( twoCharString );
			return;
		}
		else { // just <
			oneCharString[0] = CHAR_LESS;
			intedifyWord( oneCharString );
			fseek( fp, -1, SEEK_CUR );
			return;
		}
	}

	//-------------------------------------------
	// FOUND > ( May be >= )
	//-------------------------------------------
	if( character==CHAR_MORE ) {
		if( fgetc( fp )==CHAR_EQUALS ) { // >=
			twoCharString[0] = CHAR_MORE;
			twoCharString[1] = CHAR_EQUALS;
			intedifyWord( twoCharString );
			return;
		}
		else { // just >
			oneCharString[0] = CHAR_MORE;
			intedifyWord( oneCharString );
			fseek( fp, -1, SEEK_CUR );
			return;
		}
	}

	//-------------------------------------------
	// FOUND : (Must be := or error)
	//-------------------------------------------
	if( character==COLON ) {
		if( fgetc( fp )==CHAR_EQUALS ) { // found =
			twoCharString[0] = COLON;
			twoCharString[1] = CHAR_EQUALS;
			intedifyWord( twoCharString );
			return;
		}
		else { // Something else; error
			lexicalError( UNFINISHED_ASSIGN );
		}
	}


	//-------------------------------------------
	// FOUND UNKNOWN CHARACTER (error)
	//-------------------------------------------
	lexicalError( UNKNOWN_CHARACTER );

}

void intedifyWord( const char* word ) {
	if( token!=NULL ) { // Free last allocated word
		free( token );
		token = NULL;
	}
	token = malloc( sizeof *token * strlen( word )+1 );
	strcpy( token, word );

	if( strcmp( word, "and" )==0 ) {
		id = AND;
	}
	else if( strcmp( word, "declare" )==0 ) {
		id = DECLARE;
	}
	else if( strcmp( word, "do" )==0 ) {
		id = DO;
	}
	else if( strcmp( word, "else" )==0 ) {
		id = ELSE;
	}
	else if( strcmp( word, "enddeclare" )==0 ) {
		id = ENDDECLARE;
	}
	else if( strcmp( word, "exit" )==0 ) {
		id = EXIT;
	}
	else if( strcmp( word, "procedure" )==0 ) {
		id = PROCEDURE;
	}
	else if( strcmp( word, "function" )==0 ) {
		id = FUNCTION;
	}
	else if( strcmp( word, "print" )==0 ) {
		id = PRINT;
	}
	else if( strcmp( word, "incase" )==0 ) {
		id = INCASE;
	}
	else if( strcmp( word, "if" )==0 ) {
		id = IF;
	}
	else if( strcmp( word, "in" )==0 ) {
		id = IN;
	}
	else if( strcmp( word, "inout" )==0 ) {
		id = INOUT;
	}
	else if( strcmp( word, "not" )==0 ) {
		id = NOT;
	}
	else if( strcmp( word, "forcase" )==0 ) {
		id = FORCASE;
	}
	else if( strcmp( word, "program" )==0 ) {
		id = PROGRAM;
	}
	else if( strcmp( word, "or" )==0 ) {
		id = OR;
	}
	else if( strcmp( word, "return" )==0 ) {
		id = RETURN;
	}
	else if( strcmp( word, "while" )==0 ) {
		id = WHILE;
	}
	else if( strcmp( word, "copy" )==0 ) {
		id = COPY;
	}
	else if( strcmp( word, "when" )==0 ) {
		id = WHEN;
	}
	else if( strcmp( word, "<" )==0 ) {
		id = LESS;
	}
	else if( strcmp( word, "<=" )==0 ) {
		id = LESSEQ;
	}
	else if( strcmp( word, ">" )==0 ) {
		id = MORE;
	}
	else if( strcmp( word, ">=" )==0 ) {
		id = MOREEQ;
	}
	else if( strcmp( word, "<>" )==0 ) {
		id = DIFFERENT;
	}
	else if( strcmp( word, "=" )==0 ) {
		id = EQUALS;
	}
	else if( strcmp( word, ":=" )==0 ) {
		id = ASSIGN;
	}
	else if( strcmp( word, "+" )==0 ) {
		id = PLUS;
	}
	else if( strcmp( word, "-" )==0 ) {
		id = MINUS;
	}
	else if( strcmp( word, "*" )==0 ) {
		id = MULTIPLICATION;
	}
	else if( strcmp( word, "/" )==0 ) {
		id = DIVISION;
	}
	else if( strcmp( word, ";" )==0 ) {
		id = SEMICOLON;
	}
	else if( strcmp( word, "," )==0 ) {
		id = COMMA;
	}
	else if( strcmp( word, "(" )==0 ) {
		id = OPEN_PARATHENSIS;
	}
	else if( strcmp( word, ")" )==0 ) {
		id = CLOSE_PARATHENSIS;
	}
	else if( strcmp( word, "{" )==0 ) {
		id = OPEN_CURLY_BRACKET;
	}
	else if( strcmp( word, "}" )==0 ) {
		id = CLOSE_CURLY_BRACKET;
	}
	else if( strcmp( word, "[" )==0 ) {
		id = OPEN_BRACKET;
	}
	else if( strcmp( word, "]" )==0 ) {
		id = CLOSE_BRACKET;
	}
	else if( strcmp( word, "print" )==0 ) {
		id = PRINT;
	}
	else if( strcmp( word, "call" )==0 ) {
		id = CALL;
	}
	else if( strcmp( word, "EOF" )==0 ) {
		id = EOF;
	}
	else {
		id = ALNUM;
	}

}

void checkIfLegalName() {
	if( isdigit( token[0] ) ) {
		syntaxError( NAME_STARTS_WITH_DIGIT );
	}
}

void error( int errcode, long codeLine ) {
	fprintf( stderr, " -------- Error! --------\n " );

	if( errcode==FILE_NOT_FOUND ) {
		fprintf( stderr, "Can not find %s", fileName );
	}
	else if( errcode==TOO_MANY_ARGUMENTS ) {
		fprintf( stderr, "Too many arguments! Usage: Give only one file name" );
	}
	else if( errcode==ALLOCATION_ERROR ) {
		fprintf( stderr, "Allocation error! Can not allocate enough memory, code line: %ld", codeLine );
	}
	else {
		fprintf( stderr, "COMPILER ERROR!!!!! UNDIFINED ERROR CODE? ERROR ID: %d", errcode );
	}


	fprintf( stderr, " \n" );

	exit( errcode );

}

void symbolTableError( int errcode, char* var ) {
	fprintf( stderr, "---------- Error: " );

	if( errcode==UNDEFINED_VAR ) {
		fprintf( stderr, "Undefined var: \"%s\", line: %ld", var, g_line );
	}
	else {
		fprintf( stderr, "COMPILER ERROR!!!!! UNDIFINED ERROR CODE? ERROR ID: %d, error at line %ld , in file %s", errcode, g_line, fileName );
	}

	fprintf( stderr, "\n" );

	exit( errcode );
}

void lexicalError( int errcode ) {
	fprintf( stderr, " -------- Error at token \"%s\" -------- \n ", token );

	if( errcode==CHARACTER_INSIDE_NUMERIC_CONST ) {
		fprintf( stderr, "Character are not allowed in numeric constants, error at line %ld , in file %s", g_line, fileName );
	}
	else if( errcode==NUMBER_OUT_OF_RANGE ) {
		fprintf( stdout, "Numbers must be between %+d and %+d, error at line: %ld , in file %s", NUM_MIN, NUM_MAX, g_line, fileName );
	}
	else if( errcode==COMMENT_NOT_CLOSING ) {
		fprintf( stderr, "Commend not closing, error at line: %ld , in file %s", g_line, fileName );
	}
	else if( errcode==UNFINISHED_ASSIGN ) {
		fprintf( stderr, "Excepted a '=' after ':', error at line: %ld , in file %s", g_line, fileName );
	}
	else if( errcode==UNKNOWN_CHARACTER ) {
		fprintf( stderr, "Found an unknown character, error at line: %ld , in file %s", g_line, fileName );
	}
	else {
		fprintf( stderr, "COMPILER ERROR!!!!! UNDIFINED ERROR CODE? ERROR ID: %d, error at line %ld , in file %s", errcode, g_line, fileName );
	}


	fprintf( stderr, " \n" );

	exit( errcode );

}

void ungetWord() {
	//NOTE: Does not clear token or id!
	//NOTE: Works only for 1 byte newline systems
	fseek( fp, -1*strlen( token ), SEEK_CUR );
	countLine = 0; //We may have gone back one line
}

int isnumber( char* string ) {
	int i = 0;
	while( string[i]!='\0' ) {
		if( !isdigit( string[i++] ) )
			return 0;
	}
	return 1;
}

int iscompare( int c ) {
	return c=='<'||c=='>'||c=='=';
}

int isoperator( int c ) {
	return ( c=='{'||c=='}'||c=='('||c==')'||c=='['||c==']'||c=='+'||c=='-'||c=='*'||c==';'||c==','||c=='=' );
}

void getInputForFilename() {
	char buffer[NAME_SIZE];

	printf( "Give file name: " );
	fgets( buffer, sizeof buffer, stdin );

	fileName = parseFilenameFromInput( buffer );
}

char* parseFilenameFromInput( const char* input ) {
	char* filenameFromInput = malloc( sizeof *filenameFromInput * strlen( input )+1 );
	int inputPos = 0, filenamePos = 0;

	if( filenameFromInput==NULL )
		error( ALLOCATION_ERROR, CURRENT_LINE );

	while( input[inputPos]!=NEW_LINE ) {
		filenameFromInput[filenamePos++] = input[inputPos++];
	}
	filenameFromInput[filenamePos] = END_STRING;
	return filenameFromInput;
}
