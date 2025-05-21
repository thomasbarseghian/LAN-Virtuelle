#ifndef RESEAU_H
#define RESEAU_H

#include <stdlib.h>
#include <stdint.h> // pour uint8_t, uint32_t, souvent dans stdint.h mais pas toujours

// Address structures
typedef struct {
    uint64_t mac;
} AdresseMAC;

typedef struct {
    uint32_t ip;
} AdresseIP;

// Station
typedef struct {
    AdresseMAC mac;
    AdresseIP ip;
} Station;

// Switch
typedef struct {
    AdresseMAC mac;
    int nbPorts;
    int priorite;
} Switch;

typedef enum {
    STATION_TYPE,
    SWITCH_TYPE
} TypeEquipement;


// Union structure
typedef struct {
    TypeEquipement type;
    union {
        Station station;
        Switch sw;
    };
} Equipement;

// Function declarations
void afficherMAC(AdresseMAC mac);
void afficherIP(AdresseIP ip);

#endif
