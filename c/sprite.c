#include <cbm.h>
#include <cx16.h>

#include "sprite.h"

// x and y are 15 bit uint's so we can toy with them just a bit.
#define     SPRITE_XH(x)             ((x>>5) & 0xff)
#define     SPRITE_XL(x)             (x>>13)
#define     SPRITE_YH(y)             ((y>>5) & 0xff)
#define     SPRITE_YL(y)             (y>>13)

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
