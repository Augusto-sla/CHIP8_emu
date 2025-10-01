#include<stdint.h>

#ifndef CHIP8_H
#define CHIP8_H

struct Chip8 {
	uint8_t memory[4096];
	uint8_t v[16];		// registers from V0 to VF
	uint8_t key[16];	// keyboard
	uint8_t  gfx[64 * 32];	// display
	uint8_t  draw_flag; 
	uint8_t  st;		// sound timer
	uint8_t  dt;		// delay timer
	uint16_t stack[16];
	uint16_t i;		// adress register
	uint16_t pc;		// program counter
	uint16_t sp;		// stack pointer
};

int load_rom(struct Chip8* chip8, const char* filename);
void chip8_cycle(struct Chip8* chip8);
void gfx(struct Chip8* chip8);
void chip8_init(struct Chip8* chip8);
#endif
