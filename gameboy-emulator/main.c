#include "cpu.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>

int main(){
  Cpu cpu;
  mmemory memory;
  cpu_init(&cpu);
  memory_init(&memory);
  return 0;
}
