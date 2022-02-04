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

#include <conio.h>
#include <peekpoke.h>
#include <stdlib.h>

//#include "bank.h"
#include "common.h"
#include "hexmap.h"
#include "world.h"

extern World destination;
extern byte  distance;

int astrogationTop = 5;

void topRow(unsigned char x, unsigned char y)
{
   int i;
   gotoxy(x,y);
   for(i=0; i<4; ++i)
   {
      cbm_k_bsout(T_RIGHT);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(T_LEFT);
      if (i==3) return;
      cputs("    ");
   }
}

void middle(unsigned char x, unsigned char y)
{
   int i;
   gotoxy(x,y);
   for(i=0; i<7; ++i)
   {
      cbm_k_bsout(VERT_BAR);
      cputs("    ");
   }
   cbm_k_bsout(VERT_BAR);
}

void middleConnector(unsigned char x, unsigned char y)
{
   int i;
   gotoxy(x,y);
   cbm_k_bsout(VERT_BAR);
   for(i=0; i<3; ++i)
   {
      cputs("    ");
      cbm_k_bsout(T_RIGHT);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(T_LEFT);
   }
   cputs("    ");
   cbm_k_bsout(VERT_BAR);
}

void endConnector(unsigned char x, unsigned char y)
{
   int i;
   gotoxy(x,y);
   for(i=0; i<3; i++)
   {
      cbm_k_bsout(T_RIGHT);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(T_LEFT);
      cputs("    ");
   }
   cbm_k_bsout(T_RIGHT);
   cbm_k_bsout(HORIZ_BAR);
   cbm_k_bsout(HORIZ_BAR);
   cbm_k_bsout(HORIZ_BAR);
   cbm_k_bsout(HORIZ_BAR);
   cbm_k_bsout(T_LEFT);
}

void bottomRow(unsigned char x, unsigned char y)
{
   int i;
   gotoxy(x+1,y);
   for(i=0; i<6; i++)
   {
      cputs("    ");
      cbm_k_bsout(VERT_BAR);
   }
   gotoxy(x+1,y+1);
   for(i=0; i<3; i++)
   {
      cputs("    ");
      cbm_k_bsout(T_RIGHT);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(HORIZ_BAR);
      cbm_k_bsout(T_LEFT);
   }
}

void highlightCenter()
{
    byte i;
    char boxTop[7] = {CH_LIGHTRED,T_RIGHT,HORIZ_BAR,HORIZ_BAR,HORIZ_BAR,HORIZ_BAR,T_LEFT};
    
    gotoxy(15,14+astrogationTop);
    for(i=0;i<7;++i) cbm_k_bsout(boxTop[i]);
    toDefaultColor();

    gotoxy(15,18+astrogationTop);
    for(i=0;i<7;++i) cbm_k_bsout(boxTop[i]);
    toDefaultColor();
}

void drawHexmap()
{
   int i;

   //cbm_k_bsout(CH_BLACK); 
   //cbm_k_bsout(0x01);  // to bg
   bgcolor(COLOR_BLACK);
   clrscr();
   titleLine();
   statusLine();

   endConnector(0,astrogationTop);
   for(i=0; i<7; i++)
   {
      middle(0,astrogationTop+1+i*4);
      middleConnector(0,astrogationTop+2+i*4);
      middle(0,astrogationTop+3+i*4);
      endConnector(0,astrogationTop+4+i*4);
   }
   bottomRow(0,astrogationTop+29);
   highlightCenter();
}

//
//  col,row are in the center of the map
//
void showWorlds(unsigned char col, unsigned char row)
{
   char i,j,x,y;
   unsigned char bank;
   unsigned int offset;
   unsigned int address;
   char sp,ggb,zone,bases;
   unsigned char a,b;
   unsigned char p;

   p=1;
  
   // translate to (0,0)
   //--row;
   //--col;

   for(i=0; i<7; ++i)
   {
      for(j=0; j<7; ++j)
      {
         offset = (-3+col+i)*64 + (-3+row+j);
         bank = 10 + offset/128;
         setBank(bank);
         offset %= 128;
         address = 0xa000 + offset * 64;

         sp = STARPORT(address);
 
         x = 1 + i * 5;
         y = astrogationTop + 1 + j * 4;
         if (i%2==1) y += 2;

         if (sp==' ')
         { 
            cputsxy(x,y,"    ");
            cputsxy(x,y+2,"    ");
            cputsxy(38,astrogationTop*2, "                                        ");
         } 
         else
         {
            ggb = BGG(address);
            zone = ZONE(address);
            bases = BASES(address);

            //
            //  This displays the currently selected world.
            //
            if (i==3 && j==3)
            {
               gotoxy(38,astrogationTop);
               if (zone == 'a') cbm_k_bsout(COLOR_YELLOW);
               if (zone == 'r') cbm_k_bsout(COLOR_LIGHTRED);
               cputs((char*)address+O_LABEL);
               if (zone != ' ') toDefaultColor();
            }

            cputcxy(x,y,sp);
            cputcxy(x+1,y,bases);
            cputcxy(x+3,y,ggb);

            a = HEXCOL(address);
            b = HEXROW(address);
            gotoxy(x,y+2);
            cprintf("%02u%02u",a,b);
         }
      }
   }
}

byte pickDestination(unsigned char col, unsigned char row, unsigned char range)
{
   byte dx = col;
   byte dy = row;

   char plural;

   drawHexmap();
   cputsxy(5,2,"astrogation shell");
   cputsxy(5,58,"select target world [ijkl] and then press <return>");
   for(;;)
   {
      showWorlds(dx,dy);
      distance = parsecDistance(col,row,dx,dy);     
      plural = distance ==1? ' ' : 's';
      gotoxy(5,40);
      cprintf("%02u%02u distance: %u parsec%c  range:%u", dx, dy, distance , plural, range);
      switch(cgetc())
      {   
         case 0x91:
         case 'i': if (dy > row-range) --dy; break;
         case 0x11:
         case 'k': if (dy < row+range) ++dy; break;
         case 0x9d:
         case 'j': if (dx > col-range) --dx; break;
         case 0x1d:
         case 'l': if (dx < col+range) ++dx; break;
         case 13 : 
		if (distance >0 && distance <=range) 
 	 	{
            	   destination.col = dx; //-1;
            	   destination.row = dy; //-1;
            	   getWorld(&destination);
            	   if ( destination.starport != ' ' ) // NOT empty hex
            	   {
	       	         return 1;
            	   }
	 	}
		return 0;
      }
   }
}

