#include <conio.h>
#include <cbm.h>
#include <cx16.h>

#include "builder.h"

char* titleLine[] = {

         "             *                                                            **    ",    
         "           **        *                                                     **   ",    
         "           **       ***                                                    **   ",    
         "           **        *                                                     **   ",    
         "   ****    **                 ****   **   ****            ***  ****        **   ",    
         "  * **** * **  *** ***       * ***  * **    ***  *  ****   **** **** * *** **   ",    
         " **  ****  ** * *** ***     *   ****  **     ****  * ***  * **   **** ********* ",    
         "****       ***   *** **    **    **   **      **  *   ****  **       **   ****  ",    
         "  ***      **     ** **    **    **   **      ** **    **   **       **    **   ",    
         "    ***    **     ** **    **    **   **      ** **    **   **       **    **   ",    
         "      ***  **     ** **    **    **   **      ** **    **   **       **    **   ",    
         " ****  **  **     ** **    **    **   **      ** **    **   **       **    **   ",    
         "* **** *   **     ** **    *******     ********* **    **   ***      **    **   ",    
         "   ****    **     ** *** * ******        **** *** ***** **   ***      *****     ",    
         "            **    **  ***  **                  *** ***   **            ***      ",    
         "                  *        **           *****   ***                             ",    
         "                 *         **         ********  **                              ",    
         "                *           **       *      ****                                ",    
         "               *                                                                "
};
void title(void)
{
   int i=5;
   bgcolor(COLOR_BLUE);
   textcolor(COLOR_YELLOW);
   clrscr();

   cbm_k_bsout(0x8E); // revert to primary case
   cbm_k_setnam("petfont.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(2, 0x0f800);

   for(i=0; i<19; ++i)
      cputsxy( 0, i+5, titleLine[i]);

   chlinexy( 0, 28, 80 );
   cputsxy( 5, 30, "adventure-class starship design");
   cputsxy( 5, 32, "december 2021 r. eaglestone");

    cputsxy(30,45, "press <space>");
    cgetc();
}

void main(void)
{
    title();
    builder_init();
    builder();
}