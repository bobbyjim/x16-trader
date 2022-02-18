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
#ifndef _MANEUVER_MAP_H_
#define _MANEUVER_MAP_H_

#define ASTROGATION_OPTION          0
#define JUMP_OPTION                 1
#define WILDERNESS_REFUEL_OPTION    2
#define MARKET_OPTION               3
#define STARPORT_OPTION             4
#define HIRING_HALL_OPTION          5
#define SHIPYARD_OPTION		        6

//void maneuvermapInit();
//void maneuvermapShow();
void showCurrentLocation();
unsigned char maneuver();

#endif
