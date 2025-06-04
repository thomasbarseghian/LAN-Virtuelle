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
        // printf("---ARÊTES---\n");
        // for (size_t i = 0; i < g.nb_aretes; i++)
        // {
        //     printf("%ld-----%ld :poids: %d\n", g.aretes[i].index_e1, g.aretes[i].index_e2, g.aretes[i].poids);
        // }
        // mettreJourTableCommutation(g);
        // afficherMAC(g.equipements[0].sw.mac);
        EthernetTram t = {
            .preambule = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
            .SFD = 0xAB,
            .Dest = 0x001A2B3C4D5E,
            .Src = 0x0066778899AA,
            .type = 0x0800,
            .donnees = "Bonjour",
            .FCS = 0x12345678};

        afficherTrameUtilisateur(&t);
        afficherTrameHexa(&t);
        affichageMachine(g);
        validerStationInput(g);
    }
    else
        printf("Error dans fichier\n");

    free(g.equipements);
    g.equipements = NULL;
    free(g.aretes);
    g.aretes = NULL;
    return 0;
}