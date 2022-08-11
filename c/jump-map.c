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

#include <conio.h>
#include <string.h>

#include "common.h"
#include "jump-map.h"
#include "alarm.h"
#include "sprite.h"
#include "ship.h"
#include "world.h"
#include "bankedText.h"

#define     HEXGRID_START       ((char*)(PETSCII_JUMP_GRID))

extern Starship ship;     // used for setting a destination.
extern World current;     // here's where we are now.
extern World destination; // here's the destination we set.
extern SpriteDefinition jamison;
extern byte playerAchievementLevel;

void jumpmapInit()
{
   int i;

   for(i=4;i<60;++i)
       cclearxy(0,i,80);

   destination.col = current.col;
   destination.row = current.row;

   sprite_loadToVERA("bi-reticle2.bin", 0x5000);
}

void setZoneColor(char zone)
{
   switch(zone)
   {
      case 'a': 
         textcolor(COLOR_YELLOW); 
         break;
      case 'r': 
         textcolor(COLOR_LIGHTRED);
         break;
      default:
         textcolor(COLOR_WHITE);
         break;
   }
}

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

   if ((col % 2 == 0) && (destination.col % 2)) jamison.y -= SPRITE_Y_SCALE(56);

   sprite_define(3, &jamison);

   if (playerAchievementLevel > 1)
   {
      textcolor(COLOR_LIGHTBLUE);
      gotoxy(2,12);
      chline(15);
      gotoxy(0,13);
      cprintf("  cpos %02u%02u\r\n", current.col, current.row);
   
      if (destination.data.zone == 'a')
         textcolor(COLOR_YELLOW);
      if (destination.data.zone == 'r')
         textcolor(COLOR_LIGHTRED);
      cprintf("  dest %02u%02u %c\r\n", destination.col, destination.row, destination.data.zone);
      textcolor(COLOR_LIGHTBLUE);
      cprintf("  pc   %u\r\n", parsecDistance(current.col, current.row, destination.col, destination.row));
      cprintf("  jn   %u\r\n  ",  ship.component[O_QSP_J]);
      chline(15);
   }
   else
   {
      textcolor(COLOR_YELLOW);
      gotoxy(0,23);
      setBank(MISC_BANK);
      printBankedText(PETSCII_ASTROGATION_HELP, 79);
   }
}

void jumpmapDisableReticle()
{
   jamison.layer = SPRITE_DISABLED;
   sprite_define(3, &jamison);
}

unsigned char rangeIsOk(int dcol, int drow)
{
   unsigned distance = parsecDistance(current.col, current.row, destination.col + dcol, destination.row + drow);
   if (distance == 0) return 0;
   return distance <= ship.component[O_QSP_J];
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
            {
               --destination.row;
               getWorld(&destination);
            }
            break;

         case 0x11:
            if (rangeIsOk(0,1))
            {
               ++destination.row;
               getWorld(&destination);
            }
            break;

         case 0x9d:
            if (rangeIsOk(-1,0))
            {
               --destination.col;
               getWorld(&destination);
            }
            else if (rangeIsOk(-1,1))
            {
               --destination.col;
               ++destination.row;
               getWorld(&destination);
            }
            else if (rangeIsOk(-1,-1))
            {
               --destination.col;
               --destination.row;
               getWorld(&destination);
            }
            break;

         case 0x1d:
            if (rangeIsOk(1,0))
            {
               ++destination.col;
               getWorld(&destination);
            } 
            else if (rangeIsOk(1,1))
            {
               ++destination.col;
               ++destination.row;
               getWorld(&destination);
            }
            else if (rangeIsOk(1,-1))
            {
               ++destination.col;
               --destination.row;
               getWorld(&destination);
            }
            break;

         case 202: // shift-J
            if (HEX_HAS_WORLD(&destination))
            {
               getWorld(&destination);
               jumpmapDisableReticle();
               return 'j';
            }

         case 13: // select
            if (HEX_HAS_WORLD(&destination))
            {
               getWorld(&destination);
               jumpmapDisableReticle();
               return 13;
            }
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

            // if current world col is odd 
            // and c is even
            // add 4 to y
            if ((current.col % 2) && (c % 2 == 0))
               y += 4;

            // if current world col is even
            // and c is even
            // subtract 4 from y
            if ((current.col % 2 == 0) && (c % 2 == 0))
               y -= 4;
            
            setZoneColor(world->zone);
            cputcxy(x,y,world->starport);

            textcolor(COLOR_LIGHTBLUE);
            if (world->belt) 
               cputcxy(x-4,y+3,':');
            if (world->gg)
               cputcxy(x+4,y+3,188);
            
            memset(shortname, 0, 6);
            strncpy(shortname, world->name, 5);
            setZoneColor(world->zone);
            cputsxy(x-strlen(shortname)/3-1,y+1,shortname);

            textcolor(worldColor[world->general_world_type & 0x0f]);
            cputcxy(x,y+4,209);

            // DEBUG
            // gotoxy(x-1,y);
            // cprintf("%u,%u", world->col, world->row);
            // gotoxy(x-1,y);
            // cprintf("%u,%u", c, r);
            // gotoxy(x-1,y+1);
            // cprintf("%u,%u", current.col, current.row);
         }
         // else
         // {
         //    sprite_disable(spriteNum);
         // }
         // ++spriteNum;
      }
}

void jumpmapShow()
{
    unsigned char x,y,z;
    char* address;
    char  tmp[67];
    tmp[66] = 0;

    titleLine();
    printAlarmBar();
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

    jumpmapShowWorldData(current.col,current.row);
}
