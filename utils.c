#include "reseau.h"

void afficherIP(AdresseIP ip)
{
    printf("Adresse IP: %s \n", ip);
}

void afficherMacHexa(uint64_t mac)
{
    printf("Adresse MAC en Hexa: %#lx\n", mac);
}

char *obtenirMACString(uint64_t mac)
{
    static char buffer[18]; // 17 characters + null terminator
    uint8_t bytes[6];

    for (int i = 0; i < 6; i++)
    {
        bytes[5 - i] = (mac >> (i * 8)) & 0xFF;
    }

    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            bytes[0], bytes[1], bytes[2],
            bytes[3], bytes[4], bytes[5]);

    return buffer;
}
void afficherSwitch(Switch sw)
{
    printf("╔══════════════════════════════════════╗\n");
    printf("║               SWITCH                 ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║ Adresse MAC     : %-17s  ║\n", obtenirMACString(sw.mac));
    printf("║ Nombre de ports : %-17ld  ║\n", sw.nbPorts);
    printf("║ Priorité        : %-17ld  ║\n", sw.priorite);
    printf("╚══════════════════════════════════════╝\n");
}

void afficherSation(Station s)
{
    printf("╔══════════════════════════════════════╗\n");
    printf("║               Station                ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║ Adresse MAC     : %-17s  ║\n", obtenirMACString(s.mac));
    printf("║ Nombre de ports : %-17s  ║\n", s.ip);
    printf("╚══════════════════════════════════════╝\n");
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

int nbSwitchReseaux(Graphe g)
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
    printf("%s\n", obtenirMACString(trame->Dest));
    printf("Adresse MAC Destination : ");
    printf("%s\n", obtenirMACString(trame->Src));
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

int validerStationInput(Graphe g)
{
    // À FAIRE
    int station = -1;
    scanf("%d", &station);
    if (station < 0 || station > nbStationReusax(g))
    {
        printf("Invalide input\n");
        return EXIT_FAILURE;
    }
    return station;
}

// Menu
// Désactive le mode canonique (lecture ligne par ligne) et l'affichage des touches tapées
// Mode canonique = lecture ligne par ligne (entrée reçue après "Entrée")
// Mode non canonique = lecture caractère par caractère (réagit tout de suite, ex: flèches)
// On désactive ICANON pour rendre le menu interactif (navigation avec flèches)
// On désactive aussi ECHO pour ne pas afficher les touches tapées
void desactiverEntreeBufferisee(struct termios *ancien)
{
    struct termios nouveau;
    // On sauvegarde l'ancien mode
    tcgetattr(STDIN_FILENO, ancien);
    nouveau = *ancien;
    // On désactive ICANON (lecture caractère par caractère) et ECHO (ne pas afficher les touches)
    nouveau.c_lflag &= ~(ICANON | ECHO);
    // On applique le nouveau mode
    tcsetattr(STDIN_FILENO, TCSANOW, &nouveau);
}

// Restaure l'ancien mode d'entrée (important après le menu pour ne pas bloquer le terminal)
void restaurerEntree(struct termios *ancien)
{
    tcsetattr(STDIN_FILENO, TCSANOW, ancien);
}

// Affiche le menu en console
// Si une option est sélectionnée, on l'affiche en vert avec ">"
void afficherMenu(char **menu, int nbOptions, int selection, int dejaSelectionner)
{
    for (int i = 0; i < nbOptions; i++)
    {
        if (i == selection)
        {
            if (dejaSelectionner != i)
            {
                // Option sélectionnée → en vert
                printf("> \033[1;32m %s\033[0m\n", menu[i]);
            }
        }
        else
            // Option non sélectionnée → affichage normal
            printf("%s\n", menu[i]);
    }
}

// Menu interactif : permet de naviguer avec les flèches haut/bas
// Retourne l'index de l'option choisie quand on appuie sur Entrée
int menuInteractif(char **menu, int nbOptions, int dejaSelectionner)
{
    struct termios ancien;
    desactiverEntreeBufferisee(&ancien); // On passe en mode lecture caractère par caractère

    int selection = 0; // L'option actuellement sélectionnée (indice)
    char c;

    while (1)
    {
        // Efface l'écran à chaque boucle (pour "rafraîchir" l'affichage du menu)
        // Séquence d'échappement ANSI pour effacer l'écran et remettre le curseur en haut à gauche :
        // \033[H → place le curseur à la position (1,1)
        // \033[J → efface l'écran à partir de la position du curseur
        // Utilisé pour redessiner proprement le menu à chaque boucle
        printf("\033[H\033[J");
        printf("== Menu ==\n\n");
        if (dejaSelectionner > -1)
            printf("À qui vous voulez envoyer\n");
        else
            printf("Quelle machine veut envoyer\n");
        afficherMenu(menu, nbOptions, selection, dejaSelectionner); // Affiche le menu

        c = getchar();
        if (c == 27) // Si on a reçu le caractère ESC (code 27), cela signifie qu'une touche spéciale a été pressée (ex: flèches directionnelles) par example flèche haut : ESC [ A
        {
            getchar(); // On lit et ignore le caractère suivant '[' (fait partie de la séquence d'échappement des flèches)

            switch (getchar()) // On lit le 3ème caractère qui indique quelle flèche a été pressée :
                               // 'A' → Flèche Haut
                               // 'B' → Flèche Bas
            {
            case KEY_UP:
                // Flèche haut → on remonte dans le menu
                selection = (selection - 1 + nbOptions) % nbOptions;
                break;
            case KEY_DOWN:
                // Flèche bas → on descend dans le menu
                selection = (selection + 1) % nbOptions;
                break;
            }
        }
        else if (c == KEY_ENTER)
        {
            // Touche Entrée → on valide le choix → on sort de la boucle
            break;
        }
    }

    restaurerEntree(&ancien); // On remet le terminal en mode normal
    return selection;         // On retourne le choix de l'utilisateur
}
int affichageMachineMenu(Graphe g, int optionDejaSelectionner)
{
    // 1. Compter le nombre de stations
    int nbStations;
    if (optionDejaSelectionner > 0)
        nbStations = nbStationReusax(g) - 1;
    else
        nbStations = nbStationReusax(g);

    // 2. Allouer un tableau de chaînes pour le menu
    char **menu = malloc(sizeof(char *) * nbStations);
    // → On alloue aussi un tableau pour mémoriser les index dans g.equipements
    int *indexEquipements = malloc(sizeof(int) * nbStations);

    // 3. Construire le menu avec les adresses MAC
    int j = 0;
    for (size_t i = 0; i < g.nb_equipements; i++)
    {
        if (g.equipements[i].type == STATION_TYPE)
        {
            if (i != (size_t)optionDejaSelectionner)
            {
                char *macStr = obtenirMACString(g.equipements[i].station.mac);
                char ligneMenu[100];
                sprintf(ligneMenu, "Station MAC : %s", macStr);
                menu[j] = strdup(ligneMenu); // On copie la chaîne dans le tableau menu
                indexEquipements[j] = i;     // On mémorise l'index dans g.equipements !
                                             // free(macStr);                // On libère la mémoire temporaire
                j++;
            }
        }
    }

    // 4. Appeler le menu interactif
    int choix = menuInteractif(menu, nbStations, optionDejaSelectionner);

    // 5. On récupère l'index réel dans g.equipements
    int indexDansGraphe = indexEquipements[choix];

    // 6. Libérer la mémoire
    for (int i = 0; i < nbStations; i++)
    {
        free(menu[i]);
    }
    free(menu);
    free(indexEquipements);

    // 7. Retourner le vrai index dans g.equipements
    return indexDansGraphe;
}

void afficherPortsSwitch(Graphe *g, Switch sw)
{
    printf("\n[Switch %s] Ports and Physical Connections:\n", obtenirMACString(sw.mac));

    for (size_t i = 0; i < sw.nbPorts; i++)
    {
        Port port = sw.ports[i];

        printf("  Port %zu: ", port.portId); // Use %zu for size_t
        printf("Connected to Station %s\n", obtenirMACString(g->equipements[port.connectedEquipementIndex].station.mac));
    }
}

int firstSwitchFound(Graphe g, size_t indexMachine)
{
    size_t N = g.nb_aretes;
    for (size_t i = 0; i < N; i++)
    {
        if (g.aretes[i].index_e1 == indexMachine)
        {
            if (g.equipements[g.aretes[i].index_e2].type == SWITCH_TYPE)
                return g.aretes[i].index_e2;
        }
        else if (g.aretes[i].index_e2 == indexMachine)
        {
            if (g.equipements[g.aretes[i].index_e1].type == SWITCH_TYPE)
                return g.aretes[i].index_e1;
        }
    }
    return -1; // Aucun switch trouvé
}

int surQuelPortConnecter(Graphe g, int senderIndex, int switchIndex)
{
    if (switchIndex < 0 || (size_t)switchIndex >= g.nb_equipements)
        return -1;

    Switch *sw = &g.equipements[switchIndex].sw;

    for (size_t i = 0; i < sw->nbPorts; i++)
    {
        if (sw->ports[i].connectedEquipementIndex == (size_t)senderIndex)
        {
            return (int)i; // Found the correct port
        }
    }

    return -1; // Not connected to this switch
}

void afficherReseau(const Graphe *reseau)
{
    printf("\n========== Réseau ==========\n\n");

    for (size_t i = 0; i < reseau->nb_equipements; i++)
    {
        const Equipement *eq = &reseau->equipements[i];
        if (eq->type == STATION_TYPE)
        {
            // Station
            afficherSation(eq->station);
        }
        else if (eq->type == SWITCH_TYPE)
        {
            // Switch
            const Switch *sw = &eq->sw;
            afficherSwitch(*sw);

            printf("  Table de commutation (%zu entrées) :\n", sw->nbTable);

            // Top border
            printf("  ┌────────────┬───────────────────┐\n");

            // Header row
            printf("  │ %-10s │ %-17s │\n", "Port", "Adresse MAC");

            // Header-data separator
            printf("  ├────────────┼───────────────────┤\n");

            // Rows
            for (size_t j = 0; j < sw->nbTable; j++)
            {
                printf("  │ %-10d │ %-17s │\n", (int)sw->tableCommutation[j].nbPort,
                       obtenirMACString(sw->tableCommutation[j].AdresseMAC));
            }

            // Bottom border
            printf("  └────────────┴───────────────────┘\n\n");
        }
        else
        {
            printf("Equipement #%zu : Type inconnu\n\n", i);
        }
    }
    printf("===========================\n");
}

//Pour Dijkstra

uint64_t genererBID(uint16_t priorite, uint64_t mac) {
    return ((uint64_t)priorite << 48) | (mac & 0xFFFFFFFFFFFF);
}

size_t sommets_adjacents(Graphe const *g, size_t s, size_t sa[])
{
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)
    int res = 0;
    size_t nbSwitches = nbSwitchReseaux(*g);
    for(size_t i=0;i<nbSwitches;i++){
        if(existe_arete(g,(Arete){s,i,0})){
            sa[res]=i;
            res++;
        }
    }
    return res;
}
bool existe_arete(Graphe const *g, Arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes
    for(size_t i=0;i<g->nb_aretes;i++){
        if((g->aretes[i].index_e1==a.index_e1&&g->aretes[i].index_e2==a.index_e2)||(g->aretes[i].index_e2==a.index_e1&&g->aretes[i].index_e1==a.index_e2)){
            return true;
        }
    }
    return false;
}
