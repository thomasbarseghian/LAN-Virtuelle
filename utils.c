#include "reseau.h"

void afficherIP(AdresseIP ip)
{
    printf("Adresse IP: %s \n", ip);
}

void afficherMacHexa(const char *str)
{
    uint64_t mac;
    printf("Mac  Hexa Adresse : %lx\n", convertMacToInteger(str, &mac));
}

void afficherSwitch(Switch sw)
{
    printf("Adresse Mac : %lu\n", sw.mac);
    printf("Nombre de ports : %ld\n", sw.nbPorts);
    printf("Priorité : %ld\n", sw.priorite);
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

void ajoutEquipement(Graphe *reseau, Equipement e)
{
    reseau->equipements[reseau->nb_equipements++] = e;
}