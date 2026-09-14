#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#define memory_size 0x10000
typedef struct{
  uint8_t memory[memory_size];
}mmemory;

void memory_init(mmemory *memory);
uint8_t read_memory(mmemory *memory, uint8_t address);
void write_memory(mmemory *memory,uint8_t address, uint8_t value);

#endif
