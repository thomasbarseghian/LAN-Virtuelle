#include "reseau.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <chemin_du_fichier>\n", argv[0]);
        return 1;
    }
    Graphe g;

    // printf("%lu\n", convertMacToInteger("54:d6:a6:82:c5:23"));
    if (creeReseau(argv[1], &g) == 0)
    {
        printf("Reseau cree avex succée\n");
        // for (size_t i = 0; i < g.nb_equipements; i++)
        // {
        //     if (g.equipements[i].type == STATION_TYPE)
        //     {
        //         afficherSation(g.equipements[i].station);
        //     }
        //     else
        //     {
        //         afficherSwitch(g.equipements[i].sw);
        //     }
        // }
    }
    else
        printf("Error dans fichier\n");

    free(g.equipements);
    g.equipements = NULL;
    free(g.aretes);
    g.aretes = NULL;
    return 0;
}