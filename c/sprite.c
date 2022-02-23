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
#include <cbm.h>
#include <cx16.h>
#include <6502.h>

#include "sprite.h"


#define     ABI_SPRITE_SET_IMAGE     0xfef0
#define     ABI_SPRITE_SET_POSITION  0xfef3
#define     ABI_R0                   (*(unsigned *)0x02)
#define     ABI_R1                   (*(unsigned *)0x04)
#define     ABI_R2                   (*(unsigned *)0x06)
#define     ABI_R2L                  (*(unsigned char*)0x06)
#define     ABI_R3                   (*(unsigned *)0x08)
#define     ABI_R4                   (*(unsigned *)0x0a)
#define     ABI_R5                   (*(unsigned *)0x0c)
#define     ABI_R6                   (*(unsigned *)0x0e)
#define     ABI_R7                   (*(unsigned *)0x10)
#define     ABI_R8                   (*(unsigned *)0x12)
#define     ABI_R9                   (*(unsigned *)0x14)
#define     ABI_R10                  (*(unsigned *)0x16)
#define     ABI_R11                  (*(unsigned *)0x18)
#define     ABI_R12                  (*(unsigned *)0x1a)
#define     ABI_R13                  (*(unsigned *)0x1c)
#define     ABI_R14                  (*(unsigned *)0x1e)

// x and y are 15 bit uint's so we can toy with them just a bit.
#define     SPRITE_XH(x)             ((x>>5) & 0xff)
#define     SPRITE_XL(x)             (x>>13)
#define     SPRITE_YH(y)             ((y>>5) & 0xff)
#define     SPRITE_YL(y)             (y>>13)

struct regs testregs;

void sprite_loadToVERA(char *filename, uint16_t address)
{
   cbm_k_setnam(filename);
   cbm_k_setlfs(0,8,0);
   cbm_k_load(2, address); // 2 = VERA
}

/*
 *  Copies 1K of the *** current *** RAM bank into VERA starting at 1FC00. 
 */
void sprite_refresh()
{
   unsigned char* sourceAddress = ((unsigned char *)0xa000);
   //
   //  Set Port 0 to point to sprite register
   //
   VERA.control = 0; // port 0
   VERA.address = 0xfc00;
   VERA.address_hi = VERA_INC_1 + 1; // from cx16.h

   //
   //  Copy over 1K from the bank to VERA
   //
   while(sourceAddress < ((unsigned char *)0xa400))
      VERA.data0 = *sourceAddress++;
}

// void abi_sprite_set_image(uint8_t spritenum, SpriteDefinition *sprdef)
// {
//    testregs.a  = spritenum;
//    testregs.x  = sprdef->x >> 5;
//    testregs.y  = sprdef->y >> 5;
//    ABI_R0      = sprdef->block;
//    ABI_R2L     = sprdef->mode;   // bpp
//    testregs.flags = 0;
//    testregs.pc = ABI_SPRITE_SET_IMAGE;
//    _sys(&testregs);
// }

void sprite_define(uint8_t spritenum, SpriteDefinition *sprdef)
{
   //
   //  Set Port 0 to point to sprite register
   //
   VERA.control = 0; // port 0
   VERA.address = SPRITE_REGISTERS(spritenum);
   VERA.address_hi = VERA_INC_1 + 1; // from cx16.h

   VERA.data0 = SPRITE_BLOCK_LO(sprdef->block); // lower 8 bits here
   VERA.data0 = sprdef->mode + SPRITE_BLOCK_HI(sprdef->block);  // upper 4 bits in the lower nybble here
   VERA.data0 = SPRITE_XH(sprdef->x);
   VERA.data0 = SPRITE_XL(sprdef->x);
   VERA.data0 = SPRITE_YH(sprdef->y);
   VERA.data0 = SPRITE_YL(sprdef->y);
   VERA.data0 = sprdef->layer + (sprdef->flip_vert << 1) + sprdef->flip_horiz;     // leave collision mask alone for now
   VERA.data0 = sprdef->dimensions + sprdef->palette_offset;
}

// void abi_sprite_set_position(uint8_t spritenum, SpriteDefinition *sprdef)
// {
//    testregs.a = spritenum;
//    testregs.x = sprdef->x >> 5;
//    testregs.y = sprdef->y >> 5;
//    testregs.flags = 0;
//    testregs.pc = ABI_SPRITE_SET_POSITION;
//    _sys(&testregs);
// }

void sprite_pos(uint8_t spritenum, SpriteDefinition* pos)
{
   //
   //  Set Port 0 to point to sprite x,y registers
   //
   VERA.control = 0; // port 0
   VERA.address = SPRITE_REGISTERS(spritenum) + 2; // x
   VERA.address_hi = VERA_INC_1 + 1;

   VERA.data0 = SPRITE_XH(pos->x);
   VERA.data0 = SPRITE_XL(pos->x);
   VERA.data0 = SPRITE_YH(pos->y);
   VERA.data0 = SPRITE_YL(pos->y);
   VERA.data0 = pos->layer + (pos->flip_vert << 1) + pos->flip_horiz;
}

void sprite_flip(uint8_t spritenum, SpriteDefinition* pos)
{
   //
   //  Set Port 0 to point to sprite x,y registers
   //
   VERA.control = 0; // port 0
   VERA.address = SPRITE_REGISTERS(spritenum) + 6; // flip
   VERA.address_hi = 1;
   VERA.data0 = pos->layer + (pos->flip_vert << 1) + pos->flip_horiz;
}

void sprite_changeBlock(uint8_t spritenum, SpriteDefinition *sprdef)
{
   //
   //  Set Port 0 to point to sprite register
   //
   VERA.control = 0; // port 0
   VERA.address = SPRITE_REGISTERS(spritenum);
   VERA.address_hi = VERA_INC_1 + 1; // from cx16.h

   VERA.data0 = SPRITE_BLOCK_LO(sprdef->block); // lower 8 bits here
   VERA.data0 = sprdef->mode + SPRITE_BLOCK_HI(sprdef->block);  // upper 4 bits in the lower nybble here
}

void sprite_disable(uint8_t spritenum)
{
   //
   //  Set Port 0 to point to sprite x,y registers
   //
   VERA.control = 0; // port 0
   VERA.address = SPRITE_REGISTERS(spritenum) + 6; // layer
   VERA.address_hi = 1;
   VERA.data0 = 0;
}
