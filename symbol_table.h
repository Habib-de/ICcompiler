#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h" // Include AST structures
#include <stdlib.h> 

// Define data types for identifiers
typedef enum {
    TYPE_UNKNOWN, 
    TYPE_NUMBER
} DataType;

// Structure for a Symbol Table Entry
typedef struct SymbolTableEntry {
    char* name;
    DataType type;
    struct SymbolTableEntry* next;
} SymbolTableEntry;

// Structure for Symbol Table
typedef struct SymbolTable {
    size_t size;
    SymbolTableEntry** buckets;
} SymbolTable;

// Function Prototypes
size_t hash_string(const char* str, size_t table_size);
SymbolTable* create_symbol_table(size_t size);
void destroy_symbol_table(SymbolTable* table);
int add_symbol(SymbolTable* table, const char* name, DataType type);
SymbolTableEntry* lookup_symbol(SymbolTable* table, const char* name);
void print_symbol_table(SymbolTable* table);

// ✅ Added the missing global symbol table function declaration
extern SymbolTable* get_global_symbol_table();

#endif // SYMBOL_TABLE_H
