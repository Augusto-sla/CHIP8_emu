#include <string.h>
#include <stdio.h>
#include "chip8.h"

int load_rom(struct Chip8* chip8, const char* filename)
{
	FILE* rom = fopen(filename, "rb");
	if (rom == NULL)
	{
		perror("couldn't open the rom");
		return -1;
	}
	
	fseek(rom, 0, SEEK_END);
	long rom_size = ftell(rom);
	if (rom_size > 4096 - 0x200)
	{
		puts("Rom bigger than expectated, maximum size: 3584 bytes");
		return -1;
	}
	rewind(rom);
	
	fread(&chip8->memory[0x200], 1, rom_size, rom);
	
	fclose(rom);

	return 0;
}

void chip8_cycle(struct Chip8* chip8)
{	
	uint8_t x, y, n, nn, nnn;
	uint16_t address;

	uint8_t byte1 = chip8->memory[chip8->pc];
	uint8_t byte2 = chip8->memory[chip8->pc + 1];
	
	uint16_t opcode = (byte1 << 8) | byte2;

	chip8->pc += 2;

	switch(opcode & 0xF000)
	{
		case 0x0000:
			if(opcode == 0x00E0){
				// CLC (clear screeno)
				memset(chip8->gfx, 0, sizeof(chip8->gfx));
				chip8->draw_flag = 1;
			} else if (opcode == 0x00EE) {
				// RET (return subroutine)
				chip8->sp--;
				chip8->pc = chip8->stack[chip8->sp];
			}
			break;

		case 0x1000:
			// 1NNN: jump to NNN
			address = opcode & 0x0FFF;
			chip8->pc = address;
			break;

		case 0x2000:
			// 2NNN: call subroutine
			chip8->stack[chip8->sp] = chip8->pc;
			chip8->sp++;
			chip8->pc = opcode & 0x0FFF;
			break;

		case 0x6000:
			// 6XNN: VX = NN
			x = (opcode & 0x0F00) >> 8;
			nn = opcode & 0x00FF;
			chip8->v[x] = nn;
			break;

		case 0x7000:
			// 7XNN: add value to register VX
			x = (opcode & 0x0F00) >> 8;
			nn = (opcode & 0x0FF);
			chip8->v[x] += nn; 
			break;

		case 0xA000:
			// ANNN: I = NNN
			chip8->i = opcode & 0x0FFF;
			break;

		case 0xD000:
			// DXYN: display/draw
			x = (opcode & 0x0F00) >> 8;
			y = (opcode & 0x00F0) >> 4; 
			n = (opcode & 0x000F);
			
			chip8->v[0xF] = 0;

			for (int line = 0; line < n; line++){
				uint8_t sprite = chip8->memory[chip8->i + line];
				for (int bit = 0; bit < 8; bit++){
					if (sprite & (0x80 >> bit)){
						int px = (chip8->v[x] + bit) % 64;
						int py = (chip8->v[y] + line) % 32;	
						int index = py * 64 + px;
						if (chip8->gfx[index] == 1) chip8->v[0xF] = 1;
						chip8->gfx[index] ^= 1;
						}
				}
			}

			chip8->draw_flag = 1;
			break;

		default:
			// unknow opcode
			break;
	}
}
