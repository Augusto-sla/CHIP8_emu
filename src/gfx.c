#include <stdio.h>
#include <stdlib.h> 
#include "chip8.h"

void gfx(struct Chip8* chip8)
{
	system("clear");
	for (int y = 0; y < 32; y++){
		for (int x = 0; x < 64; x++){
			int index = y * 64 + x;
			printf(chip8->gfx[index] ? "#" : " ");
		}	
		printf("\n");
	}
}
