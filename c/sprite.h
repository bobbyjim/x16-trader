/*

    Traveller-Trader: a space trader game
    Copyright (C) 2022 Robert Eaglestone

    This file is part of Traveller-Trader.
        
    Traveller-Trader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
        
    Traveller-Trader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Traveller-Trader.  If not, see <https://www.gnu.org/licenses/>.
        
*/    
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <stdint.h>

#define		SPRITE_MODE_8BPP			128
#define		SPRITE_MODE_4BPP			0

//
//  64 x 64 is bits     1111xxxx
//  64 x  8 is bits     0011xxxx
//  32 x 32 is bits     1010xxxx
//  16 x 16 is bits     0101xxxx
//  16 x  8 is bits     0001xxxx
//   8 x  8 is bits     0000xxxx
//
#define 	SPRITE_8_BY_8				  0
#define		SPRITE_8_BY_16				( 0 + 64 )
#define		SPRITE_8_BY_32				( 0 + 128)
#define		SPRITE_8_BY_64				( 0 + 192)
#define		SPRITE_16_BY_8				(16 + 0  )
#define		SPRITE_16_BY_16				(16 + 64 )
#define		SPRITE_16_BY_32				(16 + 128)
#define		SPRITE_16_BY_64				(16 + 192)
#define		SPRITE_32_BY_8				(32 + 0  )
#define		SPRITE_32_BY_16				(32 + 64 )
#define		SPRITE_32_BY_32				(32 + 128)
#define		SPRITE_32_BY_64				(32 + 192)
#define		SPRITE_64_BY_8				(48 + 0  )
#define		SPRITE_64_BY_16				(48 + 64 )
#define		SPRITE_64_BY_32				(48 + 128)
#define		SPRITE_64_BY_64				(48 + 192)

#define		SPRITE_DISABLED				 0
#define	    SPRITE_LAYER_BACKGROUND		(1 << 2)
#define		SPRITE_LAYER_0				(2 << 2)
#define		SPRITE_LAYER_1				(3 << 2)

#define	    SPRITE_SET_IMAGE	        0xFEF0
#define     SPRITE_MOVE		            0xFEF3
#define     SPRITE_X_SCALE(x)           (x<<5)
#define     SPRITE_Y_SCALE(y)           (y<<5)

#define		VERA_SPRITE_AREA_BOTTOM		0x4000
#define		VERA_SPRITE_AREA_TOP		0xcfff

//
//  Stuff you shouldn't care about
//
#define		SPRITE_REGISTER_OFFSET(spritenum)	(spritenum << 3)
#define 	SPRITE_REGISTERS(spritenum)	((spritenum << 3) + 0xfc00)
#define     SPRITE_BLOCK_HI(addr)      ((addr >> 13) & 0x1f)
#define     SPRITE_BLOCK_LO(addr)      ((addr >> 5) & 0xff)

//
//  A note on positions:
//  15 bits is an abuse, unless you use it fractionally.
//  Let's shift it by 5 bits before using it.
//
typedef struct {
	uint16_t block; 
	uint8_t  mode;
    uint8_t  collision_mask;
    uint8_t  layer;
	uint8_t  dimensions;      
	int8_t   palette_offset;
	int      x          :15;
	int		 flip_horiz :1;
	int      y          :15;
	int    	 flip_vert  :1;
	int		 dx;
	int		 dy;
} SpriteDefinition;

void sprite_loadToVERA(char *filename, uint16_t address);
				 
void sprite_define(uint8_t spritenum, SpriteDefinition *sprdef);
//void abi_sprite_set_image(uint8_t spritenum, SpriteDefinition *sprdef);

void sprite_pos(uint8_t spritenum, SpriteDefinition *pos);
//void abi_sprite_set_position(uint8_t spritenum, SpriteDefinition *sprdef);

void sprite_changeBlock(uint8_t spritenum, SpriteDefinition *sprdef);
void sprite_flip(uint8_t spritenum, SpriteDefinition* pos);
void sprite_refresh();
void sprite_disable(uint8_t spriteNum);

#endif
