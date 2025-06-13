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
        lancerDijkstra(&g);
        afficherEtatPorts(g);
        communiquer(&g);
        afficherReseau(&g);
    }
    else
    {
        printf("Erreur dans fichier\n");
        return 1;
    }

    for (int i = 0; i < nbSwitchReseaux(g); i++)
    {
        free(g.equipements[i].sw.tableCommutation);
        free(g.equipements[i].sw.ports);
    }
    free(g.equipements);
    g.equipements = NULL;
    free(g.aretes);
    g.aretes = NULL;
    return 0;
}
