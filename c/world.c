/*

    Traveller-Trader: a space trader game
    Copyright (C) 2021 Robert Eaglestone

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
#include "bank.h"
#include "world.h"
#include "trade.h"

// #include "burtle.h"

extern World localSystem[];
extern byte  localSystemCount;

void printWorld(World* worldptr)
{
   cputs(getLabel(worldptr));
   cputs("  ");
   printTradeCodes(worldptr->tcIndex);
   cprintf("\r\n\r\n");
}

char* getShortLabel(World* world)
{
   setBank(world->bank);
   return (char *) (world->record * 64 + O_LABEL);
}

char* getLabel(World* world)
{
   setBank(world->bank);
   return (char *) (0xa002 + world->record * 64);
}

void getWorld(World* world)
{
   int offset, bank, address;
   offset = world->col * 64 + world->row;
   bank = MAP_BANK_BEGIN + offset/128;
   setBank(bank);
   offset %= 128;
   address = ADDRESS_START + offset * 64;
 
   world->record   = offset;
   world->bank     = bank;

   world->alleg    = ALLEG(address);
   world->bases    = BASES(address);
   world->bgg      = BGG(address);   
   world->starport = STARPORT(address);
   world->pop      = POP(address);
   world->tcIndex  = TCINDEX(address);
   world->tc1      = TC1(address);
   world->tl       = TL(address);
   world->zone     = ZONE(address);
}

char* getBases(World* world)
{
   int address;
   address = ADDRESS_START + world->record * 64;

   switch(BASES(address))
   {
      case 'N': return "navy base";
      case 'S': return "scout base";
      case 'A': return "navy and scout base";
      case 'W': return "way station";
      case 'B': return "navy and way station";
      case 'D': return "navy depot";
   }
   return "none";
}

char hasGasGiants(World* world)
{
   int address;
   int bgg;
   address = ADDRESS_START + world->record * 64;
   bgg = BGG(address);

   if (bgg == 'g' || bgg == '2')
      return 'y';
   return 'n';
}

char hasBelts(World* world)
{
   int address;
   int bgg;
   address = ADDRESS_START + world->record * 64;
   bgg = BGG(address);

   if (bgg == 'b' || bgg == '2')
      return 'y';
   return 'n';
}

char* getZone(World* world)
{
   int address;
   address = ADDRESS_START + world->record * 64;

   switch(ZONE(address))
   {
      case 'A': return "amber";
      case 'R': return "red";
   }
   return "green";
}

byte getHexCol(World* world)
{
   return HEXCOL(ADDRESS_START + world->record * 64);
}

byte getHexRow(World* world)
{
   return HEXROW(ADDRESS_START + world->record * 64);
}

/*
   un-comment this when we have a burtle library to link in

void buildLocalSystem(World* mainworld)
{

   byte i;

   //
   // OK, the UWP is sufficient to randomize the local system.
   //
   burtleVal seed; // sufficiently gigantic

   // row and col are unique.  let's use that for now.
   // col is up to 256.
   seed = mainworld->row * 256 + mainworld->col;

   burtleInit(seed);
   localSystemCount = burtleRand() % 256;

   for(i=0; i<localSystemCount; ++i)
   { 
      SET_ORBIT(localSystem[i], burtleRand() % 256);
      SET_THETA(localSystem[i], burtleRand() % 256);

      localSystem[i].record = i;

      localSystem[i].starport = 'X';
      localSystem[i].siz      = twoD(-2);
      localSystem[i].atm      = goodFlux(localSystem[i].siz);
      localSystem[i].hyd      = goodFlux(localSystem[i].atm);
      localSystem[i].pop      = 0;
      localSystem[i].tl       = 0;

      localSystem[i].bgg      = ' ';
      localSystem[i].zone     = mainworld->zone;
      localSystem[i].bases    = ' ';
      localSystem[i].alleg    = ' ';
      localSystem[i].tcIndex  = mainworld->tcIndex;
      localSystem[i].tc1      = 0;
   }   
}
*/

