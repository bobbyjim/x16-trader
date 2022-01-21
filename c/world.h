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

#ifndef _world_h_
#define _world_h_

#include <peekpoke.h>

#include "common.h"

#define O_HEXCOL        0
#define O_HEXROW        1
#define O_LONGLABEL     2
#define O_LABEL         7
#define O_STARPORT      28
#define O_ZONE          38
#define O_ALLEGIANCE    39
#define O_BASES         41
#define O_BGG           42
#define O_TCINDEX       43
#define O_TC1           44
#define O_SIZATM        45
#define O_HYDPOP        46
#define O_GOVLAW        47
#define O_TL            48
#define O_STAR1         58
#define O_STAR2         60
#define O_STAR3         62

#define WORLD_ADDR(W)	(ADDRESS_START+W->record*64)
#define HEXCOL(A)	PEEK(A+O_HEXCOL)
#define HEXROW(A)	PEEK(A+O_HEXROW)
#define STARPORT(A)	PEEK(A+O_STARPORT)
#define BGG(A)		PEEK(A+O_BGG)
#define ZONE(A)		PEEK(A+O_ZONE)
#define ALLEG(A)	PEEK(A+O_ALLEGIANCE)
#define BASES(A)	PEEK(A+O_BASES)
#define TCINDEX(A)	PEEK(A+O_TCINDEX)
#define	TC1(A)		PEEK(A+O_TC1)
#define POP(A)		(PEEK(A+O_HYDPOP)<<4)
#define TL(A)		PEEK(A+O_TL)

typedef struct
{
   int ancients: 1;
   int capital:  1;
   int hellworld: 1;
   int icecapped: 1;
   int military:  1;
   int reserve:   1;
   int research:  1;
   int satellite: 1;
} TC1BitFields;

//
// for non-mainworlds, row,col = orbit, theta
//
#define	SET_ORBIT(W,O)	(W.row=O)
#define SET_THETA(W,T)	(W.col=T)
#define GET_ORBIT(W)  	W.row
#define GET_THETA(W)	W.col

typedef struct
{
   byte col;		// for non-mainworlds, this is theta
   byte row;		// for non-mainworlds, this is orbit #
   byte bank;
   byte record;

   char starport;
   byte siz;
   byte atm;
   byte hyd;
   byte pop;
   byte tl;

   char bgg;
   char zone;
   char bases;
   char alleg;
   byte tcIndex;
   byte tc1;

} World;

void printWorld(World* world);
char* getLabel(World* world);
char* getShortLabel(World* world);
void getWorld(World* world);
char* getBases(World* world);
char hasGasGiants(World* world);
char hasBelts(World* world);
char* getZone(World* world);
byte getHexCol(World* world);
byte getHexRow(World* world);

void buildLocalSystem(World* mainworld);

#endif
