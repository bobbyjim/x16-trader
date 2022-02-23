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

#ifndef _ship_h_
#define _ship_h_

#include "common.h"

#define SHIP_INDEX_ESCORT		0x00
#define SHIP_INDEX_YT_PACKET	0x01
#define SHIP_INDEX_MARAVA		0x02
#define SHIP_INDEX_KILAALUM		0x03
#define SHIP_INDEX_GAZELLE		0x04
#define SHIP_INDEX_SAFARI		0x05
#define SHIP_INDEX_LAB			0x06
#define SHIP_INDEX_LINER		0x07
#define SHIP_INDEX_MARCH		0x08
#define SHIP_INDEX_MERCHANT		0x09
#define SHIP_INDEX_SCOUT		0x0a
#define SHIP_INDEX_LAB2			0x0b
#define SHIP_INDEX_BEOWULF		0x0c
#define SHIP_INDEX_MAADA		0x0d
#define SHIP_INDEX_LISHDA		0x0e
#define SHIP_INDEX_BROADSWORD	0x0f
#define SHIP_INDEX_HOSHISAME	0x10
#define SHIP_INDEX_ENKIDU		0x11
#define SHIP_INDEX_DONOSEV		0x12
#define SHIP_INDEX_PACKET		0x13
#define SHIP_YACHT				0x14


#define O_QSP_M			0
#define O_QSP_J			1
#define O_QSP_BRIDGE	2
#define O_QSP_CPU    	3

#define O_QSP_CARGOP	4
#define O_QSP_FUELP		5
#define O_STATE_JUMP_FUEL_USED 5

#define O_QSP_SPACE		6
#define O_QSP_WORLD		7
#define O_QSP_SCOOPS	8
#define O_QSP_COLLECT	9

#define O_QSP_STEALTH	10
#define O_QSP_DAMPER	11
#define O_QSP_SCREEN	12
#define O_QSP_GLOBE		13

#define O_QDP_HP(x)		(14+x)
#define	SHIP_JUMP_RATING(shptr)			((shptr)->component[O_QSP_J])
#define	SHIP_MANEUVER_RATING(shptr)		((shptr)->component[O_QSP_M])

typedef struct
{
	// header
	byte index;		// ship definition record
	char name[16];
	char allegiance;
	char mission;
	byte size;

	int  cfg         :3;
	int  tl          :5;

	byte armor;
	byte mcrp;
	byte sr;
	byte lb;

	int comfort      :3;
	int demand       :5;

	// component array
	byte component[22];	
} Starship;

void showShipSummary(Starship* ship);

void ship_init(Starship* ship);
void ship_debug(Starship* ship);
void ship_describe(Starship* ship);

byte readShip(byte index, Starship* ship);
char* shipName(byte index);
char* shipMissionFromCode(char code);
char* shipBasicMission(byte index);
char* shipOwner(byte index);

byte  shipOwnerMatchesAllegiance(Starship* ship, char alleg);
byte  shipMatchesBaseRequirements(Starship* ship, char bases);
byte  shipMatchesZoneRequirements(Starship* ship, char zone);
byte  shipMatchesStarportRequirements(Starship* ship, char starport);

#endif
