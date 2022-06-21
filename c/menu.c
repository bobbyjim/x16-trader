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

#include "menu.h"

unsigned char mi, oldmi, i;

unsigned char menu_run(unsigned char  x, unsigned char y, unsigned char optcount, char* opts[])
{
    char letter = 65;
    //chlinexy(x, y+1, 50);
    for(i=0; i<optcount; ++i)
    {
        gotoxy(x + 2, 2 + y + i * 2);
        textcolor(COLOR_GRAY2);
        cputc('[');
        textcolor(COLOR_ORANGE);
        cputc( 65 + i );
        textcolor(COLOR_GRAY2);
        cputs("] ");
        textcolor(COLOR_YELLOW);
        cputs(opts[i]);
//        cputcxy(x + 2, 2 + y + i * 2, 65 + i);
//        cputsxy(x + 4, 2 + y + i * 2, opts[i]);
    }

    mi = 0;
    for(;;)
    {
        cputcxy( x, 2 + y + oldmi * 2, ' ');
        cputcxy( x, 2 + y + mi    * 2, '*');
        oldmi = mi;
        letter = cgetc();
        if (letter > 64 && letter < 65 + optcount)
        {
            return letter - 65;
        }
        switch(letter)
        {
            case 17:  // down
                ++mi;
                if (mi >= optcount) mi = 0;
                break;

            case 145: // up
                --mi;
                if (mi == 255) mi = optcount - 1;
                break;

            case 13: 
                return mi;
        }
    }
}
