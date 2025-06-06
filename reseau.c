#include "reseau.h"

// Fonction pour crée réseau à partie de fichier de configration
// Cette fonction prend path de fichier que utilisateur va donner quand il execute le program
// example: /main ./mylan.lan.txt
int creeReseau(const char *filePath, Graphe *reseau)
{
    // ====================
    // Étape 0 : Ouverture du fichier en mode lecture
    // ====================
    FILE *fptr = ouvrirFichier(filePath);
    if (fptr == NULL)
        return EXIT_FAILURE;

    // ====================
    // Étape 1 : Lecture du nombre d'équipements et d'arêtes
    // ====================
    int nbEquipements = 0, nbAretes = 0;
    if (lireNbEquipementsEtAretes(fptr, &nbEquipements, &nbAretes) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    // ====================
    // Étape 2 : Verifie que nombre de eqiupements et arêtes sont bonne
    // ====================
    if (verifieNbEquipementEtAretes(fptr, nbEquipements, nbAretes) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    // ====================
    // Étape 3 : Allocation mémoire pour le graphe
    // ====================
    if (initialiserReseau(reseau, nbEquipements, nbAretes) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    // ====================
    // Étape 4 : Lecture des équipements
    // ====================
    if (creerEquipements(fptr, reseau, nbEquipements) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    // ====================
    // Étape 5 : Lecture des Aretes
    // ====================
    if (creerArrets(fptr, reseau, nbAretes) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    fclose(fptr);
    return EXIT_SUCCESS;
}

FILE *ouvrirFichier(const char *filePath)
{
    FILE *fptr = fopen(filePath, "r");
    if (fptr == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier/ fichier n'existe pas.\n");
    }
    return fptr;
}
int lireNbEquipementsEtAretes(FILE *fptr, int *nbEquipements, int *nbAretes)
{
    char ligne[256];
    if (fgets(ligne, 256, fptr) == NULL)
        return EXIT_FAILURE;

    if (extraireNbEquipementsEtAretes(nbEquipements, nbAretes, ligne) == 1)
    {
        printf("Erreur dans fichier de configuration.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int verifieNbEquipementEtAretes(FILE *fptr, int nbEquipements, int nbAretes)
{
    int totalAttendu = nbEquipements + nbAretes + 1; // +1 for the header line
    int totalLignes = 0;
    char ligne[256];

    // Rembobiner pour démarrer depuis le début du fichier
    rewind(fptr);

    while (fgets(ligne, sizeof(ligne), fptr) != NULL)
    {
        totalLignes++;
    }

    if (totalLignes != totalAttendu)
    {
        printf("Erreur : Le fichier contient %d lignes, mais %d étaient attendues.\n", totalLignes, totalAttendu);
        return EXIT_FAILURE;
    }

    // Revenir à la deuxième ligne
    rewind(fptr);
    fgets(ligne, sizeof(ligne), fptr); // sauter la première ligne
    return EXIT_SUCCESS;
}
int initialiserReseau(Graphe *reseau, int nbEquipements, int nbAretes)
{
    reseau->nb_equipements = nbEquipements;
    reseau->nb_aretes = nbAretes;

    reseau->equipements = malloc(sizeof(Equipement) * nbEquipements);
    if (reseau->equipements == NULL)
    {
        printf("Erreur d'allocation de mémoire\n");
        return EXIT_FAILURE;
    }

    reseau->aretes = malloc(sizeof(Arete) * nbAretes);
    if (reseau->aretes == NULL)
    {
        printf("Erreur d'allocation de mémoire\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
int creerEquipements(FILE *fptr, Graphe *reseau, int nbEquipements)
{
    char ligne[256];

    for (int i = 0; i < nbEquipements; i++)
    {
        if (fgets(ligne, 256, fptr) == NULL)
            return EXIT_FAILURE;

        char *parties[4];
        int j = 0;
        char *partie = strtok(ligne, ";");
        while (partie != NULL && j < 4)
        {
            parties[j++] = partie;
            partie = strtok(NULL, ";");
        }

        if (j != 4 && j != 3)
        {
            printf("Erreur dans fichier de configuration.\n");
            return EXIT_FAILURE;
        }

        int typeMachine = 0;
        if (!estInteger(parties[0], &typeMachine) == EXIT_SUCCESS)
        {
            printf("Erreur : Type Machine invalide\n");
            return EXIT_FAILURE;
        }

        if (typeMachine == 2)
        {
            Switch sw;
            if (convertMacToInteger(parties[1], &(sw.mac)))
            {
                printf("Erreur : adresse MAC invalide\n");
                return EXIT_FAILURE;
            }
            int temp = 0;
            if (estInteger(parties[3], &temp) == EXIT_SUCCESS)
                sw.priorite = (size_t)temp;
            else
            {
                printf("Error partie Priorite\n");
                return EXIT_FAILURE;
            }

            temp = 0;
            if (estInteger(parties[2], &temp) == EXIT_SUCCESS)
                sw.nbPorts = (size_t)temp;
            else
            {
                printf("Error partie Ports\n");
                return EXIT_FAILURE;
            }

            sw.ports = NULL;
            sw.tableCommutation = NULL;

            Equipement e;
            e.sw = sw;
            e.type = SWITCH_TYPE;

            ajoutEquipement(reseau, &e, i);
        }
        else if (typeMachine == 1)
        {
            Station s;
            if (convertMacToInteger(parties[1], &(s.mac)))
            {
                printf("Erreur : adresse MAC invalide\n");
                return EXIT_FAILURE;
            }

            if (verifyIp(parties[2]))
            {
                printf("Erreur : adresse IP invalide\n");
                return EXIT_FAILURE;
            }
            strcpy(s.ip, parties[2]);
            Equipement e;
            e.station = s;
            e.type = STATION_TYPE;
            ajoutEquipement(reseau, &e, i);
        }
        else
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
int creerArrets(FILE *fptr, Graphe *reseau, int nbAretes)
{
    char ligne[256];

    for (int i = 0; i < nbAretes; i++)
    {
        if (fgets(ligne, 256, fptr) == NULL)
            return EXIT_FAILURE;

        char *parties[3];
        int j = 0;
        char *partie = strtok(ligne, ";");
        while (partie != NULL)
        {
            parties[j++] = partie;
            partie = strtok(NULL, ";");
        }
        if (j != 3)
        {
            printf("Erreur dans fichier de configuration. :: Aretes entre equipements :: Format n'est pas bonne \n");
            return EXIT_FAILURE;
        }
        int indexEquipment1, indexEquipment2, poidsArete;
        if (estInteger(parties[0], &indexEquipment1) != EXIT_SUCCESS)
        {
            printf("Erreur dans fichier de configuration. :: Aretes entre equipements :: Format de 1er equipment n'est pas bonne \n");
            return EXIT_FAILURE;
        }
        if (estInteger(parties[1], &indexEquipment2) != EXIT_SUCCESS)
        {
            printf("Erreur dans fichier de configuration. :: Aretes entre equipements :: Format de 2eme equipment n'est pas bonne  \n");
            return EXIT_FAILURE;
        }
        if (estInteger(parties[2], &poidsArete) != EXIT_SUCCESS)
        {
            printf("Erreur dans fichier de configuration. :: Aretes entre equipements :: Format de poids entre equipment n'est pas bonne  \n");
            return EXIT_FAILURE;
        }
        if ((indexEquipment1 < 0 || (size_t)indexEquipment1 > reseau->nb_equipements) ||
            (indexEquipment2 < 0 || (size_t)indexEquipment2 > reseau->nb_equipements))
        {
            printf("Erreur dans fichier de configuration. :: Aretes entre equipements :: Equipements n'existe pas! \n");
            return EXIT_FAILURE;
        }
        Arete a;
        a.index_e1 = indexEquipment1;
        a.index_e2 = indexEquipment2;
        a.poids = (uint8_t)poidsArete;
        ajoutArete(reseau, &a, i);
    }
    return EXIT_SUCCESS;
}

uint64_t convertMacToInteger(const char *str, uint64_t *mac)
{
    if (verifyMac(str))
    {
        printf("MAC adresse incorrecte\n");
        return EXIT_FAILURE;
    }
    *mac = 0;
    for (int i = 0; i < 6; i++)
    {
        int rightBits = hexCharToInt(str[0]);
        int leftBits = hexCharToInt(str[1]);

        if (rightBits == -1 || leftBits == -1)
        {
            printf("Erreur : caractère hexadécimal invalide\n");
            return EXIT_FAILURE;
        }
        *mac = (*mac << 8) | ((rightBits << 4) | leftBits);
        str += 2;
        if (*str == ':')
            str++;
    }
    if (*str != '\0' && *str != '\n')
    {
        printf("Erreur : caractères invalides après adresse MAC\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// Faut compléter
int mettreJourTableCommutation(Graphe g)
{
    int nbSwitches = nbSwitchReusax(g);
    printf("nbSwitches %d : \n", nbSwitches);
    for (size_t i = 0; i < g.nb_equipements; i++)
    {
        if (g.equipements[i].type == SWITCH_TYPE)
        {
            afficherSwitch(g.equipements[i].sw);
        }
    }
    return EXIT_SUCCESS;
}
