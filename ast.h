#ifndef AST_H
#define AST_H

// Define the types of AST nodes
typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGNMENT,
    NODE_PRINT_STATEMENT,
    NODE_PROGRAM // Top-level node for a sequence of statements
} AST_NodeType;

// Define a union to hold different types of values in nodes
typedef union {
    int int_value;      // For integer numbers (simplified)
    double float_value; // For float numbers (simplified)
    char* string_value; // For identifier names
} AST_NodeValue;

// Forward declaration for recursive structure
struct AST_Node;

// Structure for a list of statements (for the program node)
typedef struct StatementList {
    struct AST_Node* statement;
    struct StatementList* next;
} StatementList;


// Define the structure for an AST node
typedef struct AST_Node {
    AST_NodeType type;
    AST_NodeValue value; // Holds the value for literals/identifiers

    // Pointers to child nodes (used for operations, assignments, etc.)
    struct AST_Node* left;
    struct AST_Node* right;
    struct AST_Node* expression; // For print statements, return statements
    struct AST_Node* identifier; // For assignment statements

    // For the program node (list of statements)
    StatementList* statements;

} AST_Node;

// Function prototypes for AST creation (implemented in parser.y actions)
AST_Node* create_number_node(int value); // Simplified for integers
AST_Node* create_float_node(double value); // Simplified for floats
AST_Node* create_identifier_node(char* name);
AST_Node* create_binary_op_node(int op_type, AST_Node* left, AST_Node* right); // op_type could be token type (PLUS, MINUS, etc.)
AST_Node* create_assignment_node(AST_Node* identifier, AST_Node* expression);
AST_Node* create_print_statement_node(AST_Node* expression);
StatementList* create_statement_list_node(AST_Node* statement);
StatementList* append_statement_list(StatementList* list, AST_Node* statement); // ADD THIS LINE
AST_Node* create_program_node(StatementList* statements);


// Function prototype for printing the AST (implemented in lexernew.c or a separate file)
void print_ast(AST_Node* root, int indent);
void free_ast(AST_Node* root); // Function to free allocated AST memory

#endif // AST_H
