#include <conio.h>
#include <stdio.h>
#include <string.h>

#include "console.h"
#include "world.h"

extern World current;

void console()
{
    char buffer[8];
    int  row, col;

    for(;;)
    {
        cputs("% ");
        scanf("%s", buffer);
        if(!strcmp(buffer, "bye"))
        {
            return;
        }
        if(!strcmp(buffer, "set") || !strcmp(buffer, "s"))
        {
            scanf("%s", buffer);
            if (!strcmp(buffer, "hex"))
            {
               scanf("%d %d", col, row);
               current.row = row;
               current.col = col;
               getWorld(&current);
               cprintf("done\r\n");
            }
        }
    }
}