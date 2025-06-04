#include "reseau.h"

void afficherIP(AdresseIP ip)
{
    printf("Adresse IP: %s \n", ip);
}

void afficherMacHexa(uint64_t mac)
{
    printf("Adresse MAC en Hexa: %#lx\n", mac);
}

void afficherMAC(uint64_t mac)
{
    uint8_t bytes[6];
    char *buffer = malloc(sizeof(char) * 18);

    for (int i = 0; i < 6; i++)
    {
        bytes[5 - i] = (mac >> (i * 8)) & 0xFF;
    }

    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            bytes[0], bytes[1], bytes[2],
            bytes[3], bytes[4], bytes[5]);

    printf("%s \n", buffer);
    free(buffer);
}

void afficherSwitch(Switch sw)
{
    printf("========= Switch =========\n");
    printf("Adresse Mac : %lu\n", sw.mac);
    printf("Nombre de ports : %ld\n", sw.nbPorts);
    printf("Priorité : %ld\n", sw.priorite);
}

void afficherSation(Station s)
{
    printf("========= Station =========\n");
    printf("Adresse Mac : %lu\n", s.mac);
    printf("Adresse ip : %s\n", s.ip);
}

// Convertit un caractère hexadécimal ('0'-'9', 'a'-'f', 'A'-'F') en entier
int hexCharToInt(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return (c - 'A') + 10;
    if (c >= 'a' && c <= 'f')
        return (c - 'a') + 10;
    return -1;
}

// Pour extraire le nombre d'équipements et d'arêtes
int extraireNbEquipementsEtAretes(int *nbEquipements, int *nbAretes, char *ligne)
{
    bool lecturePremierNombre = false;
    *nbEquipements = 0;
    *nbAretes = 0;

    while (*ligne != '\n')
    {
        if (*ligne >= '0' && *ligne <= '9')
        {
            if (!lecturePremierNombre)
            {
                *nbEquipements = *nbEquipements * 10 + (*ligne - '0');
            }
            else
            {
                *nbAretes = *nbAretes * 10 + (*ligne - '0');
            }
        }
        else if (*ligne == ' ')
        {
            if (!lecturePremierNombre)
            {
                lecturePremierNombre = true;
            }
            else
            {
                return EXIT_FAILURE;
            }
        }
        else
        {

            return EXIT_FAILURE;
        }

        ligne++;
    }
    if (!lecturePremierNombre)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int estInteger(char *str, int *temp)
{
    int integer = 0;
    if (str == NULL || *str == '\0')
    {
        return EXIT_FAILURE;
    }
    while (*str != '\0' && *str != '\n' && *str != '\r')
    {
        if (*str >= '0' && *str <= '9')
        {
            integer = (integer * 10) + (*str - '0');
            str++;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    *temp = integer;
    return EXIT_SUCCESS;
}

int verifyMac(const char *mac)
{
    int i = 0;
    while (mac[i] != '\0' && mac[i] != '\n')
    {
        if ((mac[i] >= '0' && mac[i] <= '9') ||
            (mac[i] >= 'A' && mac[i] <= 'F') ||
            (mac[i] >= 'a' && mac[i] <= 'f') ||
            (mac[i] == ':'))
        {
        }
        else
        {
            return EXIT_FAILURE;
        }
        i++;
    }
    if (i > 17)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void ajoutEquipement(Graphe *reseau, Equipement *e, int index)
{
    reseau->equipements[index] = *e;
}

void ajoutArete(Graphe *reseau, Arete *a, int index)
{
    reseau->aretes[index] = *a;
}

int verifyIp(char *ip)
{
    if (ip == NULL)
        return EXIT_FAILURE;
    char *partie = strtok(ip, ".");
    int i = 0;
    while (partie != NULL)
    {
        if (partieIPEstValide(partie))
            return EXIT_FAILURE;
        i++;
        partie = strtok(NULL, ".");
    }
    if (i < 3)
        return EXIT_FAILURE; // IP n'est pas bonne
    return EXIT_SUCCESS;
}

int partieIPEstValide(char *partie)
{
    int len = strlen(partie);
    if (len == 0 || (len > 3 && partie[3] != '\n'))
    {
        return EXIT_FAILURE;
    }

    if (len > 0 && partie[0] == '0')
        return EXIT_FAILURE;
    int valid8bits = 0;
    while (*partie != '\n' && *partie != '\0')
    {
        if (*partie >= '0' && *partie <= '9')
        {
            valid8bits = valid8bits * 10 + (*partie - '0');
        }
        else
        {
            return EXIT_FAILURE;
        }
        partie++;
    }
    if (valid8bits < 0 || valid8bits > 255)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int nbSwitchReusax(Graphe g)
{
    int nbSwitches = 0;
    for (size_t i = 0; i < g.nb_equipements; i++)
    {
        if (g.equipements[i].type == SWITCH_TYPE)
        {
            nbSwitches++;
        }
    }
    return nbSwitches;
}
int nbStationReusax(Graphe g)
{
    int nbSwitches = 0;
    for (size_t i = 0; i < g.nb_equipements; i++)
    {
        if (g.equipements[i].type == STATION_TYPE)
        {
            nbSwitches++;
        }
    }
    return nbSwitches;
}

void afficherTrameUtilisateur(EthernetTram *trame)
{
    printf("----- Trame Ethernet (mode utilisateur) -----\n");
    printf("Adresse MAC Source      : ");
    afficherMAC(trame->Dest);
    printf("Adresse MAC Destination : ");
    afficherMAC(trame->Src);
    printf("Type                    : 0x%04X\n", trame->type);
    printf("Données                 : %s\n", trame->donnees);
    printf("FCS                     : 0x%08X\n", trame->FCS);

    printf("--------------------------------------------\n");
}

void afficherTrameHexa(EthernetTram *trame)
{
    printf("----- Trame Ethernet (hexadécimal brut) -----\n");

    for (int i = 0; i < 7; i++)
    {
        printf("%02X ", trame->preambule[i]);
    }
    printf("%02X ", trame->SFD);

    for (int i = 5; i >= 0; i--)
    {
        printf("%02X ", (uint8_t)(trame->Dest >> (i * 8)));
    }
    for (int i = 5; i >= 0; i--)
    {
        printf("%02X ", (uint8_t)(trame->Src >> (i * 8)));
    }
    printf("%02X %02X", (trame->type >> 8) & 0xFF, ((trame->type) & 0xFF));

    for (size_t i = 0; trame->donnees[i] != '\0'; i++)
        printf("%02X ", (uint8_t)trame->donnees[i]);
    printf("%02X %02X %02X %02X\n",
           (trame->FCS >> 24) & 0xFF,
           (trame->FCS >> 16) & 0xFF,
           (trame->FCS >> 8) & 0xFF,
           trame->FCS & 0xFF);

    printf("--------------------------------------------\n");
}

void affichageMachine(Graphe g)
{
    int j = 0;
    printf(" Quelle station veut envoyer un message ? ");
    for (size_t i = 0; i < g.nb_equipements; i++)
    {
        if (g.equipements[i].type == STATION_TYPE)
        {
            printf("%d - MAC : ", j);
            afficherMAC(g.equipements[i].station.mac);
            j++;
        }
    }
}
int validerStationInput(Graphe g)
{
    // À FAIRE
    int station;

    return station;
}