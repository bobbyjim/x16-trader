/*
  
    8SH: a command interpreter for 8 bit 'retro' systems.
    Copyright (C) 2020 Robert Eaglestone

    This file is part of 8SH.

    8SH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    8SH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 8SH.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef bcpm_rand_h
#define bcpm_rand_h

typedef unsigned long int  u4;
typedef struct ranctx 
{ 
   u4 a; 
   u4 b;  
   u4 c; 
   u4 d; 
   unsigned char initialized;
} ranctx;

#define randrot(x,k) (((x)<<(k))|((x)>>(32-(k))))
u4 ran32val( ranctx *x );
void ran32init( ranctx *x, u4 seed );
int burtle32_srand(u4 seed);
u4 burtle32_random();

#endif
