#include "reseau.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void afficherIP(AdresseIP ip);

uint64_t convertMacToHexa(char *str)
{
    uint64_t hexaMac = 0;
    if (str[0] != '2')
        return hexaMac;

    for (size_t i = 2; i < 19; i++)
    {
        printf("%c", str[i]);
    }
    return hexaMac;
}