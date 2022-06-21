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

#include <stdlib.h>
#include <conio.h>

#include "common.h"
#include "combat.h"
#include "damage.h"

extern byte              playerAchievementLevel;
extern Starship          ship;
extern byte              shipState[SHIP_COMPONENT_COUNT];
extern byte              shipDamage[SHIP_COMPONENT_COUNT];
extern byte engineer;
extern byte gunner  ;
extern byte medic   ;
extern byte pilot   ;

Starship adversary;
byte     adversaryDamage[SHIP_COMPONENT_COUNT];
byte     adversaryEngineer;
byte     adversaryGunner;
byte     adversaryMedic;
byte     adversaryPilot;

void combat_run()
{
    // 
}
