#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

char* allocate_register();
void free_register(char* reg);
char* spill_to_memory(char* temp_var);

#endif // REGISTER_ALLOCATOR_H
