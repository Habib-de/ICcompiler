#include "ast.h"
#include <stdlib.h>
#include <string.h>

AST_Node* create_number_node(int value) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_NUMBER;
    node->value.int_value = value;
    node->left = node->right = node->expression = node->identifier = NULL;
    node->statements = NULL;
    return node;
}

AST_Node* create_float_node(double value) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_NUMBER;
    node->value.float_value = value;
    node->left = node->right = node->expression = node->identifier = NULL;
    node->statements = NULL;
    return node;
}

AST_Node* create_identifier_node(char* name) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_IDENTIFIER;
    node->value.string_value = strdup(name); // Duplicate string
    node->left = node->right = node->expression = node->identifier = NULL;
    node->statements = NULL;
    return node;
}

AST_Node* create_binary_op_node(int op_type, AST_Node* left, AST_Node* right) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_BINARY_OP;
    node->value.int_value = op_type;
    node->left = left;
    node->right = right;
    node->expression = node->identifier = NULL;
    node->statements = NULL;
    return node;
}

AST_Node* create_assignment_node(AST_Node* identifier, AST_Node* expression) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_ASSIGNMENT;
    node->identifier = identifier;
    node->expression = expression;
    node->left = node->right = NULL;
    node->statements = NULL;
    return node;
}

AST_Node* create_print_statement_node(AST_Node* expression) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_PRINT_STATEMENT;
    node->expression = expression;
    node->left = node->right = node->identifier = NULL;
    node->statements = NULL;
    return node;
}

StatementList* create_statement_list_node(AST_Node* statement) {
    StatementList* list = (StatementList*)malloc(sizeof(StatementList));
    list->statement = statement;
    list->next = NULL;
    return list;
}

StatementList* append_statement_list(StatementList* list, AST_Node* statement) {
    StatementList* new_node = create_statement_list_node(statement);
    StatementList* current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return list;
}

AST_Node* create_program_node(StatementList* statements) {
    AST_Node* node = (AST_Node*)malloc(sizeof(AST_Node));
    node->type = NODE_PROGRAM;
    node->statements = statements;
    node->left = node->right = node->expression = node->identifier = NULL;
    return node;
}

void free_ast(AST_Node* root) {
    if (root == NULL) return;

    // Recursively free child nodes
    free_ast(root->left);
    free_ast(root->right);
    free_ast(root->expression);
    free_ast(root->identifier);

    if (root->statements) {
        StatementList* current = root->statements;
        while (current) {
            StatementList* next = current->next;
            free_ast(current->statement);
            free(current);
            current = next;
        }
    }

    free(root);
}