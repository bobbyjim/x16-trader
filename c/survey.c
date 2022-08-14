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

extern World current;
extern byte pay_period;
extern long hcr;

void survey()
{
    clrscr();
    statusLine();
    printAlarmBar();

    gotoxy(24,24);
    textcolor(COLOR_CYAN);
    if (WORLD_IS_SURVEYED(&current) == 0)
    {
       byte days = diceRoll2d();
       cprintf("system survey (%u days): kcr 100", days );
       hcr += 1000L;
       pay_period += days;
       setWorldSurveyed(&current);
    }
    else
    {
       if (WORLD_HAS_BELTS(&current))
       {
           byte days = diceRoll2d();
           byte tons = diceRoll3d();
           gotox(17);
           cprintf("you sold cr %u of ore by prospecting for %u days", tons * 100, days);
           hcr += (long) tons;
           pay_period += days;
       }
       else
       {
           cprintf("sorry, this system has no planetoid belts");
       }    
    }
    textcolor(COLOR_YELLOW);
    cputsxy(28,50,"(press return to continue)");
    cgetc();
}