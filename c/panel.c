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
#include <cx16.h>
#include <conio.h>
#include <stdlib.h>

#include "panel.h"

#define     CORNER_NW       213
#define     CORNER_SW       202
#define     CORNER_NE       201
#define     CORNER_SE       203
#define     HORIZ           192
#define     VERT            221

#define     DEFAULT_COLOR       COLOR_GRAY3

//
//  height and width include border
//
void drawPanel( int x, int y, int width, int height, char* label )
{
    int tmp;

    --width;
    --height;

    cputcxy(x,       y,        CORNER_NW);
    cputcxy(x+width, y,        CORNER_NE);
    cputcxy(x,       y+height, CORNER_SW);
    cputcxy(x+width, y+height, CORNER_SE);

    for(tmp=x+1; tmp<x+width; ++tmp)
    {
        cputcxy(tmp,y,       HORIZ);
        cputcxy(tmp,y+height,HORIZ);
    }

    cputsxy(x+2,y,label);

    while(--height)
    {
        cputcxy(x,      y+height,VERT);
        cputcxy(x+width,y+height,VERT);
    }
}
