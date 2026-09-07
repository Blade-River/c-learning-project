#include <stdio.h>
#include <stdint.h>

typedef struct{
  uint8_t memory[256];
  uint8_t A,B,C,D;
  uint8_t pc;
  uint8_t running_time;
} Cpu;
uint8_t program[] =
{
    0x10, 0x05,   // A = 5
    0x20, 0x03,   // B = 3
    0x30,         // A = A + B
    0x60,         // print A
    0xFF          // halt
};
void init_cpu(Cpu *cpu){ 
  for (int a = 0; a < 256; a++){
    cpu->memory[a] = 0;
  }
  cpu->A = 0;
  cpu->B = 0;
  cpu->C = 0;
  cpu->D = 0;
  cpu->running_time = 1;
  for (int a = 0; a < sizeof(program); a++){
    cpu->memory[a] = program[a];
  }
}
uint8_t fetch(Cpu *cpu){
  uint8_t opcode = cpu->memory[cpu->pc];
  cpu->pc++;
  return opcode;
}
void execute(Cpu *cpu,uint8_t opcode){
  switch (opcode) {
    case 0x10:
    {
      uint8_t value = cpu->memory[cpu->pc];
      cpu->pc++;
      cpu->A = value;
      break;
    }
    case 0x20:
    {
      uint8_t value = cpu->memory[cpu->pc];
      cpu->pc++;
      cpu->B = value;
      break;
    }
    case 0x30:
    {
     cpu->A = cpu->A + cpu->B;
     break;
    }
    case 0x60:
    {
      printf("value: %d",cpu->A);
      break;
    }
    case 0xFF:
    {
      cpu->running_time =0;
      break;
    }
  }
}
int main(){
  Cpu cpu;
  init_cpu(&cpu);
  while (cpu.running_time == 1){
    uint8_t opcode = fetch(&cpu);
    execute(&cpu, opcode);
  }
  return 0;
}
