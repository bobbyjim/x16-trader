#ifndef _ship_h_
#define _ship_h_
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
#include "common.h"

#define STARSHIP_COUNT			(((byte*)(SHIP_DESIGN_COUNT))[0])
#define	STARSHIP_DATA			((Starship*)SHIP_DESIGNS_BEGIN)

#define SHIP_INDEX_EAKHAU		0x00
#define SHIP_INDEX_STAYOW		0x01
#define SHIP_INDEX_EKAWSYKUA	0x02
#define SHIP_INDEX_KHOSAA		0x03
#define SHIP_INDEX_KTEIROA		0x04
#define SHIP_INDEX_KHTUKHAO		0x05
#define SHIP_INDEX_AOSITAOH		0x06
#define SHIP_INDEX_HKIYRERAO	0x07
#define SHIP_INDEX_KTIYHUI 		0x08

#define SHIP_INDEX_MIYMOSA		0x09
#define SHIP_INDEX_LOSKAY		0x0a
#define SHIP_INDEX_DROYNE_ESCORT 0x0b
#define SHIP_INDEX_SYUNUTSTRYAK	0x0c
#define SHIP_INDEX_BRETREMOY	0x0d

#define SHIP_INDEX_BEOWULF		0x0e
#define SHIP_INDEX_MAADA		0x0f
#define SHIP_INDEX_MARAVA		0x10
#define SHIP_INDEX_SUSA         0x11
#define SHIP_INDEX_SAFARI		0x12
#define SHIP_INDEX_LINER		0x13
#define SHIP_INDEX_MARCH		0x14
#define SHIP_INDEX_PACKET		0x15
#define SHIP_INDEX_SUNFLOWER    0x16
#define SHIP_INDEX_GUILDER		0x17
#define SHIP_INDEX_BROADSWORD   0x18
#define SHIP_INDEX_KILAALUM		0x19
#define SHIP_INDEX_GAZELLE	  	0x1a
#define SHIP_INDEX_ANNIC_NOVA	0x1b
#define SHIP_INDEX_KURGULASH	0x1c
#define SHIP_INDEX_DONOSEV		0x1d
#define SHIP_INDEX_LAB			0x1e
#define SHIP_INDEX_SERPENT		0x1f
#define SHIP_INDEX_MURPHY		0x20

#define SHIP_INDEX_KFORGZUE		0x21
#define SHIP_INDEX_SE_KOEZ		0x22
#define SHIP_INDEX_ZUKSEG		0x23
#define SHIP_INDEX_DHAZTUEN		0x24
#define SHIP_INDEX_TATHOE		0x25
#define SHIP_INDEX_AZ_ALRRAK	0x26
#define SHIP_INDEX_FANG			0x27
#define SHIP_INDEX_AEDOKHAZ		0x28

#define SHIP_INDEX_VLEZHDATL	0x29
#define SHIP_INDEX_SESAVETL		0x2a
#define SHIP_INDEX_SHIVVA		0x2b
#define SHIP_INDEX_CHTABL		0x2c
#define SHIP_INDEX_QLIAF		0x2d

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

#define O_QDP_BATTERY(x)		(14+x)

#define	SHIP_BATTERY_T1			0
#define SHIP_BATTERY_T2			32
#define SHIP_BATTERY_T3			64
#define SHIP_BATTERY_B1			96
#define SHIP_BATTERY_B2			128
#define SHIP_BATTERY_BAY		160
#define SHIP_BATTERY_LBAY		196
#define SHIP_BATTERY_MAIN 		228

// char* weapon[] = {
//    "none",
//    "empty",
//    "mining laser",   "pulse laser",       "beam laser",
//    "plasma gun",     "fusion gun",
//    "salvo rack",     "missile",           "kk missile",     "am missile",
//    "jump damper",    "tractor/pressor",   "inducer",        "disruptor",   "stasis",
//    "sandcaster",
//    "hybrid l-s-m",   "particle accelerator",    "meson gun" 
// };

#define SHIP_WEAPON_NONE			0
#define SHIP_WEAPON_EMPTY			1
#define SHIP_WEAPON_MINING_LASER	2
#define SHIP_WEAPON_PULSE_LASER		3
#define SHIP_WEAPON_BEAM_LASER		4
#define SHIP_WEAPON_PLASMA_GUN		5
#define SHIP_WEAPON_FUSION_GUN		6
#define SHIP_WEAPON_SALVO_RACK		7
#define SHIP_WEAPON_MISSILE_RACK	8
#define SHIP_WEAPON_KK_MISSILE_RACK	9
#define SHIP_WEAPON_AM_MISSILE_RACK	10
#define SHIP_WEAPON_JUMP_DAMPER		11
#define SHIP_WEAPON_TRACTOR_PRESSOR	12
#define SHIP_WEAPON_INDUCER			13
#define SHIP_WEAPON_DISRUPTOR		14
#define SHIP_WEAPON_STASIS_GUN		15
#define SHIP_WEAPON_SANDCASTER		16
#define SHIP_WEAPON_HYBRID			17
#define SHIP_WEAPON_PARTICLE_ACCEL	18
#define SHIP_WEAPON_MESON_GUN		19

#define	SHIP_MANEUVER_RATING(shptr)		((shptr)->component[O_QSP_M])
#define	SHIP_JUMP_RATING(shptr)			((shptr)->component[O_QSP_J])
#define SHIP_BRIDGE_RATING(ship)		((ship)->component[O_QSP_BRIDGE])
#define SHIP_CPU_RATING(ship)			((ship)->component[O_QSP_CPU])
#define SHIP_HARDPOINT(ship,hp)			((ship)->component[14+hp])
#define SHIP_CARGO(s)					((s)->component[O_QSP_CARGOP] * (s)->size)
#define SHIP_HAS_SCOOPS(s)				((s)->component[O_QSP_SCOOPS])
#define SHIP_HAS_COLLECTORS(s)			((s)->component[O_QSP_COLLECT])
#define SHIP_SPACE_SENSORS(s)			((s)->component[O_QSP_SPACE]   & 0x1f)
#define SHIP_WORLD_SENSORS(s)			((s)->component[O_QSP_WORLD]   & 0x1f)
#define SHIP_STEALTH_RATING(s)			((s)->component[O_QSP_STEALTH] & 0x1f)
#define SHIP_DAMPER_RATING(s)			((s)->component[O_QSP_DAMPER]  & 0x1f)
#define SHIP_SCREEN_RATING(s)			((s)->component[O_QSP_SCREEN]  & 0x1f)
#define SHIP_GLOBE_RATING(s)			((s)->component[O_QSP_GLOBE]   & 0x1f)

typedef struct
{
	int unit        : 5;
	int emplacement : 3;

} ShipComponent;

#define		SHIP_COMPONENT_COUNT		22
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
	byte component[SHIP_COMPONENT_COUNT];	
} Starship;

void showShipSummary(Starship* ship);

void ship_init(byte ship_index, Starship* ship);
void ship_debug(Starship* ship);
void ship_describe(Starship* ship);
void ship_loadImageFromIndex(byte ship_index);
void ship_loadImage(char* filename);
char* ship_filename(byte ship_index);
byte ship_combatStrength(Starship* ship);
byte readShip(byte index, Starship* ship);
//char* shipMission(char code);
//char* shipOwner(char owner);

#endif
