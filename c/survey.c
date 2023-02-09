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

#include "alarm.h"
#include "survey.h"
#include "world.h"
#include "common.h"
#include "ship.h"

extern World current;
extern byte pay_period;
extern long hcr;
extern Starship ship;

void survey()
{
    clrscr();
    statusLine();
    printAlarmBar();

    textcolor(COLOR_CYAN);
    if (WORLD_IS_SURVEYED(&current) == 0)
    {
       byte days = diceRoll2d();
       gotoxy(24,24);
       cprintf("space sensor rating: %u\r\n", SHIP_SPACE_SENSORS(&ship));
       gotoxy(24,26);
       cprintf("system survey (%u days): kcr 100", days );
       hcr += 1000L;
       pay_period += days;
       setWorldStatus(&current, 's');
    }
    else
    {
       if (WORLD_HAS_BELTS(&current))
       {
           byte days = diceRoll2d();
           byte tons = diceRoll3d();
           gotoxy(24,24);
           cprintf("survey sensor rating: %u\r\n", SHIP_SURVEY_SENSORS(&ship));
           gotox(17);
           cprintf("you sold cr %u of ore by prospecting for %u days", tons * 100, days);
           hcr += (long) tons;
           pay_period += days;
       }
       else
       {
           cputsxy(17, 30, "sorry, this system has no planetoid belts");
       }    
    }
    textcolor(COLOR_YELLOW);
    cputsxy(28,50,"(press return to continue)");
    cgetc();
}