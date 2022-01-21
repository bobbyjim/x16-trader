#ifndef _DAMAGE_H_
#define _DAMAGE_H_

void showShipBits();

typedef struct {

    unsigned config : 3; // CBPUSAL

} Hull;

typedef struct {

} Drive;

typedef struct {

    unsigned category    : 2; // sensor, weapon, defense
    unsigned code        : 5; // A-Z (0-25)
    unsigned range       : 4; 
    unsigned emplacement : 4;

} Equipment;

typedef struct {

} Ops;

typedef struct {

} LifeSupport;

typedef struct {

    unsigned type   : 4;
    unsigned status : 4;
    unsigned rating : 4;
    unsigned damage : 4;
    unsigned volume : 13; // 4096
    unsigned mcr    : 10; // 1024 (BCr1)

    union {
        
    }
    // component-specific features could go in a union?

} Component;


#endif
