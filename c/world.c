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
#include <stdlib.h>

#include "common.h"
#include "world.h"
#include "trade.h"

extern World localSystem[];
extern byte  localSystemCount;
extern byte  playerAchievementLevel;

char *stellarSizeMap[] = { "h", "i", "ii", "iii", "iv", "v", "d", "d" };

void printWorld(World* worldptr)
{
   cputs(WORLD_LONG_LABEL(worldptr));
   cputs("  ");
   showTradeCodes(worldptr);
   cprintf("\r\n\r\n");
}

void showTradeCodes(World* world)
{
   if (world->data.agricultural) cputs("ag ");
   if (world->data.ancients    ) cputs("an ");
   if (world->data.asteroid    ) cputs("as ");
   if (world->data.barren      ) cputs("ba ");
   if (world->data.capital     ) cputs("cp ");
   if (world->data.desert      ) cputs("de ");
   if (world->data.fluid_seas  ) cputs("fl ");
   if (world->data.hellworld   ) cputs("he ");
   if (world->data.hi_pop      ) cputs("hi ");
   if (world->data.icecapped   ) cputs("ic ");
   if (world->data.industrial  ) cputs("in ");
   if (world->data.lo_pop      ) cputs("lo ");
   if (world->data.military    ) cputs("mr ");
   if (world->data.non_agri    ) cputs("na ");
   if (world->data.non_ind     ) cputs("ni ");
   if (world->data.poor        ) cputs("po ");
   if (world->data.reserve     ) cputs("re ");
   if (world->data.rich        ) cputs("ri ");
   if (world->data.research    ) cputs("rs ");
   if (world->data.satellite   ) cputs("sa ");
   if (world->data.vacuum      ) cputs("va ");
   if (world->data.water_world ) cputs("wa ");
}

char* starportQuality(char letter)
{
   switch(letter)
   {
      case 'a': return "excellent";
      case 'b': return "good";
      case 'c': return "fair";
      case 'd': return "poor";
      case 'e': return "barely there";
      case 'x': return "non-existent";
      default:  return "unknown!";
   }
}

char* zoneString[] = {
   "green (safe)",
   "amber (warning)",
   "red (dangerous)"
};

void world_describe(World* world)
{
   textcolor(COLOR_LIGHTBLUE);

   cprintf("         world name      : %s\r\n\r\n", world->data.name);

   if (playerAchievementLevel > 1)
      cprintf("         starport quality: %c (%s)\r\n\r\n", world->data.starport, starportQuality(world->data.starport));

   if (playerAchievementLevel > 2)
   {
      cprintf("         gas giant       : %s\r\n\r\n", 
         WORLD_HAS_GGS(world)? "yes" : "no"
      );
      cputs("         trade comments  : ");
      showTradeCodes(world);
   }

   if (playerAchievementLevel > 3)
   {
      cprintf("\r\n\r\n         zone            : %s\r\n\r\n",
         zoneString[world->data.zone_digital]
      );
   }
   cputs("\r\n\r\n");
}

#define     MU                   2
#define     MU2                  MU*2
#define     MU5                  MU*5
#define     WR                   30
#define     HALF_WR              15
#define     WC                   80

unsigned char worldmap[WC][WR];
unsigned char worldmapColor[] = 
{
   COLOR_BLUE,
   COLOR_CYAN,    COLOR_CYAN,    COLOR_CYAN,
   COLOR_YELLOW,  COLOR_YELLOW,  COLOR_YELLOW, 
   COLOR_GREEN,   COLOR_GREEN,   COLOR_GREEN,   COLOR_GREEN,   COLOR_GREEN,
   COLOR_LIGHTGREEN, COLOR_LIGHTGREEN,
   COLOR_GRAY3,   COLOR_GRAY3,   COLOR_GRAY3,   COLOR_GRAY3,   
   COLOR_GRAY2,   COLOR_GRAY2,
   COLOR_GRAY3,   COLOR_GRAY3
};

unsigned char gasgiantMapColor[] = 
{
   COLOR_BLUE, COLOR_BLUE,
   COLOR_PURPLE, COLOR_PURPLE,
   COLOR_BLUE, COLOR_BLUE, COLOR_BLUE, COLOR_BLUE,
   COLOR_RED,
   COLOR_RED,    COLOR_LIGHTRED, COLOR_LIGHTRED,
   COLOR_ORANGE, COLOR_YELLOW, COLOR_ORANGE,
   COLOR_RED, COLOR_ORANGE, COLOR_ORANGE, COLOR_ORANGE, COLOR_ORANGE,
   COLOR_WHITE, COLOR_BLACK
};

unsigned char* mapColorPalette = worldmapColor;

void world_setGasGiantPalette() { mapColorPalette = gasgiantMapColor; }
void world_setRockballPalette() { mapColorPalette = worldmapColor; }

void drawWorld(byte streaky, byte variance)
{
   int i, j, x, y;

   for(i=0; i<MU5; ++i)
   {
      x = (rand() % (WC-10)) + 5;
      y = (rand() % (WR-10)) + 5;
      for(j=0; j<(rand() % (WR * WC))/MU2; ++j)
      {
         x += 1 * ((rand() % 3) - 1) + streaky;
         y += 1 * ((rand() % 3) - 1) + streaky;
         if (x>=0 && x<=WC && y>=0 && y<=WR && (worldmap[x][y] < 20))
            ++worldmap[x][y];
      }
   }

   for(j=4; j<WR; ++j)
   {
      int j1 = j+10;   // translate to center
      int j2 = j1 * j1;

      for(i=0; i<WC; ++i)
      {
          int i1 = i-40;   // translate to center
          int r2 = j2 + i1 * i1; // r^2
          //
          //  if the computed radius is outside the circle, skip it.
          //  (the circle's radius is 40 of course)
          //
          if (r2 > 1600) continue;

          if (j == HALF_WR && worldmap[i][j] == 0)
          {
            textcolor(mapColorPalette[0]);
            cputcxy(i,j,'x');
            // 195 is nice, especially with dark blue.
          }
          else
          {
            textcolor(mapColorPalette[worldmap[i][j] + rand() % variance]);
            if (worldmap[i][j] == 0)
               cputcxy(i,j,166);
            else
               cputcxy(i,j,'x');
            // 166 is too bold.
            // 214 isn't bad.
          }
      }
   }
}

void getWorld(World* world)
{
   int offset = world->col * 64 + world->row;
   int bank = MAP_BANK_BEGIN + offset/128;

   int address;
   WorldData* data;

   setBank(bank);
   offset %= 128;
   address = ADDRESS_START + offset * 64;
 
   world->bank     = bank;
   world->record   = offset;
   world->address  = address;

   data = (WorldData*)(address);
   world->data     = *data;           // COPY THE DATA IN!
}
