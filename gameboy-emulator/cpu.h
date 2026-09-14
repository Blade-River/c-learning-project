#ifndef CPU_H
#define CPU_H

#define flag_c 0x20
#define flag_z 0x10
#define flag_n 0x80
#define flag_hf 0x40

#include <stdint.h>
#include "memory.h"

typedef struct{
  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t H;
  uint8_t L;
  uint8_t F;

  uint16_t sp;
  uint16_t pc;

  uint8_t halet;


}Cpu;

void cpu_init(Cpu *cpu);
uint16_t get_af(Cpu *cpu);
uint16_t get_bc(Cpu *cpu);
uint16_t get_de(Cpu *cpu);
uint16_t get_hl(Cpu *cpu);

void set_ad(Cpu *cpu, uint16_t value);
void set_bc(Cpu *cpu, uint16_t value);
void set_de(Cpu *cpu, uint16_t value);
void set_hl(Cpu *cpu, uint16_t value);

void set_flag(Cpu *cpu, uint8_t flag);
void clear_flag(Cpu *cpu, uint8_t flag);
int get_flag(Cpu *cpu, uint8_t flag);

void cpu_cycle(Cpu *cpu,mmemory *memory);
#endif

