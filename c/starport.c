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

#include "starport.h"
#include "ship.h"
#include "common.h"
#include "alarm.h"
#include "emplacement.h"
#include "panel.h"
#include "bankedText.h"

extern Starship   ship;
extern long       hcr;
extern byte       shipState[24];
extern byte       shipDamage[24];

//extern char* componentLabel[];  <-- #define TEXT_SHIP_COMPONENT(x)  ((char*) 0xa000 + x * 16)
extern unsigned char cost[];

#define  MENU_PANEL_ROW      16
#define  MESSAGE_ROW         56

void checkComponentSelection(byte selected)
{
   unsigned kcr = cost[selected];

   cclearxy(0,MESSAGE_ROW,70);
   cclearxy(0,MESSAGE_ROW+1,70);
   cclearxy(0,MESSAGE_ROW+2,70);
   if (kcr > 0)
   {
      gotoxy(0,MESSAGE_ROW);
      cprintf("    cr %u000 to repair this part.\r\n", kcr);
      if (kcr * 10 > hcr)
      {
         cprintf("    you lack the funds to repair it.\r\n");
      }
      else
      {
         cprintf("    press <r> to repair this part.\r\n");
      }
   } 
}

void showComponentStatus(byte selected)
{
   byte i;
   byte mult;

   setBank(TEXT_BANK_1);

   textcolor(COLOR_LIGHTBLUE);
   cputsxy(7,MENU_PANEL_ROW+2,"component        status   rating  current  value (mcr)\r\n" );
   cputsxy(7,MENU_PANEL_ROW+3,"---------------  -------  ------  -------  -----------\r\n" );
   textcolor(COLOR_CYAN); // LIGHTBLUE);
   for(i=0; i<COMPONENT_COUNT; ++i)
   {
      gotoxy(7,MENU_PANEL_ROW+4+i);
      if (selected == i) revers(1);

      mult = 1;
      if (i == O_QSP_CARGOP || i == O_QSP_FUELP) mult = ship.size;

      if (i < FIRST_EMPLACEMENT)
      {
         printBankedText( (unsigned) TEXT_SHIP_COMPONENT(i), 15 );
         if (ship.component[i] == 0)
            cprintf("     -\r\n");
         else
            cprintf("   %-6s   %-4d    %-4d   \r\n",
               getStatusLabel(shipState[i]),
               ship.component[i] * mult,
               (ship.component[i] - shipState[i]) * mult
            );
      }
      else
      {
         if (ship.component[i] == 0)
            cprintf("    -\r\n");
         else
            cprintf("%4s %-10s  %-8s  %-4s    %-4s  \r\n",
               getEmplacementName(ship.component[i]),
               getWeaponName(ship.component[i]),
               ship.component[i]>0? getStatusLabel(shipState[i]) : "-",
               "-",
               getStatusLabel(shipDamage[i])
            );
      }
      if (selected == i) revers(0);
   }
}

void repair(byte componentSelected)
{
   unsigned cost_00 = 0;
   int range = ship.component[ O_QSP_J ] - shipState[ O_STATE_JUMP_FUEL_USED ];

   if (shipState[componentSelected] == STATUS_LOW)
   {   
      cost_00 = 10 * 5 * (ship.component[ O_QSP_J ] - range) * ship.size;
      if (cost_00 < hcr)
      { 
         hcr -= cost_00;
	      clearComponentState(componentSelected);
      } 
      gotoxy(2,50);
      cprintf("fuel cost cr %u00", cost_00);
      pressReturnAndClear();
   }
}

void landAtStarport()
{
   byte componentSelected = 0;

   // if (shipState[ O_STATE_JUMP_FUEL_USED ] > 0)
   //    shipState[ O_STATE_JUMP_FUEL_USED ] = STATUS_LOW;

   clrscr();
   titleLine();
   //statusLine();
   textcolor(COLOR_YELLOW);
   drawPanel(5, MENU_PANEL_ROW, 58, 36, " ship maintenance " );

   for(;;) // menu
   {
      gotoxy(5,2);
      textcolor(COLOR_LIGHTRED);
      showShipSummary(&ship);
      textcolor(COLOR_LIGHTBLUE);
      ship_describe(&ship);
      textcolor(COLOR_YELLOW);
      cputsxy(7,MENU_PANEL_ROW+30,"press <r> over selection to refuel or repair");
      cputsxy(7,MENU_PANEL_ROW+32,"press <u> over selection to upgrade");
      cputsxy(7,MENU_PANEL_ROW+34,"press <return> to exit");

      gotoxy(0,20);
      showComponentStatus(componentSelected);
      checkComponentSelection(componentSelected);
      switch(cgetc())
      {
        case 0x11: // down
                componentSelected = ++componentSelected % COMPONENT_COUNT;
                break;
        case 0x91: // up
                if (componentSelected == 0)
                   componentSelected = COMPONENT_COUNT;
                --componentSelected;
                break;
        case 'r' : // repair (or refuel)
		         repair(componentSelected);
 		         break;
        case 0x1d: // right
               break;
        case 0x9d: // left
               break;
        case 0x0d: // return
               return;
      }
   }
}
