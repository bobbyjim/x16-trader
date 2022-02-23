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

#ifndef _world_h_
#define _world_h_

#include "common.h"

#define  DIFFERENT_WORLDS(w1,w2)       (w1.row != w2.row || w1.col != w2.col)
#define  WORLD_LONG_LABEL(worldptr)    ((worldptr)->data.name)
#define  WORLD_HAS_GGS(worldptr)       ((worldptr)->data.gg)
#define  WORLD_HAS_BELTS(worldptr)     ((worldptr)->data.belt)

#define  STARPORT_NO_MAINTENANCE(worldptr)   ((worldptr)->data.starport > 'c')
#define  STARPORT_NO_SHIPYARD(worldptr)      ((worldptr)->data.starport > 'b')
#define  STARPORT_SLOW_FUEL(worldptr)        ((worldptr)->data.starport > 'b')    
#define  STARPORT_NO_HIRING_HALL(worldptr)   ((worldptr)->data.starport > 'd')

//
// for non-mainworlds, row,col = orbit, theta
//
// #define SET_ORBIT(W,O)	(W.row=O)
// #define SET_THETA(W,T)	(W.col=T)
// #define GET_ORBIT(W)  	W.row
// #define GET_THETA(W)	   W.col

typedef struct {
   unsigned char col;
   unsigned char row;
   char sector[10];      // sector, hex, and null byte
   char name[16];        // name and null byte
   char starport;
   char uwp[9];          // sahpgl-t and null byte

   char zone;
   char allegiance;
   char bases;
   byte pad1;

   int  zone_digital : 2;
   int  belt : 1;
   int  gg   : 1;
   int  junk : 4;

   unsigned char tcIndex;
   
   int ancients:  1;
   int capital:   1;
   int hellworld: 1;
   int icecapped: 1;
   int military:  1;
   int reserve:   1;
   int research:  1;
   int satellite: 1;

   int siz       :4;
   int atm       :4;
   int hyd       :4;
   int pop       :4;
   int gov       :4;
   int law       :4;
   int tl        :8;

   int agricultural :1;
   int asteroid     :1;
   int barren       :1;
   int desert       :1;
   int fluid_seas   :1;
   int hi_pop       :1;
   int industrial   :1;
   int lo_pop       :1;
   int non_agri     :1;
   int non_ind      :1;
   int poor         :1;
   int rich         :1;
   int vacuum       :1;
   int water_world  :1;
   int sparebits    :2;

   unsigned char general_world_type;
   unsigned char buffer2[6];
   char star_class_1;
   int  star_size_1   :3;
   int  star_num_1    :5;
   char star_class_2;
   int  star_size_2   :3;
   int  star_num_2    :5;
   char star_class_3;
   int  star_size_3   :3;
   int  star_num_3    :5;

} WorldData;


typedef struct
{
   WorldData data;   // all read-only world data
   byte bank;        // the bank this world is in
   byte record;      // the byte offset
   unsigned int address;

   //
   //  These two fields are R/W for READING in the world data.
   //
   byte col;		// for non-mainworlds, this is theta
   byte row;		// for non-mainworlds, this is orbit #

} World;

void showTradeCodes(World* world);
void world_setGasGiantPalette();
void world_setRockballPalette();
void printWorld(World* world);
void drawWorld(byte streaky, byte variance);
void getWorld(World* world);
void world_describe(World* world);

#endif
