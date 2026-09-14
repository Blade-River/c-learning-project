#include "memory.h"
#include <stdint.h>

uint8_t read_memory(mmemory *memori,uint8_t address){
  return memori->memory[address];
}

void write_memory(mmemory *memory, uint8_t address, uint8_t value){
  memory->memory[address] = value;
}

void init_memory(mmemory *memory){
  for (int i =  0; i < memory_size; i++){
    memory->memory[i] = 0;
  }
}
