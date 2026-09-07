#include <complex.h>
#include <endian.h>
#include <stdio.h>
#include <stdint.h>

#define memory_size 1024
#define register_count 8
#define display_size 64
#define zero_flag 0x01
#define carry_flag 0x02
#define n_flag 0x04

typedef struct{
  uint8_t memory[memory_size];
  uint8_t R[register_count];
  uint16_t sp;
  uint16_t pc;
  uint8_t running;
  uint8_t flags;
  uint8_t display[display_size];
} Cpu;
uint8_t program[] =
{
    // R0 = 5
    0x10, 0x00, 0x05,

    // R1 = 3
    0x10, 0x01, 0x03,

    // R0 = R0 + R1
    0x20, 0x01,

    // STORE R0 -> memory[100]
    0x50, 0x00, 0x64, 0x00,

    // LOAD memory[100] -> R2
    0x51, 0x02, 0x64, 0x00,

    // PRINT R2
    0x60, 0x02,

    // HALT
    0xFF
};

void init_cpu(Cpu *cpu){
  for (int a = 0; a < memory_size; a++){
    cpu->memory[a] = 0;
  }
  for (int a = 0; a < register_count; a++){
    cpu->R[a] = 0;
  }
  cpu->sp = memory_size - 1;
  cpu->pc = 0;
  cpu->running = 1;
  cpu->flags = 0;
  for (int a = 0; a < display_size; a++){
    cpu->display[a] = 0; 
  }
  for (int a = 0; a < sizeof(program); a++){
    cpu->memory[a] = program[a];
  }
}
void zflag(Cpu *cpu,uint8_t result){
  if (result == 0){
    cpu->flags |= zero_flag;
  }
  else{
    cpu->flags &= ~zero_flag;
  }
}
void cflag(Cpu *cpu,uint16_t result){
  if (result > 255){
    cpu->flags |= carry_flag;
  }
  else{
    cpu->flags &= ~carry_flag;
  }
}
void nflag(Cpu *cpu, uint8_t result){
  if (result & 0x80){
    cpu->flags |= n_flag; 
  }
  else{
    cpu->flags &= ~n_flag;
  }
}

void push(Cpu *cpu, uint8_t reg){
  cpu->memory[cpu->sp] = reg;
  cpu->sp--;
}
uint8_t pop(Cpu *cpu){
  cpu->sp++;
  return cpu->memory[cpu->sp];
}
uint16_t fetch_address(Cpu *cpu){
  uint8_t low = cpu->memory[cpu->pc++];
  uint8_t high = cpu->memory[cpu->pc++];

  uint16_t address = low | ((uint16_t)high << 8);
  return address;
}

uint8_t fetch_opcode(Cpu *cpu){
  uint8_t opcode = cpu->memory[cpu->pc];
  cpu->pc++;
  return opcode;
}

void execute(Cpu *cpu, uint8_t opcode){
  switch (opcode){
    case 0x10:
    {
      //giving register value
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint8_t value = cpu->memory[cpu->pc];
      cpu->pc++;

      cpu->R[reg] = value;
      break;
    }
    case 0x20:
    {
      //addition
      uint8_t r1 = cpu->memory[cpu->pc];
      cpu->pc++;
      uint8_t x = (r1 >> 4) & 0x0F;
      uint8_t y = r1 & 0x0F;
      
      uint16_t value = cpu->R[x] + cpu->R[y];
      cflag(cpu, value);
      cpu->R[x] = (uint8_t)value;
      zflag(cpu, cpu->R[x]);
      nflag(cpu,cpu->R[x]);
      break;
      
    }
    case 0x21:
    {
      //subsraction
      uint8_t r1 = cpu->memory[cpu->pc];
      cpu->pc++;
      uint8_t x = (r1 >> 4) & 0x0F;
      uint8_t y = r1 & 0x0F;

      uint16_t value = cpu->R[x] - cpu->R[y];
      cpu->R[x] = (uint8_t)value;
      zflag(cpu, cpu->R[x]);
      nflag(cpu, cpu->R[x]);

      break;
    }
    case 0x22:
    {
      //increase
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint16_t result = cpu->R[reg];
      result++;
      cflag(cpu, result);
      cpu->R[reg] = (uint8_t)result;
      zflag(cpu, cpu->R[reg]);
      nflag(cpu, cpu->R[reg]);
      break;  
    }
    case 0x23:
    {
      //deacrease
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint16_t result = cpu->R[reg];
      result--;
      cpu->R[reg] = (uint8_t)result;
      nflag(cpu,cpu->R[reg]);
      zflag(cpu,cpu->R[reg]);
      break;

    }
    case 0x11:
    {
      //move register value;
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint8_t x = (reg >> 4) & 0x0F;
      uint8_t y = reg & 0x0F;
      cpu->R[x] = cpu->R[y];
      break;
    }
    case 0x30:
    {
      //jump
      uint16_t address = fetch_address(cpu);
      cpu->pc = address;
      break;
    }
    case 0x31:
    {
      uint16_t address = fetch_address(cpu);
      if (cpu->flags & zero_flag){
        cpu->pc = address; 
      }
      break;
    }
    case 0x32:
    {
      uint16_t address = fetch_address(cpu);
      if (!(cpu->flags & zero_flag)){
        cpu->pc = address;
      }
      break;
    }
    case 0x33:
    {
      uint16_t address = fetch_address(cpu);
      if (cpu->flags & n_flag){
        cpu->pc = address;
      }
      break;
    }
    case 0x34:
    {
      uint16_t address = fetch_address(cpu);
      if (cpu->flags & carry_flag){
        cpu->pc = address;
      }
      break;
    }
    case 0x35:
    {
      uint16_t address = fetch_address(cpu);
      if (!(cpu->flags & carry_flag)){
        cpu->pc = address;
      }
      break;
    }
    case 0x50:
    {
      //store
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint16_t address = fetch_address(cpu);
      cpu->memory[address] = cpu->R[reg];
      break;
    }
    case 0x51:
    {
      //load
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      uint16_t address = fetch_address(cpu);
      cpu->R[reg] = cpu->memory[address];
      break;
    }
    case 0x40:
    {
      //psuh
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      push(cpu,cpu->R[reg]);
      break;
    }
    case 0x41:
    {
      //pop
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      cpu->R[reg] = pop(cpu);
      break;
    }
    case 0x42:
    {
      //call
      uint16_t address = fetch_address(cpu);
      uint16_t return_address = cpu->pc;
      push(cpu,(uint8_t)(return_address & 0xFF));
      push(cpu,(uint8_t)(return_address >> 8));
      cpu->pc = address;
      break;
    }
    case 0x43:
    {
      //ret
      uint8_t high = pop(cpu);
      uint8_t low = pop(cpu);
      uint16_t address = low | ((uint16_t)high << 8);
      cpu->pc = address;
      break;
    }
    case 0x60:
    {
      uint8_t reg = cpu->memory[cpu->pc];
      cpu->pc++;
      printf("%d",cpu->R[reg]);
      break;
    }
    case 0xFF:
    {
      cpu->running = 0;
      break;
    }
  }
}

int main(){
  Cpu cpu;
  init_cpu(&cpu);
  while (cpu.running == 1){
     uint8_t opcode =  fetch_opcode(&cpu);
     execute(&cpu, opcode);
  }
  return 0;
}
