
#include <conio.h>
#include <string.h>

#include "common.h"
#include "jump-map.h"
#include "alarm.h"
#include "sprite.h"
#include "ship.h"
#include "world.h"

#define     HEXGRID_START       ((char*)(JUMP_GRID_ADDRESS))

typedef struct {

   unsigned char col;
   unsigned char row;
   char longlabel[5];
   char label[21];
   char starport;
   unsigned char buffer1[9];
   char zone;
   char allegiance[2];
   char bases;
   char bgg;
   unsigned char tcindex;
   int ancients:  1;
   int capital:   1;
   int hellworld: 1;
   int icecapped: 1;
   int military:  1;
   int reserve:   1;
   int research:  1;
   int satellite: 1;
   int siz       :4;
   int atm       :4;
   int hyd       :4;
   int pop       :4;
   int gov       :4;
   int law       :4;
   int tl        :4;
   int agricultural :1;
   int asteroid     :1;
   int barren       :1;
   int desert       :1;
   int fluid_seas   :1;
   int hi_pop       :1;
   int industrial   :1;
   int lo_pop       :1;
   int non_agri     :1;
   int poor         :1;
   int rich         :1;
   int vacuum       :1;
   int water_world  :1;
   unsigned char general_world_type;
   unsigned char buffer2[6];
   char star_class_1;
   int  star_num_1   :5;
   int  star_size_1  :3;
   char star_class_2;
   int  star_num_2   :5;
   int  star_size_2  :3;
   char star_class_3;
   int  star_num_3   :5;
   int  star_size_3  :3;

} WorldData;

void jumpmapInit()
{
   int i;

   for(i=4;i<60;++i)
       cclearxy(0,i,80);

   sprite_loadToVERA("bi-reticle2.bin", 0x5000);
}

extern Starship ship;     // used for setting a destination.
extern World current;     // here's where we are now.
extern World destination; // here's the destination we set.
extern SpriteDefinition jamison;

void jumpmapReticle()
{
   int col = 3 + destination.col - current.col;
   int row = 3 + destination.row - current.row;

   jamison.block =  0x5000;
   jamison.mode  =  SPRITE_MODE_8BPP;
   jamison.layer =  SPRITE_LAYER_1;
   jamison.dimensions = SPRITE_64_BY_64;
   jamison.x     = SPRITE_X_SCALE(130 + col * 72);
   jamison.y     = SPRITE_Y_SCALE( 64 + row * 64 - (col % 2) * 32);

   sprite_define(3, &jamison);

   gotoxy(0,15);
   cprintf("%02u%02u\r\n", current.col, current.row);
   cprintf("%02u%02u\r\n", destination.col, destination.row);
   cprintf("%u pc\r\n", parsecDistance(current.col, current.row, destination.col, destination.row));
   cprintf("%u j\r\n",  ship.component[O_QSP_J]);
}

void jumpmapDisableReticle()
{
   jamison.layer = SPRITE_DISABLED;
   sprite_define(3, &jamison);
}

unsigned char rangeIsOk(int dcol, int drow)
{
   return parsecDistance(current.col, current.row, destination.col + dcol, destination.row + drow) <= ship.component[O_QSP_J];
}

char jumpmapSetDestination()
{
   for(;;)
   {
      jumpmapReticle();
      switch(cgetc())
      {
         case 0x91:
            if (rangeIsOk(0,-1))
               --destination.row;
            break;

         case 0x11:
            if (rangeIsOk(0,1))
               ++destination.row;
            break;

         case 0x9d:
            if (rangeIsOk(-1,0))
               --destination.col;
            break;

         case 0x1d:
            if (rangeIsOk(1,0))
               ++destination.col;
            break;

         case 202: // shift-J
            getWorld(&destination);
            jumpmapDisableReticle();
            return 'j';

         case 13: // select
            getWorld(&destination);
            jumpmapDisableReticle();
            return 13;
      }
   }
}

void jumpmapShow()
{
    unsigned char x,y,z;
    char* address;
    char  tmp[67];
    tmp[66] = 0;

    jumpmapInit();

    textcolor(COLOR_LIGHTBLUE);
    setBank(MISC_BANK);

    for(y=0; y<7; ++y)
    {
       address = HEXGRID_START;
       z = 3+y*8;
       for(x=0;x<8;++x)
       {
          strncpy(tmp, address, 66);
          cputsxy(14,z+x+1,tmp);
          address += 77; // or whatever
       }      
    }
}

unsigned char worldColor[] = {
   COLOR_GRAY3,   COLOR_GREEN,      COLOR_WHITE,   COLOR_LIGHTRED, 
   COLOR_GRAY1,   COLOR_LIGHTGREEN, COLOR_PURPLE,  COLOR_ORANGE, 
   COLOR_GRAY2,   COLOR_BLUE,       COLOR_YELLOW,  COLOR_GRAY1, 
   COLOR_GRAY1,   COLOR_GRAY1,      COLOR_GRAY1,   COLOR_GRAY1
};

void jumpmapShowWorldData(unsigned char col, unsigned char row)
{
   int x,y;
   int r,c;
//   unsigned char spriteNum;
   unsigned char bank;
   unsigned int  offset;
   WorldData *world;
   char shortname[6];

//   spriteNum = 50; // I guess
   for(c=0; c<7; ++c)
      for(r=0; r<7; ++r)
      {
         offset = (-3+col+c) * 64 + (-3+row+r);
         bank = 10 + offset/128;
         offset %= 128;

         setBank(bank);
         world = (WorldData*)(0xa000 + offset * 64);

         if (world->starport >= 'a')
         {
            x = 20 + c * 9;
            y =  4 + r * 8;
            if (c%2==0) y += 4;

            textcolor(COLOR_WHITE);
            cputcxy(x,y,world->starport);

            textcolor(COLOR_LIGHTBLUE);
            if (world->bgg == 'b' || world->bgg == '2') 
               cputcxy(x-4,y+3,':');
            if (world->bgg == 'g' || world->bgg == '2')
               cputcxy(x+4,y+3,188);
            
            memset(shortname, 0, 6);
            strncpy(shortname, world->label+5, 5);
            textcolor(COLOR_WHITE);
            cputsxy(x-strlen(shortname)/3-1,y+1,shortname);

            textcolor(worldColor[world->general_world_type & 0x0f]);
            cputcxy(x,y+4,209);
         }
         // else
         // {
         //    sprite_disable(spriteNum);
         // }
         // ++spriteNum;
      }
}