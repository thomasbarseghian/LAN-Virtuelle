#ifndef RESEAU_H
#define RESEAU_H

#include <stdlib.h>
#include <stdint.h> // pour uint8_t, uint32_t, souvent dans stdint.h mais pas toujours

// Address structures
typedef enum
{
    STATION_TYPE,
    SWITCH_TYPE
} TypeEquipement;

typedef enum
{
    ROOT,
    DESIGNATED,
    NON_DESIGNATED
} TypePort;

typedef uint64_t AdresseMAC;

typedef uint32_t AdresseIP;

// Station
typedef struct
{
    AdresseMAC mac;
    AdresseIP ip;
} Station;

// Switch
typedef struct
{
    AdresseMAC mac;
    int nbPorts;
    int priorite;
    TypePort typePort;

} Switch;

// Union structure
typedef struct
{
    TypeEquipement type;
    union
    {
        Station station;
        Switch sw;
    };
} Equipement;

// Function declarations
void afficherMAC(AdresseMAC mac);
void afficherIP(AdresseIP ip);
uint64_t convertMacToHexa(char *str);

#endif
