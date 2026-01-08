#include "register_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 4
static char* available_registers[] = {"EAX", "EBX", "ECX", "EDX"};
static int register_map[NUM_REGISTERS] = {0}; // 0 = free, 1 = occupied

// Function to allocate a register
char* allocate_register() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!register_map[i]) { // If register is free
            register_map[i] = 1; // Mark as occupied
            return available_registers[i];
        }
    }
    return NULL; // No free register (needs memory spill)
}

// Function to free a register
void free_register(char* reg) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (strcmp(reg, available_registers[i]) == 0) {
            register_map[i] = 0; // Mark as free
            return;
        }
    }
}

// Function to spill to memory when registers are full
char* spill_to_memory(char* temp_var) {
    static int memory_location = 0;
    char* mem_slot = (char*)malloc(10);
    sprintf(mem_slot, "[MEM_%d]", memory_location++);
    return mem_slot;
}
