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

#include    <stdlib.h>
#include    <conio.h>

#include    "common.h"
#include    "damage.h"
#include    "bankedText.h"

byte damage_location;

//      O_QSP_M			0
//      O_QSP_J			1
//      O_QSP_BRIDGE	2
//      O_QSP_CPU    	3
//      O_QSP_CARGOP	4
//      O_QSP_FUELP		5
//      O_STATE_JUMP_FUEL_USED 5
//      O_QSP_SPACE		6
//      O_QSP_WORLD		7
//      O_QSP_SCOOPS	8
//      O_QSP_COLLECT	9
//      O_QSP_STEALTH	10
//      O_QSP_DAMPER	11
//      O_QSP_SCREEN	12
//      O_QSP_GLOBE		13
//      O_QDP_BATTERY(x)		(14+x)

// void damage_ship(Starship* thisShip, byte shipDamage[])
// {
//     setBank(TEXT_BANK_1);
//     thisShip->armor--;

//     // for(;;)
//     // {
//     //    damage_location = rand() % SHIP_COMPONENT_COUNT;
//     //    if(thisShip->component[damage_location] > 0)
//     //    {
//     //       shipDamage[ damage_location ]++;

//     //       cputs("captain, ");
//     //       printBankedText(TEXT_SHIP_DAMAGE(damage_location), 34);
//     //       return;
//     //    }
//     // }
// }
