/*

    Traveller-Trader: a space trader game
    Copyright (C) 2021 Robert Eaglestone

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

#ifndef _alarm_h_
#define _alarm_h_

#include <cx16.h>
#include "common.h"

#define STATUS_OK             	0
#define STATUS_MINOR          	1
#define STATUS_SERIOUS        	2
#define STATUS_MAJOR          	3
#define STATUS_CRITICAL       	4
#define STATUS_UNKNOWN        	5
#define STATUS_LOW	      	    6

#define COMPONENT_COUNT		    22
#define FIRST_EMPLACEMENT       14

byte getComponentState(byte component);
void clearComponentState(byte component);
byte setComponentState(byte component, byte state, word cost);
void showComponentStatus(byte compnum);

void printAlarmBar();
char* getStatusLabel(byte severity);

void printMainComponentState(char state);
void printSecondaryComponentState(char state);

#endif
