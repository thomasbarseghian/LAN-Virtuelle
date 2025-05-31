#include "reseau.h"

// Fonction pour crée réseau à partie de fichier de configration
// Cette fonction prend path de fichier que utilisateur va donner quand il execute le program
// example: /main ./reseaux.txt
int creeReseau(const char *filePath)
{
    FILE *fptr;
    // Ouvrir un fichier en mode lecture
    fptr = fopen(filePath, "r");
    if (fptr == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier/ fichier n'existe pas.\n");
        return EXIT_FAILURE;
    }

    // Nous lisons le fichier ligne par ligne

    int nbEquipements = -1;
    int nbAretes = -1;
    char ligne[256];

    // Premieur Ligne
    fgets(ligne, 256, fptr);
    if (extraireNbEquipementsEtAretes(&nbEquipements, &nbAretes, ligne) == 1)
    {
        printf("Erreur dans fichier de configuration.\n");
        return EXIT_FAILURE;
    }

    // Crée Graph
    Graphe reseau;
    reseau.nb_equipements = nbEquipements;
    reseau.nb_aretes = nbAretes;
    reseau.equipements = malloc(sizeof(Equipement) * nbEquipements);
    if (reseau.equipements == NULL)
    {
        printf("Erreur d'allocation de mémoire");
        return EXIT_FAILURE;
    }
    reseau.aretes = malloc(sizeof(Arete) * nbAretes);
    if (reseau.aretes == NULL)
    {
        printf("Erreur d'allocation de mémoire");
        return EXIT_FAILURE;
    }

    // Creation des Equipements
    for (int i = 0; i < nbEquipements; i++)
    {
        fgets(ligne, 256, fptr);

        char *parties[4]; // il y a 4 partie different au max

        int j = 0;
        char *partie = strtok(ligne, ";");
        while (partie != NULL && j < 4)
        {
            parties[j++] = partie;
            partie = strtok(NULL, ";"); // prochain partie
        }

        // valider que nous avons exactement 3 ou 4 parties
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

            // Creation de Switch
            Switch sw;
            if (convertMacToInteger(parties[1], &(sw.mac)))
            {
                printf("Erreur : adresse MAC invalide\n");
                return EXIT_FAILURE;
            }
            int temp = 0;
            if (estInteger(parties[3], &temp) == EXIT_SUCCESS)
            {
                sw.priorite = (size_t)temp;
            }
            else
            {
                printf("Error partie Priorite\n");
                return EXIT_FAILURE;
            }
            temp = 0;
            if (estInteger(parties[2], &temp) == EXIT_SUCCESS)
            {
                sw.nbPorts = (size_t)temp;
            }
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
            ajoutEquipement(&reseau, e);
        }
        else if (typeMachine == 1)
        {
            // Creation de Machine
            Station s;
            if (convertMacToInteger(parties[1], &(s.mac)))
            {
                printf("Erreur : adresse MAC invalide\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            return EXIT_FAILURE;
        }
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