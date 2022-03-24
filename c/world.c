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

byte  zoneColor[]  = {
   COLOR_GREEN,
   COLOR_YELLOW,
   COLOR_LIGHTRED,
   COLOR_GREEN
};

char* zoneString[] = {
   "green (safe)",
   "amber (warning)",
   "red (dangerous)"
};

char* worldAllegiance(byte code)
{
   switch(code)
   {
      case 'a': return "aslan";
      case 'd': return "droyne";
      case 'v': return "vargr";
      case 'z': return "zhodani";
      default : return "imperial";
   }
}

char* displayBases(char bases)
{
   switch(bases)
   {
      case 'n': return "navy";
      case 's': return "scout";
      case 'a': return "navy + scout";
      case 'b': return "navy + way station";
      case 'd': return "depot";
      case 'w': return "way station";
      default : return "none";
   }
}

char* systemStuff(World* world)
{
   if (WORLD_HAS_BELTS(world))
   {
      if (WORLD_HAS_GGS(world)) 
         return "belt and gas giant";
      else 
         return "belt";
   }

   if (WORLD_HAS_GGS(world)) return "gas giant";

   return "none";
}

void world_describe(World* world)
{
   textcolor(COLOR_LIGHTBLUE);

   cprintf("         world name      : %s (%s)\r\n\r\n", world->data.name, worldAllegiance(world->data.allegiance));

   if (playerAchievementLevel > 1)
      cprintf("         starport quality: %c - %-15s  bases: %s\r\n\r\n", world->data.starport, starportQuality(world->data.starport), displayBases(world->data.bases));

   if (playerAchievementLevel > 2)
   {
      if (WORLD_HAS_BELTS(world) || WORLD_HAS_GGS(world))
         cprintf("         system contents : %s\r\n\r\n", systemStuff(world)); 
   }

   if (playerAchievementLevel > 3)
   {
      textcolor(zoneColor[world->data.zone_digital]);
      cprintf("         zone            : %s\r\n\r\n",
         zoneString[world->data.zone_digital]
      );
      textcolor(COLOR_LIGHTBLUE);
   }

   if (playerAchievementLevel > 4)
   {
      cputs("         trade comments  : ");
      showTradeCodes(world);
   }

   cputs("\r\n\r\n");
}

#define     MU                   2
#define     MU2                  MU*2
#define     MU5                  MU*5
#define     WR                   30
#define     WR2                  20
#define     HALF_WR              15
#define     WC                   80
#define     WC2                  70
#define     WRXWC                2400

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

byte world_map_calculated = 0;

void calculateWorldmap(byte streaky)
{
   int i, j, x, y;

   for(i=0; i<MU5; ++i)
   {
      x = (rand() % WC2) + 5;
      y = (rand() % WR2) + 5;
      for(j=0; j<(rand() % WRXWC)/MU2; ++j)
      {
         x += 1 * ((rand() % 3) - 1) + streaky;
         y += 1 * ((rand() % 3) - 1) + streaky;
         if (x>=0 && x<=WC && y>=0 && y<=WR && (worldmap[x][y] < 20))
            ++worldmap[x][y];
      }
   }

   world_map_calculated = 1;
}

void drawWorld(byte streaky, byte variance)
{
   int i, j;

   //if (world_map_calculated == 0)
   calculateWorldmap(streaky);

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

          //if (j == HALF_WR && worldmap[i][j] == 0)
          //{
          //  textcolor(mapColorPalette[0]);
          //  cputcxy(i,j,'x');
          //  // 195 is nice, especially with dark blue.
          //}
          //else
          //{
            textcolor(mapColorPalette[worldmap[i][j] + rand() % variance]);
            if (worldmap[i][j] == 0)
               cputcxy(i,j,166);
            else
               cputcxy(i,j,'x');
            // 166 is too bold.
            // 214 isn't bad.
          //}
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

byte planetoidPetscii[4][7] = {
   {  18,  169, 32, 32, 32, 223,  146 },     // rvs incline ... decline rvs_off
   {  18,  32, 215, 32, 32, 32,   146 },     // rvs . circle ... rvs_off
   {  18,  32,  32, 32, 213, 201, 146 },     // rvs ... arc1 arc2 rvs_off
   { 223,  18,  32, 32, 202, 146, 169 } // decline rvs .. arc3 rvs_off incline
};

void world_drawPlanetoid(byte x, byte y)
{
   textcolor(COLOR_GRAY1);
   revers(1);
   gotoxy(x,y);
   cprintf("%c   %c", 169, 223);
   gotoxy(x,y+1);
   cprintf(" %c   ",  215);
   gotoxy(x,y+2);
   cprintf("   %c%c", 213, 201);
   
   revers(0);
   gotoxy(x,y+3);
   cputc(223);
   revers(1);
   cprintf("  %c", 202);
   revers(0);
   cputc(169);
}

