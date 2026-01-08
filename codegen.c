// codegen.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "register_allocator.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include "ast.h"

// Helper function to emit assembly
void emit(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

// Function to generate assembly from AST
void generate_assembly(AST_Node* node) {
    if (node == NULL) return;

    static int label_counter = 0;

    switch (node->type) {
        case NODE_PROGRAM:
            emit(".section .data\n");
            SymbolTable* global_table = get_global_symbol_table();
            if (global_table) {
                for (size_t i = 0; i < global_table->size; i++) {
                    SymbolTableEntry* current = global_table->buckets[i];
                    while (current) {
                        emit("%s: .long 0\n", current->name);
                        current = current->next;
                    }
                }
            }
            emit("\n.section .text\n");
            emit(".globl _start\n");
            emit("_start:\n");
            {
                StatementList* stmt = node->statements;
                while (stmt) {
                    generate_assembly(stmt->statement);
                    stmt = stmt->next;
                }
            }
            emit("\n    # Exit program\n");
            emit("    movl $1, %%eax\n");
            emit("    xorl %%ebx, %%ebx\n");
            emit("    int $0x80\n");
            break;

        case NODE_NUMBER:
            emit("    movl $%d, %%eax\n", (int)node->value.float_value);
            break;

        case NODE_IDENTIFIER:
            emit("    movl %s, %%eax\n", node->value.string_value);
            break;

        case NODE_BINARY_OP:
            generate_assembly(node->left);
            emit("    pushl %%eax\n");

            generate_assembly(node->right);
            emit("    movl (%%esp), %%ebx\n");
            emit("    addl $4, %%esp\n");

            switch (node->value.int_value) {
                case PLUS:  emit("    addl %%ebx, %%eax\n"); break;
                case MINUS: emit("    subl %%ebx, %%eax\n"); break;
                case TIMES: emit("    imull %%ebx, %%eax\n"); break;
                case DIVIDE:
                    emit("    cdq\n");
                    emit("    idivl %%ebx\n");
                    break;
                default: emit("    # Unknown operator\n"); break;
            }
            break;

        case NODE_ASSIGNMENT:
            generate_assembly(node->expression);
            emit("    movl %%eax, %s\n", node->identifier->value.string_value);
            break;

        case NODE_PRINT_STATEMENT:
            generate_assembly(node->expression);
            emit("    pushl %%eax\n");
            emit("    call print_int\n"); // Assuming external print_int function
            emit("    addl $4, %%esp\n");
            break;

        default:
            emit("    # Unhandled AST Node Type: %d\n", node->type);
            break;
    }
}

// ... (rest of your codegen.c) ...

void codegen() {
    extern AST_Node* program_ast_root;
    if (program_ast_root) {
        generate_assembly(program_ast_root);
    } else {
        fprintf(stderr, "Error: No AST root to generate code from.\n");
    }
}