#ifndef RESEAU_H
#define RESEAU_H

#include <stdlib.h>
#include <stdint.h> // pour uint8_t, uint32_t, souvent dans stdint.h mais pas toujours
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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

typedef char AdresseIP[16];

// Station
typedef struct
{
    AdresseMAC mac;
    AdresseIP ip;
} Station;

// Table de commutation
typedef struct
{
    int nbPort;
    uint64_t AdresseMAC;

} tableCommutation;

// Switch
typedef struct
{
    AdresseMAC mac;
    size_t nbPorts;
    size_t priorite;
    TypePort *ports;
    tableCommutation *tableCommutation;

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

// Arete entre l'equipements
typedef struct
{
    size_t index_e1;
    size_t index_e2;
    uint8_t poids;
} Arete;

// Graph
typedef struct
{
    Equipement *equipements;
    Arete *aretes;
    size_t nb_aretes;
    size_t nb_equipements;
} Graphe;

// Function declarations
void afficherIP(AdresseIP ip);
void afficherMacHexa(const char *str);
void afficherSwitch(Switch sw);
void afficherSation(Station s);
int hexCharToInt(char c);
uint64_t convertMacToInteger(const char *str, uint64_t *mac);

int creeReseau(const char *filePath, Graphe *reseau);
int extraireNbEquipementsEtAretes(int *nbEquipements, int *nbAretes, char *ligne);
int estInteger(char *str, int *temp);
int verifyMac(const char *mac);
void ajoutEquipement(Graphe *reseau, Equipement *e, int index);
int verifyIp(char *ip);
int partieIPEstValide(char *partie);
FILE *ouvrirFichier(const char *filePath);
int lireNbEquipementsEtAretes(FILE *fptr, int *nbEquipements, int *nbAretes);
int initialiserReseau(Graphe *reseau, int nbEquipements, int nbAretes);
int creerEquipements(FILE *fptr, Graphe *reseau, int nbEquipements);
int verifieNbEquipementEtAretes(FILE *fptr, int nbEquipements, int nbAretes);
#endif
