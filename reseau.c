#include "reseau.h"

// Fonction pour crée un réseau à partir de fichier de configration
// Cette fonction prend le chemnin du fichier que utilisateur va donner quand il execute le programme
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
    // Étape 2 : Verifie que le nombre de eqiupements et arêtes est bon
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

    remplirTablePort(reseau);

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
            {
                sw.nbPorts = (size_t)temp;
                sw.maxCapacityTable = (size_t)temp;
            }
            else
            {
                printf("Error partie Ports\n");
                return EXIT_FAILURE;
            }

            sw.ports = malloc(sizeof(Port) * sw.nbPorts);
            sw.tableCommutation = malloc(sizeof(tableCommutation) * sw.maxCapacityTable);
            sw.nbTable = 0;
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
void remplirTablePort(Graphe *g)
{
    size_t nbSwitches = nbSwitchReseaux(*g);
    for (size_t i = 0; i < nbSwitches; i++)
    {
        int k = 0;
        Switch *sw = &g->equipements[i].sw;

        for (size_t p = 0; p < sw->nbPorts; p++)
        {
            sw->ports[p].connectedEquipementIndex = -1;
            sw->ports[p].portId = -1;
        }

        for (size_t j = 0; j < g->nb_aretes; j++)
        {
            if (g->aretes[j].index_e1 == i)
            {
                printf("Switch %s is connected on port %d with %s\n",
                       obtenirMACString(sw->mac), k,
                       obtenirMACString(g->equipements[g->aretes[j].index_e2].station.mac));
                sw->ports[k].connectedEquipementIndex = g->aretes[j].index_e2;
                sw->ports[k].portId = k;
                k++;
            }
            else if (g->aretes[j].index_e2 == i)
            {
                printf("Switch %s is connected on port %d with %s\n",
                       obtenirMACString(sw->mac), k,
                       obtenirMACString(g->equipements[g->aretes[j].index_e1].station.mac));
                sw->ports[k].connectedEquipementIndex = g->aretes[j].index_e1;
                sw->ports[k].portId = k;
                k++;
            }
        }
    }
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

void mettreAJourTableCommutation(Switch *sw, AdresseMAC mac, size_t portId)
{
    for (size_t i = 0; i < sw->nbTable; i++)
    {
        if (sw->tableCommutation[i].AdresseMAC == mac)
            return;
    }

    if (sw->nbTable < sw->maxCapacityTable)
    {
        sw->tableCommutation[sw->nbTable].AdresseMAC = mac;
        sw->tableCommutation[sw->nbTable].nbPort = portId;
        sw->nbTable++;

        printf("Switch learned MAC %s on port %zu\n", obtenirMACString(mac), portId);
    }
    else
    {
        printf("Table full, cannot add MAC %s\n", obtenirMACString(mac));
    }
}
// Fonction principale pour initier la communication entre deux machines
void communiquer(Graphe *g)
{
    // L'utilisateur choisit la machine source
    int sender = affichageMachineMenu(*g, -1);

    // L'utilisateur choisit la machine destination (différente de la source)
    int receiver = affichageMachineMenu(*g, sender);

    // Construction de la trame Ethernet
    EthernetTram t = {
        .preambule = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
        .SFD = 0xAB,
        .Dest = g->equipements[receiver].sw.mac, // Adresse MAC de destination
        .Src = g->equipements[sender].sw.mac,    // Adresse MAC source
        .type = 0x0800,
        .donnees = "Bonjour", // Message contenu dans la trame
        .FCS = 0x12345678};

    // Envoi de la trame du sender vers le receiver
    envoyerTram(g, sender, receiver, &t);
}

// Fonction récursive pour transmettre la trame à travers les switches
int envoyerTramRec(Graphe *g, int currentSwitchIndex, int cameFromEquipIndex, EthernetTram *t)
{
    Switch *sw = &g->equipements[currentSwitchIndex].sw;

    printf("  Passage par le switch %d (MAC %s)\n", currentSwitchIndex, obtenirMACString(sw->mac));

    // Vérifie si la table de commutation contient déjà l’adresse MAC de destination
    for (size_t i = 0; i < sw->nbTable; i++)
    {
        if (sw->tableCommutation[i].AdresseMAC == t->Dest)
        {
            int knownPort = sw->tableCommutation[i].nbPort;
            int destIndex = sw->ports[knownPort].connectedEquipementIndex;

            if (g->equipements[destIndex].type == STATION_TYPE)
            {
                printf("   Trame reçue par la machine (MAC: %lx)\n", t->Dest);
                printf("   Contenu de la trame :\n");
                printf("   De     : %lx\n", t->Src);
                printf("   Vers   : %lx\n", t->Dest);
                printf("   Données: %s\n", t->donnees);
                return 1;
            }
            else
            {
                // Recurse vers le switch suivant
                return envoyerTramRec(g, destIndex, currentSwitchIndex, t);
            }
        }
    }

    // Si la MAC n'est pas connue, on apprend la MAC source
    for (size_t i = 0; i < sw->nbPorts; i++)
    {
        if (sw->ports[i].connectedEquipementIndex == (size_t)cameFromEquipIndex)
        {
            mettreAJourTableCommutation(sw, t->Src, i);
            break;
        }
    }

    // Diffusion (broadcast) de la trame sur les autres ports
    for (size_t i = 0; i < sw->nbPorts; i++)
    {
        int neighbor = sw->ports[i].connectedEquipementIndex;

        if (neighbor == cameFromEquipIndex)
            continue; // on évite de revenir en arrière

        if (g->equipements[neighbor].type == STATION_TYPE)
        {
            if (g->equipements[neighbor].station.mac == t->Dest)
            {
                printf("   Trame reçue par la machine (MAC: %lx)\n", t->Dest);
                printf("   Contenu de la trame :\n");
                printf("   De     : %lx\n", t->Src);
                printf("   Vers   : %lx\n", t->Dest);
                printf("   Données: %s\n", t->donnees);
                return 1;
            }
        }
        else
        {
            // Envoi récursif vers le switch connecté
            if (envoyerTramRec(g, neighbor, currentSwitchIndex, t))
                return 1; // stop si reçu
        }
    }

    return 0; // Trame non délivrée
}

// Fonction qui initialise l’envoi d’une trame depuis une machine vers une autre
int envoyerTram(Graphe *g, int senderIndex, int receiverIndex, EthernetTram *t)
{
    int senderSwitch = firstSwitchFound(*g, senderIndex);
    int portOnSwitchSender = surQuellePortConnecter(*g, senderIndex, senderSwitch);

    if (senderSwitch == -1 || portOnSwitchSender == -1)
    {
        printf("Erreur: machine %d n'est pas connectée à un switch\n", senderIndex);
        return EXIT_FAILURE;
    }

    // Mise à jour de la table de commutation du switch source avec l’adresse MAC de l’émetteur
    mettreAJourTableCommutation(&g->equipements[senderSwitch].sw, t->Src, portOnSwitchSender);

    // Envoi réel de la trame
    if (!envoyerTramRec(g, senderSwitch, senderIndex, t))
    {
        printf("Le message n'a pas pu être délivré\n");
    }

    return EXIT_SUCCESS;
}
