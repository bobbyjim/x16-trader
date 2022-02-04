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

#include <peekpoke.h>
#include <conio.h>
#include <cbm.h>

#include "common.h"
//#include "bank.h"
#include "world.h"
#include "trade.h"
#include "hold.h"

extern World current, destination;
extern Cargo cargo[], starport[], temp[];
extern long hcr; // hundreds of credits
extern word hold;

byte getBasePrice(byte tradeIndex)
{
   setBank(TRADE_MATRIX_BANK);
   return PEEK(0xb000 + tradeIndex * 32);
}

char* getTradeCodes(/*char* toBuffer,*/ byte tradeIndex)
{
   setBank(TRADE_MATRIX_BANK);
   return (char*) 0xb001 + tradeIndex * 32; 
   //bankgets(toBuffer, 15, 0xb001 + tradeIndex * 32);
}

char* getTradeGoods(/*char* toBuffer,*/ byte tradeIndex)
{
   setBank(TRADE_MATRIX_BANK);
   return (char*) 0xb010 + tradeIndex * 32; 
   //bankgets(toBuffer, 15, 0xb010 + tradeIndex * 32);
}

void printTradeCodes(byte tradeIndex)
{
   char* address = (char*)(0xb001 + tradeIndex * 32);
   setBank(TRADE_MATRIX_BANK);
   while(PEEK(address) > 0)
   { 
	cputc(address[0]);
	++address;
   }
}

void printTradeGoods(byte tradeIndex)
{
   char* address = (char*)0xb010 + tradeIndex * 32;
   setBank(TRADE_MATRIX_BANK);
   while(PEEK(address) > 0)
   { 
	cputc(address[0]);
	++address;
   }
}

byte getMarketPrice(byte sourceIndex, byte marketIndex)
{
   setBank(TRADE_MATRIX_BANK);
   return PEEK(0xa000 + sourceIndex * 64 + marketIndex);
}

void showCredits()
{
   cputsxy(5,55,"cr ");
   gotoxy(10,55);
   cprintf("%ld00    ", hcr);
}

void showHoldFree(word holdFree)
{
   gotoxy(10,50);
   cprintf("%u tons free    ", holdFree);
}

void cleanUpCargo()
{
   byte i;
   byte j=1;

   for(i=0; i<19; ++i)
      if (cargo[i].tons > 0)
      {
         temp[j].index = cargo[i].index;
         temp[j].tl    = cargo[i].tl;
         temp[j].tons  = cargo[i].tons;
	 ++j;
         cargo[i].index = 0;
	 cargo[i].tl    = 0;
	 cargo[i].tons  = 0;
      }

   for(i=1; i<j; ++i)
   {
      cargo[i].index = temp[i].index;
      cargo[i].tl    = temp[i].tl;
      cargo[i].tons  = temp[i].tons;
   }
   cargo[0].index = 0;
   cargo[0].tl    = 0;
   cargo[0].tons  = 0;
}

void cleanUpStarport()
{
   byte i;
   for(i=0; i<20; ++i)
   {
      starport[i].index = 0;   
      starport[i].tl    = 0;
      starport[i].tons  = 0; 
   }
}

void moveCargo() // from 'current' to 'destination'
{
   int price;
   int matrixValue;
   byte i;
   word holdFree;

   byte sel = 0;
   word selPrice = 0;

   clrscr();
   toDefaultColor();

   printWorld(&current);
   cprintf("this world is selling tl-%u ", current.tl);
   printTradeGoods(current.tcIndex);
   price = 100 * (getBasePrice(current.tcIndex) + current.tl);
   cprintf(" at %u cr/ton", price);
   cprintf("\r\n\r\n");

   printWorld(&destination);
   setBank(TRADE_MATRIX_BANK);
   matrixValue = getMarketPrice(current.tcIndex, destination.tcIndex);
   cprintf("destination sale value: %u\n", (matrixValue + current.tl - destination.tl) * 100);

   cprintf("\r\n");

   starport[0].index = current.tcIndex;
   starport[0].tons  = 100;
   starport[0].tl    = current.tl;
   
   cputsxy(8,12,"ship    cargo type        starport     cr/ton       dest.price\r\n");
   redline();
   toDefaultColor();
   while(1)
   {
      gotoxy(0,14);
      holdFree = hold;
      for(i=0;i<20;++i)
      {
         holdFree -= cargo[i].tons;
         cputs("        ");
         if(sel == i)
            revers(1);

         if(i==0)
         {
            cprintf("%4u    %-15s    %4u         %4u        %4u00\r\n\r\n",
	         cargo[0].tons,
	         getTradeGoods(starport[0].index),
	         starport[0].tons,
	         price,
                 getMarketPrice(starport[0].index, destination.tcIndex) + starport[i].tl - destination.tl);
         }
         else if(cargo[i].tons>0 || starport[i].tons>0)
         {
            selPrice = getMarketPrice(cargo[i].index, current.tcIndex) + cargo[i].tl - current.tl;
	    cprintf("%4u    %-15s    %4u       %4u00        %4u00\r\n\r\n",
         	   cargo[i].tons,
	           getTradeGoods(cargo[i].index),
         	   starport[i].tons,
                   selPrice,
                   getMarketPrice(cargo[i].index, destination.tcIndex) + cargo[i].tl - destination.tl);
         }
         else
         {
            cprintf("\r\n\r\n");
         }

         if(sel == i)
            revers(0);
      }
      showHoldFree(holdFree);
      switch(cgetc())
      {
         case 0x91:
         case 'i': if (sel > 0) --sel; break;
         case 0x11:
         case 'k': if (sel < 20) ++sel; break;
         case 0x9d:
                if (holdFree == 0) break;
		if (starport[sel].tons == 0) break;
                if (hcr < selPrice) break;
                cargo[sel].index = starport[sel].index;
                cargo[sel].tl    = starport[sel].tl;
		++cargo[sel].tons; 
		--starport[sel].tons; 
                hcr -= selPrice;
                showCredits();
		break;
         case 0x1d:
		if (cargo[sel].tons == 0) break;
                starport[sel].index = cargo[sel].index;
                starport[sel].tl    = cargo[sel].tl;
		--cargo[sel].tons; 
		++starport[sel].tons; 
                hcr += selPrice;
                showCredits();
		break;
         case 'j': 
                if (holdFree < 10) break;
		if (starport[sel].tons < 10) break;
                if (hcr < 10 * selPrice) break;
                cargo[sel].index = starport[sel].index;
                cargo[sel].tl    = starport[sel].tl;
		cargo[sel].tons += 10; 
		starport[sel].tons -= 10; 
                hcr -= 10 * selPrice;
                showCredits();
		break;
         case 'l': 
		if (cargo[sel].tons < 10) break;
                starport[sel].index = cargo[sel].index;
                starport[sel].tl    = cargo[sel].tl;
		cargo[sel].tons -= 10; 
		starport[sel].tons += 10; 
                hcr += 10 * selPrice;
                showCredits();
		break;
	 case 13: 
                cleanUpCargo();
    		cleanUpStarport();
		return;
      }
   }
}

