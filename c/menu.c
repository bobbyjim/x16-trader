#include <conio.h>

#include "menu.h"

unsigned char mi, oldmi, i;

unsigned char menu_run(unsigned char  x, unsigned char y, unsigned char optcount, char* opts[])
{
    chlinexy(x, y+1, 50);
    for(i=0; i<optcount; ++i)
        cputsxy(x + 2, 2 + y + i * 2, opts[i]);

    mi = 0;
    for(;;)
    {
        cputcxy( x, 2 + y + oldmi * 2, ' ');
        cputcxy( x, 2 + y + mi    * 2, '*');
        oldmi = mi;
        switch(cgetc())
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
