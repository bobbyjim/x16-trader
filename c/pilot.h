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

#ifndef _pilot_h_
#define _pilot_h_

#include "common.h";

#define PILOT_OPTION            0
#define ASTROGATION_OPTION      1
#define STARPORT_OPTION         2
#define JUMP_OPTION             3
#define MARKET_OPTION           4
#define HIRING_HALL_OPTION      5
#define SHIPYARD_OPTION		    6

byte doPilot();
void refuel();
void dock();

#endif
