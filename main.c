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

    // printf("%lu\n", convertMacToInteger("54:d6:a6:82:c5:23"));
    if (creeReseau(argv[1]) == 0)
    {
        printf("Reseau cree avex succée\n");
    }
    else
        printf("Error dans fichier\n");

    return 0;
}