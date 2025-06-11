#ifndef RESEAU_H
#define RESEAU_H

#include <stdlib.h>
#include <stdint.h> // pour uint8_t, uint32_t, souvent dans stdint.h mais pas toujours
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

#define MAX_DATA_SIZE 1500
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_ENTER 10

#define MAX_HOPS 50

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

typedef struct
{
    size_t portId;
    TypePort typePort;
    size_t connectedEquipementIndex;
} Port;

// Switch
typedef struct
{
    AdresseMAC mac;
    size_t nbPorts;
    size_t priorite;
    Port *ports;
    tableCommutation *tableCommutation;
    size_t maxCapacityTable;
    size_t nbTable;

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

// Trames Ethernet
typedef struct
{
    uint8_t preambule[7];
    uint8_t SFD;
    AdresseMAC Dest;
    AdresseMAC Src;
    uint16_t type;
    uint8_t donnees[MAX_DATA_SIZE];
    uint32_t FCS;
} EthernetTram;

// Function declarations
void afficherIP(AdresseIP ip);
void afficherMacHexa(uint64_t mac);
char *obtenirMACString(uint64_t mac);
void afficherSwitch(Switch sw);
void afficherSation(Station s);
void afficherTrameUtilisateur(EthernetTram *trame);
void afficherTrameHexa(EthernetTram *trame);
void affichageMachine(Graphe g);
int affichageMachineMenu(Graphe g, int optionDejaSelectionner);
void afficherPortsSwitch(Graphe *g, Switch sw);
void afficherMenu(char **menu, int nbOptions, int selection, int dejaSelectionner);
void restaurerEntree(struct termios *ancien);
void desactiverEntreeBufferisee(struct termios *ancien);
int hexCharToInt(char c);
uint64_t convertMacToInteger(const char *str, uint64_t *mac);
int menuInteractif(char **menu, int nbOptions, int dejaSelectionner);

int creeReseau(const char *filePath, Graphe *reseau);
int extraireNbEquipementsEtAretes(int *nbEquipements, int *nbAretes, char *ligne);
int estInteger(char *str, int *temp);
int verifyMac(const char *mac);
void ajoutEquipement(Graphe *reseau, Equipement *e, int index);
void ajoutArete(Graphe *reseau, Arete *a, int index);
int verifyIp(char *ip);
int partieIPEstValide(char *partie);
FILE *ouvrirFichier(const char *filePath);
int lireNbEquipementsEtAretes(FILE *fptr, int *nbEquipements, int *nbAretes);
int initialiserReseau(Graphe *reseau, int nbEquipements, int nbAretes);
int creerEquipements(FILE *fptr, Graphe *reseau, int nbEquipements);
int verifieNbEquipementEtAretes(FILE *fptr, int nbEquipements, int nbAretes);
int creerArrets(FILE *fptr, Graphe *reseau, int nbAretes);
int nbSwitchReseaux(Graphe g);
int nbStationReusax(Graphe g);
int validerStationInput(Graphe g);
void mettreAJourTableCommutation(Switch *sw, AdresseMAC mac, size_t portId);

int firstSwitchFound(Graphe g, size_t indexMachine);
void communiquer(Graphe *g);

void remplirTablePort(Graphe *g);
int surQuellePortConnecter(Graphe g, int senderIndex, int switchIndex);
int envoyerTram(Graphe *g, int senderIndex, int receicerIndex, EthernetTram *t);
int envoyerTramRec(Graphe *g, int currentSwitchIndex, int cameFromEquipIndex, EthernetTram *t, int depth);
#endif
