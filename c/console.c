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
//#include <stdio.h>
#include <string.h>

#include "console.h"
#include "world.h"

extern World current;

void console()
{
    // char buffer[8];
    // int  row, col;

    // for(;;)
    // {
    //     cputs("% ");
    //     scanf("%s", buffer);
    //     if(!strcmp(buffer, "bye"))
    //     {
    //         return;
    //     }
    //     if(!strcmp(buffer, "set") || !strcmp(buffer, "s"))
    //     {
    //         scanf("%s", buffer);
    //         if (!strcmp(buffer, "hex"))
    //         {
    //            scanf("%d %d", col, row);
    //            current.row = row;
    //            current.col = col;
    //            getWorld(&current);
    //            cprintf("done\r\n");
    //         }
    //     }
    // }
}