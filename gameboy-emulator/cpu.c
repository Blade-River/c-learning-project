#include "cpu.h"
#include "memory.h"
#include <stdint.h>

void cpu_init(Cpu *cpu)
{
    cpu->A = 0;
    cpu->F = 0;
    cpu->B = 0;
    cpu->C = 0;
    cpu->D = 0;
    cpu->E = 0;
    cpu->H = 0;
    cpu->L = 0;

    cpu->halet = 0;
    cpu->pc = 0;
    cpu->sp = 0;
}

uint16_t get_af(Cpu *cpu)
{
    return ((uint16_t)cpu->A << 8) | cpu->F;
}

uint16_t get_bc(Cpu *cpu)
{
    return ((uint16_t)cpu->B << 8) | cpu->C;
}

uint16_t get_de(Cpu *cpu)
{
    return ((uint16_t)cpu->D << 8) | cpu->E;
}

uint16_t get_hl(Cpu *cpu)
{
    return ((uint16_t)cpu->H << 8) | cpu->L;
}

void set_af(Cpu *cpu, uint16_t value)
{
    cpu->A = value >> 8;
    cpu->F = value & 0xF0;
}

void set_bc(Cpu *cpu, uint16_t value)
{
    cpu->B = value >> 8;
    cpu->C = value & 0xFF;
}

void set_de(Cpu *cpu, uint16_t value)
{
    cpu->D = value >> 8;
    cpu->E = value & 0xFF;
}

void set_hl(Cpu *cpu, uint16_t value)
{
    cpu->H = value >> 8;
    cpu->L = value & 0xFF;
}

void set_flag(Cpu *cpu, uint8_t flag)
{
    cpu->F |= flag;
}

void clear_flag(Cpu *cpu, uint8_t flag)
{
    cpu->F &= ~flag;
}

int get_flag(Cpu *cpu, uint8_t flag)
{
    return (cpu->F & flag) != 0;
}


void cpu_cycle(Cpu *cpu, mmemory *memory)
{
    uint8_t opcode = read_memory(memory, cpu->pc);

    switch (opcode)
    {
        
        case 0x06:  
            cpu->B = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x0E:  
            cpu->C = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x16:  
            cpu->D = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x1E:  
            cpu->E = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x26: 
            cpu->H = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x2E:  
            cpu->L = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;

        case 0x3E:  
            cpu->A = read_memory(memory, cpu->pc + 1);
            cpu->pc += 2;
            break;


        case 0x40:  
            cpu->B = cpu->B;
            cpu->pc++;
            break;

        case 0x41:  
            cpu->B = cpu->C;
            cpu->pc++;
            break;

        case 0x42:  
            cpu->B = cpu->D;
            cpu->pc++;
            break;

        case 0x43:  
            cpu->B = cpu->E;
            cpu->pc++;
            break;

        case 0x44: 
            cpu->B = cpu->H;
            cpu->pc++;
            break;

        case 0x45:  
            cpu->B = cpu->L;
            cpu->pc++;
            break;

        case 0x47:  
            cpu->B = cpu->A;
            cpu->pc++;
            break;

        case 0x48:
            cpu->C = cpu->B;
            cpu->pc++;
            break;

        case 0x49:
            cpu->C = cpu->C;
            cpu->pc++;
            break;

        case 0x4A:
            cpu->C = cpu->D;
            cpu->pc++;
            break;

        case 0x4B:
            cpu->C = cpu->E;
            cpu->pc++;
            break;

        case 0x4C:
            cpu->C = cpu->H;
            cpu->pc++;
            break;

        case 0x4D:
            cpu->C = cpu->L;
            cpu->pc++;
            break;

        case 0x4F:
            cpu->C = cpu->A;
            cpu->pc++;
            break;

        case 0x50:
            cpu->D = cpu->B;
            cpu->pc++;
            break;

        case 0x51:
            cpu->D = cpu->C;
            cpu->pc++;
            break;

        case 0x52:
            cpu->D = cpu->D;
            cpu->pc++;
            break;

        case 0x53:
            cpu->D = cpu->E;
            cpu->pc++;
            break;

        case 0x54:
            cpu->D = cpu->H;
            cpu->pc++;
            break;

        case 0x55:
            cpu->D = cpu->L;
            cpu->pc++;
            break;

        case 0x57:
            cpu->D = cpu->A;
            cpu->pc++;
            break;

        case 0x58:
            cpu->E = cpu->B;
            cpu->pc++;
            break;

        case 0x59:
            cpu->E = cpu->C;
            cpu->pc++;
            break;

        case 0x5A:
            cpu->E = cpu->D;
            cpu->pc++;
            break;

        case 0x5B:
            cpu->E = cpu->E;
            cpu->pc++;
            break;

        case 0x5C:
            cpu->E = cpu->H;
            cpu->pc++;
            break;

        case 0x5D:
            cpu->E = cpu->L;
            cpu->pc++;
            break;

        case 0x5F:
            cpu->E = cpu->A;
            cpu->pc++;
            break;

        case 0x60:
            cpu->H = cpu->B;
            cpu->pc++;
            break;

        case 0x61:
            cpu->H = cpu->C;
            cpu->pc++;
            break;

        case 0x62:
            cpu->H = cpu->D;
            cpu->pc++;
            break;

        case 0x63:
            cpu->H = cpu->E;
            cpu->pc++;
            break;

        case 0x64:
            cpu->H = cpu->H;
            cpu->pc++;
            break;

        case 0x65:
            cpu->H = cpu->L;
            cpu->pc++;
            break;

        case 0x67:
            cpu->H = cpu->A;
            cpu->pc++;
            break;

        case 0x68:
            cpu->L = cpu->B;
            cpu->pc++;
            break;

        case 0x69:
            cpu->L = cpu->C;
            cpu->pc++;
            break;

        case 0x6A:
            cpu->L = cpu->D;
            cpu->pc++;
            break;

        case 0x6B:
            cpu->L = cpu->E;
            cpu->pc++;
            break;

        case 0x6C:
            cpu->L = cpu->H;
            cpu->pc++;
            break;

        case 0x6D:
            cpu->L = cpu->L;
            cpu->pc++;
            break;

        case 0x6F:
            cpu->L = cpu->A;
            cpu->pc++;
            break;

        case 0x78:
            cpu->A = cpu->B;
            cpu->pc++;
            break;

        case 0x79:
            cpu->A = cpu->C;
            cpu->pc++;
            break;

        case 0x7A:
            cpu->A = cpu->D;
            cpu->pc++;
            break;

        case 0x7B:
            cpu->A = cpu->E;
            cpu->pc++;
            break;

        case 0x7C:
            cpu->A = cpu->H;
            cpu->pc++;
            break;

        case 0x7D:
            cpu->A = cpu->L;
            cpu->pc++;
            break;

        case 0x7F:
            cpu->A = cpu->A;
            cpu->pc++;
            break;
        case 0x01:
          {
            uint8_t l = read_memory(memory, cpu->pc + 1);
            uint8_t high = read_memory(memory, cpu->pc + 2);
            uint16_t value = l | (uint16_t)high << 8;
            set_bc(cpu, value);
            cpu->pc += 3;
            break;
          }
        case 0x11:
          {
            uint8_t low = read_memory(memory, cpu->pc + 1);
            uint8_t high = read_memory(memory, cpu->pc + 2);
            uint16_t value = low |  (uint16_t)high << 8;
            set_de(cpu, value);
            cpu->pc += 3;
            break;
          }
        case 0x21:
          {
            uint8_t low = read_memory(memory, cpu->pc + 1);
            uint8_t high = read_memory(memory, cpu->pc + 2);
            uint16_t value = low | (uint16_t)high << 8;
            set_hl(cpu, value);
            cpu->pc += 3;
            break;
          }
        case 0x31:
          {
            uint8_t low = read_memory(memory, cpu->pc + 1);
            uint8_t high = read_memory(memory, cpu->pc + 2);
            cpu->sp = low | (uint16_t)high << 8;
            cpu->pc += 3;
            break;
          }
        case 0x03:
          {
            set_bc(cpu, get_bc(cpu) + 1);
            cpu->pc++;
            break;
          }
        case 0x13:
          {
            set_de(cpu, get_de(cpu) + 1);
            cpu->pc++;
            break;
          }
        case 0x23:
          {
            set_hl(cpu, get_hl(cpu) + 1);
            cpu->pc++;
            break;
          }
        case 0x33:
          {
            cpu->sp++;
            cpu->pc++;
            break;
          }
        case 0x0B:
          {
            set_bc(cpu, get_bc(cpu) - 1);
            cpu->pc++;
            break;
          }
        case 0x1B:
          {
            set_de(cpu, get_de(cpu) - 1);
            cpu->pc++;
            break;
          }
        case 0x2B:
          {
            set_hl(cpu, get_hl(cpu) - 1);
            cpu->pc++;
            break;
          }
        case 0x3B:
          {
            cpu->sp--;
            cpu->pc++;
            break;
          }
        case 0x09:
          {
            set_hl(cpu, get_hl(cpu) + get_bc(cpu));
            cpu->pc++;
            break;
          }
        case 0x19:
          {
            set_hl(cpu, get_hl(cpu) + get_de(cpu));
            cpu->pc++;
            break;
          }
        case 0x29:
          {
            set_hl(cpu, get_hl(cpu) + get_hl(cpu) );
            cpu->pc++;
            break;
          }
        case 0x39:
          {
            set_hl(cpu, get_hl(cpu) + cpu->pc);
            cpu->pc++;
            break;
          }
        case 0xF9:
          {
            cpu->pc = get_hl(cpu);
            cpu->pc++;
            break;
          }
    
    }
}
