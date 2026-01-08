#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global symbol table instance
static SymbolTable* global_symbol_table = NULL;

// Function to get the global symbol table
SymbolTable* get_global_symbol_table() {
    if (!global_symbol_table) {
        global_symbol_table = create_symbol_table(100); // Initialize the table
    }
    return global_symbol_table;
}

// Hash function (DJB2)
size_t hash_string(const char* str, size_t table_size) {
    unsigned long hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + (*str++);
    }
    return hash % table_size;
}

// Create a new symbol table
SymbolTable* create_symbol_table(size_t size) {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!table) {
        perror("Symbol Table Allocation Failed");
        exit(EXIT_FAILURE);
    }
    table->size = size;
    table->buckets = (SymbolTableEntry**)calloc(size, sizeof(SymbolTableEntry*));
    return table;
}

// Destroy the symbol table
void destroy_symbol_table(SymbolTable* table) {
    if (!table) return;
    for (size_t i = 0; i < table->size; i++) {
        SymbolTableEntry* current = table->buckets[i];
        while (current) {
            SymbolTableEntry* next = current->next;
            free(current->name);
            free(current);
            current = next;
        }
    }
    free(table->buckets);
    free(table);
}

// Add a new entry to the symbol table
int add_symbol(SymbolTable* table, const char* name, DataType type) {
    if (!table || !name) return -1;
    size_t index = hash_string(name, table->size);
    SymbolTableEntry* current = table->buckets[index];
    
    while (current) {
        if (strcmp(current->name, name) == 0) {
            fprintf(stderr, "Error: Redefinition of identifier '%s'\n", name);
            return -1;
        }
        current = current->next;
    }

    SymbolTableEntry* new_entry = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    new_entry->name = strdup(name);
    new_entry->type = type;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    
    return 0;
}

// Lookup an identifier in the symbol table
SymbolTableEntry* lookup_symbol(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    size_t index = hash_string(name, table->size);
    SymbolTableEntry* current = table->buckets[index];

    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Debug function: Print symbol table contents
void print_symbol_table(SymbolTable* table) {
    if (!table) {
        printf("Symbol Table is NULL.\n");
        return;
    }
    printf("\n--- Symbol Table ---\n");
    for (size_t i = 0; i < table->size; i++) {
        SymbolTableEntry* current = table->buckets[i];
        while (current) {
            printf("  Name: %s, Type: %s\n", current->name, 
                   (current->type == TYPE_NUMBER) ? "NUMBER" : "UNKNOWN");
            current = current->next;
        }
    }
    printf("--------------------\n");
}
