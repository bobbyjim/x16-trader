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

#include "common.h"
#include "world.h"
#include "ship.h"
#include "trade.h"
#include "burtle_jsf32.h"

extern Starship ship;
extern World current, destination;
extern Cargo cargo[], starport[], temp[];
extern long hcr; // hundreds of credits

unsigned getHoldFree()
{  
   int i;
   unsigned hold = ship.component[ O_QSP_CARGOP ] * ship.size;
   for(i=0; i<MAX_CARGO_ITEMS; ++i)
      hold -= cargo[i].tons;

   return hold;
}

int  getModifierForCurrentWorld(char tradeComment, signed char mod)
{
//   cprintf("trade comment %c, dm %d\r\n", tradeComment, mod);
   switch(tradeComment)
   {
      case 'a': if (current.data.agricultural) return mod; break;
      case 'b': if (current.data.non_agri) return mod;     break;
      case 'r': if (current.data.rich) return mod;         break;
      case 'p': if (current.data.poor) return mod;         break;
      case 'i': if (current.data.industrial) return mod;   break;
      case 'j': if (current.data.non_ind) return mod;      break;
      default : return 0;
   }
}

byte modarray[] = { 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 17, 20, 30, 40 };

byte modifier(int dm)
{
   dm = diceRoll(2,dm) - 2; // translate to zero
   if (dm < 0)  dm = 0;
   if (dm > 13) dm = 13;
   return modarray[dm];
}

// void showTable()
// {
//    int i;
//    setBank(TRADE_MATRIX_BANK);
//    for(i=0; i<36; ++i)
//    {
//       CargoEntry* tmp = ((CargoEntry*)(0xa000 + 32 * i));
//       cprintf("%16s %5u %lu\r\n", 
//          tmp->label,
//          tmp->base,
//          (unsigned long)tmp->base * 100
//       );
//    }
// }

/*
   
      Calculate the local (starport) prices of goods.
      Do this once, when entering a system.

      Multiply by 10 to get the price in Cr.
      Divide by 10 to get the price in Hundreds of Cr.

 */
void trade_calculateMarketPrices()
{
   int i;
   CargoEntry* tmp;

   setBank(TRADE_MATRIX_BANK);
   tmp   = ((CargoEntry*)(0xa000 + 32 * (burtle32() % 36)));
   starport[0].cargoAddress = tmp;
   starport[0].tons = diceRoll(tmp->dice,0) * tmp->mult;

   for(i=0; i<MAX_CARGO_ITEMS; ++i)
   {
      int purchaseMod = getModifierForCurrentWorld( starport[i].cargoAddress->p00, starport[i].cargoAddress->p01 )
                      + getModifierForCurrentWorld( starport[i].cargoAddress->p10, starport[i].cargoAddress->p11 )
                      + getModifierForCurrentWorld( starport[i].cargoAddress->p20, starport[i].cargoAddress->p21 );

      starport[i].price = (unsigned long)starport[i].cargoAddress->base * (unsigned long)modifier( purchaseMod );
      cargo[i].price    = starport[i].price;
   }
}

void showInventory(byte sel)
{
   int i;
   long baseValue = 0;

   sel %= MAX_CARGO_ITEMS;

   cputsxy(8,1,"ship    cargo type        starport    cr/ton    base price\r\n");
   redline();
   cputs("\r\n");
   textcolor(COLOR_LIGHTBLUE);

   for(i=0; i<MAX_CARGO_ITEMS; ++i)
   {
      if (sel == i)
         revers(1);

      gotox(8);
      if (starport[i].cargoAddress > 0)
         cprintf("%-3u     %-15s   %-3u      %8ld0     %8u0\r\n\r\n", 
            cargo[i].tons, 
            starport[i].cargoAddress->label, 
            starport[i].tons, 
            starport[i].price,
            10 * starport[i].cargoAddress->base);
      else
         cprintf("%-3s     %-15s   %-3s     %8s             -\r\n\r\n", "-", "-", "-", "-" );

      revers(0);
   }

   cclear(50);
   gotox(8);
   cprintf("  %d tons free.    cr %ld00\n", getHoldFree(), hcr);
}

void buy(byte sel, byte amount)
{
   long price = (long)amount * starport[sel].price / 10;

   if (cargo[MAX_CARGO_ITEMS-1].cargoAddress > 0) return; // that's right, FIFO
   if (starport[sel].tons < amount) return;
   if (price > hcr) return;

   cargo[sel].cargoAddress = starport[sel].cargoAddress;
   cargo[sel].tons    += amount;
   starport[sel].tons -= amount;
   hcr -= price;
}

void sell(byte sel, byte amount)
{
   if (cargo[sel].tons < amount) return;

   cargo[sel].cargoAddress = starport[sel].cargoAddress;
   cargo[sel].tons    -= amount;
   starport[sel].tons += amount;
   hcr += (long)amount * starport[sel].price / 10;
}

//
//  shifts all cargos down one.
//
void cleanup()
{
   int i;
   for(i=MAX_CARGO_ITEMS-2; i>-1; --i)
   {
      cargo[i+1] = cargo[i];
      starport[i+1].cargoAddress = cargo[i+1].cargoAddress;
      starport[i+1].tons = 0;
   }
   cargo[0].tons = 0;
   cargo[0].cargoAddress = 0;

//   showInventory(0);
//   cgetc();
}


void trade_speculate() // from 'current' to 'destination'
{
   byte selected = 0;

   clrscr();
   toDefaultColor();

//   trade_calculateMarketPrices();

   while(1)
   {
      if (selected == 255) selected = MAX_CARGO_ITEMS-1;
      selected %= MAX_CARGO_ITEMS;
      //if (selected >  MAX_CARGO_ITEMS ) selected = 0;

      showInventory(selected);
      switch( cgetc() )
      {
         case 0x91: --selected; break;
         case 0x11: ++selected; break;
         case 0x9d: if (getHoldFree() > 0) 
                       buy( selected, 1 ); 
                    break;
         case 0x1d: sell( selected, 1 ); break;
         case 13:   cleanup(); return; // done
      }
   }
}

