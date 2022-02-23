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

#ifndef _trade_h_
#define _trade_h_

#include "common.h"

typedef struct
{
    char      label[16];
    unsigned  base;
    byte      dice;
    byte      mult;

    char      p00;
    char      p01;
    char      p10;
    char      p11;
    char      p20;
    char      p21;

    char      r00;
    char      r01;
    char      r10;
    char      r11;
    char      r20;
    char      r21;
} CargoEntry;

typedef struct
{
    CargoEntry* cargoAddress;
    byte tons; 
    long price;
} Cargo;


unsigned getHoldFree();

// byte getBasePrice(byte tradeIndex);
// char* getTradeCodes(byte tradeIndex);
// char* getTradeGoods(byte tradeIndex);
// void printTradeCodes(byte tradeIndex);
// void printTradeGoods(byte tradeIndex);

void trade_calculateMarketPrices();
void trade_speculate();

#endif
