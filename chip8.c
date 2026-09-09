#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <raylib.h>
#define memory_size 4096
#define register_count 16
#define stack_count 16
#define display_size 32*64
#define keypad_size 16
#define FONTSET_SIZE 80

typedef struct{
  uint8_t memory[memory_size];
  uint8_t V[register_count];
  uint16_t pc;
  uint16_t stack[stack_count];
  uint8_t sp;
  uint8_t delay_time;
  uint8_t sound_time;
  uint16_t I;
  uint8_t display[display_size];
  uint8_t keypad[keypad_size];
} Chip8;
const unsigned char fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};
void init_chip8(Chip8 *chip8){
  for (int a = 0; a < memory_size; a++){
    chip8->memory[a] = 0;
  }
  for (int a = 0; a < register_count; a++){
    chip8->V[a] = 0;
  }
  for (int a = 0; a < stack_count; a++){
    chip8->stack[a] = 0;
  }
  chip8->I = 0;
  chip8->pc = 0x200;
  chip8->sp = 0;
  chip8->delay_time = 0;
  chip8->sound_time = 0;
  for (int a = 0; a < display_size; a++){
    chip8->display[a] = 0;
  }
  for (int a = 0; a < keypad_size; a++){
    chip8->keypad[a] = 0;
  }
  for (int a = 0; a < FONTSET_SIZE; a++){
    chip8->memory[0x50 + a] = fontset[a];
  }
}
void updatetimer(Chip8 *chip8){
  if (chip8->delay_time > 0){
    chip8->delay_time--;
  }
  if (chip8->sound_time > 0){
    chip8->sound_time--;
  }
}
void updatekeypad(Chip8 *chip8){
  chip8->keypad[0x0] = IsKeyDown(KEY_X);
  chip8->keypad[0x1] = IsKeyDown(KEY_ONE);
  chip8->keypad[0x2] = IsKeyDown(KEY_TWO);
  chip8->keypad[0x3] = IsKeyDown(KEY_THREE);
  chip8->keypad[0x4] = IsKeyDown(KEY_Q);
  chip8->keypad[0x5] = IsKeyDown(KEY_W);
  chip8->keypad[0x6] = IsKeyDown(KEY_E);
  chip8->keypad[0x7] = IsKeyDown(KEY_A);
  chip8->keypad[0x8] = IsKeyDown(KEY_S);
  chip8->keypad[0x9] = IsKeyDown(KEY_D);
  chip8->keypad[0xA] = IsKeyDown(KEY_Z);
  chip8->keypad[0xB] = IsKeyDown(KEY_C);
  chip8->keypad[0xC] = IsKeyDown(KEY_FOUR);
  chip8->keypad[0xD] = IsKeyDown(KEY_R);
  chip8->keypad[0xE] = IsKeyDown(KEY_F);
  chip8->keypad[0xF] = IsKeyDown(KEY_V);
} 

void push(Chip8 *chip8,uint16_t address){
  chip8->stack[chip8->sp] = address;
  chip8->sp++;
}
uint16_t pop(Chip8 *chip8){
  chip8->sp--;
  return chip8->stack[chip8->sp];
}
void rom_loader(Chip8 *chip8,char filename[]){
  FILE *Fptr;
  
  Fptr = fopen(filename, "rb");
  size_t data_byte = fread(&chip8->memory[0x200],1,memory_size - 0x200,Fptr);
  fclose(Fptr);

}
void chip8_cycle(Chip8 *chip8){
  uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
 
  chip8->pc += 2;

  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t N = (opcode & 0x000F);
  uint8_t NN = (opcode & 0x00FF);
  uint16_t NNN = (opcode & 0x0FFF);

  switch (opcode & 0xF000) {
    case 0x0000:
      switch (opcode & 0x000F){
        case 0x0000 :
        {
          for (int i = 0; i < display_size; i++){
            chip8->display[i] = 0; 
          }
          break;
        }
        case 0x000E:
        {
          uint16_t return_address = pop(chip8);
          chip8->pc = return_address;
          break;
        }

      }
      break;
    case 0x3000:
    {
      //skip if vx == nn
      if (chip8->V[x] == NN){
        chip8->pc += 2;
      }
      break;
    }
    case 0x4000:
    {
      //skip if vx != nn
      if (chip8->V[x] != NN){
        chip8->pc += 2;
      }
      break;
    }
    case 0x5000:
    {
      //skip if vx == vy
      if (chip8->V[x] == chip8->V[y]){
        chip8->pc += 2;
      }
      break;
    }
    
    case 0x6000:
    {
      //vx = nn
      chip8->V[x] = NN;
      break;
    }
    case 0x7000:
    {
      //vx += nn
      chip8->V[x] += NN;
      break;
    }
    case 0x8000:
      switch (opcode & 0x000F) {
        case 0x0000:
        {
          //vx = vy
            chip8->V[x] = chip8->V[y];
            break;
        }
        case 0x0001 :
        {
          // vx |= vy
          chip8->V[x] |= chip8->V[y];
          break;
        }
        case 0x0002:
        {
          //vx and vy
            chip8->V[x] &= chip8->V[y];
          break;
        } 
        case 0x0003:
        {
          //vx xor vy
            chip8->V[x] ^= chip8->V[y];
          break;
        }
        case 0x0004:
        {
          //additon of vx and vy but carry flag store at vf
          uint16_t result = chip8->V[x] + chip8->V[y];
          chip8->V[0xF] = result > 255;
          chip8->V[x] = chip8->V[x] + chip8->V[y];
          break;
        }
        case 0x0005:
        {
          //subration with carry flag
          uint8_t vx = chip8->V[x];
          uint8_t vy = chip8->V[y];

          chip8->V[0xF] = vx >= vy;

          chip8->V[x] = vx - vy;
          break;  
        }
        case 0x0006:
        {
          chip8->V[0xF] = chip8->V[x] & 1;
          chip8->V[x] >>= 1;
          break;
        }
        case 0x0007:
        {
          //subration with carry flag
          uint8_t vx = chip8->V[x];
          uint8_t vy = chip8->V[y];

          chip8->V[0xF] = vy >= vx;

          chip8->V[x] = vy - vx;
          break;  

        }
        case 0x000E:
        {
          chip8->V[0xF] = (chip8->V[x] & 0x80) >> 7;
          chip8->V[x] <<= 1;
          break;
        }

      }
      break;
    case 0x1000:
    {
      chip8->pc = NNN;
      break;
    }
    case 0x9000:
    {
      //skip if vx != vy
      if (chip8->V[x] != chip8->V[y]){
        chip8->pc += 2;
      }
      break;
    }
    case 0x2000:
    {
      //call NNN
      push(chip8,chip8->pc);
      chip8->pc = NNN;
      break;
    }
    case 0xA000:
    {
      //INDEX REGISTER = NNN
      chip8->I = NNN;
      break;
    }
    case 0xB000:
    {
      //pc = nnn + v0
      chip8->pc = NNN + chip8->V[0];
      break;
    }
    case 0xC000:
    {
      //random 
      chip8->V[x] = (rand() & 0xFF) & NN;
      break;
    }
    case 0xD000:
    {
      uint8_t start_x = chip8->V[x];
      uint8_t start_y = chip8->V[y];

      chip8->V[0xF] = 0;

      for (uint8_t row = 0; row < N; row++){
         uint8_t sprite = chip8->memory[chip8->I + row];

         for (uint8_t col = 0; col < 8; col++){
           if (sprite & (0x80 >> col)){
             uint8_t px = (start_x + col) % 64;
             uint8_t py = (start_y + row) % 32;
            
             uint16_t index = py * 64 + px;

             if (chip8->display[index]){ 
               chip8->V[0xF] = 1;
             }
             chip8->display[index] ^= 1;
           }
         }
      }
      break;
    }
    case 0xE000:
    {
      //check if key is pressed or not 
      switch (opcode & 0x00FF){
        case 0x009E:
        {
          if (chip8->keypad[chip8->V[x]]){
            chip8->pc += 2;
          }
          break;
        }
        case 0x00A1:
        {
          //if key is not pressed
          if(!(chip8->keypad[chip8->V[x]])){
            chip8->pc += 2;
          }
          break;
        }
      }
      break;
    }
    case 0xF000:
    {
      switch (opcode & 0x00FF){
        case 0x0007:
        {
          //setting delay timer
          chip8->V[x] = chip8->delay_time;
          break;
        }
        case 0x000A:
        {
          //waititng till until the key is not pressed
          int keyispressed = -1;
          for (int i = 0; i < 16; i++){
            if (chip8->keypad[i]){
              keyispressed = i;
              break;
            }
          }
          if (keyispressed != -1){
            chip8->V[x] = keyispressed;
          }
          else {
            chip8->pc -= 2;
          }
          break;
        }
        case 0x0015:
        {
          //seeting dealy timer
          chip8->delay_time = chip8->V[x];
          break;
        }
        case 0x0018:
        {
          //seting dealy timer
          chip8->sound_time = chip8->V[x];
          break;
        }
        case 0x001E:
        {
          //increasing index regiseter
          chip8->I += chip8->V[x];
          break;
        }
        case 0x0029:
        {
          //idk
          chip8->I = 0x50 + (chip8->V[x] * 5);
          break;
        }
        case 0x0033:
        {
          uint8_t value = chip8->V[x];

          chip8->memory[chip8->I]     = value / 100;
          chip8->memory[chip8->I + 1] = (value / 10) % 10;
          chip8->memory[chip8->I + 2] = value % 10;

          break;
        }
        case 0x0055:
        {
          for (uint8_t i = 0; i  <= x; i++){
            chip8->memory[chip8->I + i] = chip8->V[i];
          }
          break;
        }
        case 0x0065:
        {
          for (uint8_t i = 0; i <= x; i++){
            chip8->V[i] = chip8->memory[chip8->I + i];
          }
          break;
        }
      }
      break;
    }

  }
}
void drawSprite(Chip8 *chip8){
    for (int i = 0; i < 32; i++){
      for (int j = 0; j < 64; j++){
        int index = i * 64 + j;
        if (chip8->display[index]){
          DrawRectangle(j * 10, i * 10, 10, 10, WHITE);
        }
      }
    }
  }
int main()
{
    Chip8 chip8;

    init_chip8(&chip8);

  
    rom_loader(&chip8, "Tetris [Fran Dachille, 1991].ch8");

    InitWindow(640, 320, "CHIP-8");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        updatekeypad(&chip8);

        for (int i = 0; i < 10; i++)
        {
            chip8_cycle(&chip8);
        }

        updatetimer(&chip8);

        BeginDrawing();

        ClearBackground(BLACK);
        drawSprite(&chip8);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
