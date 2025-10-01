#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"



int main(int argc, char** argv)
{
	struct Chip8 chip8;

	if (argc < 2){
		puts("please insert the rom");
		return 1;
	}	

	chip8_init(&chip8);

	if (load_rom(&chip8, argv[1]) != 0){
		return 1;
	}
	

	while(1){
		chip8_cycle(&chip8);
		if (chip8.draw_flag){
			gfx(&chip8);
			chip8.draw_flag = 0;
		}
	}
	
	return 0;

}
	

void chip8_init(struct Chip8* chip8)
{
	memset(chip8->memory, 0, sizeof(chip8->memory));
	memset(chip8->v, 0, sizeof(chip8->v));
	memset(chip8->gfx, 0, sizeof(chip8->gfx));
	memset(chip8->stack, 0, sizeof(chip8->stack));
	memset(chip8->key, 0, sizeof(chip8->key));
	chip8->st = 0;
    	chip8->dt = 0;
    	chip8->pc = 0x200;
    	chip8->i = 0;
    	chip8->sp = 0;
    	chip8->draw_flag = 0;
}
