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

#ifndef _HIRING_HALL_H_
#define _HIRING_HALL_H_

#include "common.h"

#define     SKILL_NONE          0
#define     SKILL_ADMIN         1
#define     SKILL_ASTROGATOR    2
#define     SKILL_ENGINEER      3
#define     SKILL_GUNNER        4
#define     SKILL_LIAISON       5
#define     SKILL_MEDIC         6
#define     SKILL_PILOT         7
#define     SKILL_STEWARD       8
#define     SKILL_STREETWISE    9

#define     TOTAL_SKILLS        10
#define     TOTAL_CREW_SLOTS    10

typedef struct
{
    char* name;
    byte upp[6];
    char race;
    byte skill[TOTAL_SKILLS];
    byte room;
} Crew;

void hire();
byte findHighestSkillFor(byte skill);

#endif
