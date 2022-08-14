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

#include "wilderness.h"
#include "ship.h"
#include "world.h"
#include "alarm.h"

extern Starship ship;
extern byte  shipState[22];
extern World current;
extern byte pay_period;
extern byte playerAchievementLevel;

//char* star_magnitude[] = { " ", "i", "ii", "iii", "iv", "v", "vi" };

#define REFUEL_MESSAGE_Y    37

void wilderness_refuel()
{
    shipState[ O_STATE_JUMP_FUEL_USED ] = 0;

    clrscr();
    printAlarmBar();
    statusLine();

    // print a little summary of the data we've got on this system
    cputsxy(15,35,current.data.sector);
    cputcxy(25,35,current.data.starport);
    cputsxy(26,35,current.data.uwp);

    // gotox(35);
    // cprintf("%c%d %s  %c%d %s  %c%d %s", 
    //    current.data.star_class_1,
    //    current.data.star_num_1,
    //    star_magnitude[current.data.star_size_1],

    //    current.data.star_class_2,
    //    current.data.star_num_2,
    //    star_magnitude[current.data.star_size_2],

    //    current.data.star_class_3,
    //    current.data.star_num_3,
    //    star_magnitude[current.data.star_size_3]
    // );

    if (!current.data.surveyed)
    {
        cputs("  ");
        cputc(STAR_TRADER_ASTERISK_CHARACTER);
        cputc(STAR_TRADER_ASTERISK_CHARACTER);
        cputs(" not");
    }
    cputs(" surveyed");

    textcolor(COLOR_GREEN);
    gotoxy(0,40);

    if (current.data.hyd > 0 && current.data.atm > 2 && current.data.atm < 11)
    {
        cputsxy(7,REFUEL_MESSAGE_Y,"you have refueled from surface water on the mainworld (one day)");
        world_setRockballPalette();
        drawWorld(0,1);
        ++pay_period;
    }
    else if (current.data.gg && ship.component[ O_QSP_SCOOPS ])
    {
        cputsxy(12,REFUEL_MESSAGE_Y,"you have refueled by skimming a gas giant (five days)");
        world_setGasGiantPalette();
        drawWorld(1,1);
        pay_period += 5;
    }
    else if (ship.component[ O_QSP_COLLECT ])
    {
        cputsxy(6,REFUEL_MESSAGE_Y,"you have refueled by charging your collectors (seven days)");
        pay_period += 7; // might be more based on condition of collectors!
    }
    else if (current.data.belt)
    {
        cputsxy(13,REFUEL_MESSAGE_Y,"you have refueled from ice in an asteroid belt (ten days)");
        world_drawPlanetoid(37,20);
        pay_period += 10;
        //
        //  This might also be the correct place to inject a little prospecting??
        //
    }
    else 
    {
        cputsxy(19,REFUEL_MESSAGE_Y,"you have refueled at the oort cloud (two weeks)");        
        pay_period += 14;
    }

    if (playerAchievementLevel < 4) playerAchievementLevel = 4;
    
    textcolor(COLOR_YELLOW);
    cputsxy(28,50,"(press return to continue)");
    cgetc();
}