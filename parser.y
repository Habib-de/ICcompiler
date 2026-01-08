%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strdup
#include "ast.h"    // Include our AST node definitions
#include "symbol_table.h" // Include our Symbol Table definitions

int yylex(void);
void yyerror(const char *s);

// Define the root of the AST here (allocate memory and initialize)
AST_Node* program_ast_root = NULL;

// Define the global Symbol Table
SymbolTable* global_symbol_table = NULL;

// Forward declarations for struct types used in %union
// This helps Bison recognize them correctly when defining the union
struct AST_Node;
struct StatementList;
struct SymbolTableEntry; // Forward declare SymbolTableEntry as well if needed in %union


%}

%union {
    int int_val;
    double float_val;
    char* string_value;
    struct AST_Node* ast_node;
    struct StatementList* stmt_list;
}

%token <string_value> IDENTIFIER
%token <float_val> NUMBER
%token PRINT DEF RETURN
%token PLUS MINUS TIMES DIVIDE EQUALS
%token LPAREN RPAREN
%token INVALID

%type <ast_node> program statement expression_statement print_statement expression term factor
%type <stmt_list> statements

%%

program:
      /* empty */ { $$ = create_program_node(NULL); program_ast_root = $$; }
    | statements { $$ = create_program_node($1); program_ast_root = $$; }
    ;

statements:
      statement { $$ = create_statement_list_node($1); }
    | statements statement { $$ = append_statement_list($1, $2); }
    ;

statement:
      expression_statement { $$ = $1; }
    | print_statement { $$ = $1; }
    | INVALID { printf("Syntax Error: Skipping invalid token.\\n"); yyerrok; $$ = NULL; }
    ;

expression_statement:
      IDENTIFIER EQUALS expression {
        add_symbol(global_symbol_table, $1, TYPE_NUMBER);
        $$ = create_assignment_node(create_identifier_node(strdup($1)), $3);
      }
    ;

print_statement:
      PRINT LPAREN expression RPAREN { $$ = create_print_statement_node($3); }
    ;

expression:
      expression PLUS term   { $$ = create_binary_op_node(PLUS, $1, $3); }
    | expression MINUS term  { $$ = create_binary_op_node(MINUS, $1, $3); }
    | term                   { $$ = $1; }
    ;

term:
      term TIMES factor   { $$ = create_binary_op_node(TIMES, $1, $3); }
    | term DIVIDE factor  { $$ = create_binary_op_node(DIVIDE, $1, $3); }
    | factor              { $$ = $1; }
    ;

factor:
      NUMBER              { $$ = create_float_node($1); }
    | IDENTIFIER          {
        SymbolTableEntry* entry = lookup_symbol(global_symbol_table, $1);
        if (entry == NULL) {
            fprintf(stderr, "Error: Use of undeclared identifier '%s'\n", $1);
        }
        $$ = create_identifier_node(strdup($1));
      }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}