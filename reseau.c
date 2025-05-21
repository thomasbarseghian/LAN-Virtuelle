#include "reseau.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void afficherIP(AdresseIP ip);

void convertMacToHexa(char *str)
{
    if (str[0] != '2')
        return;

    for (int i = 0; i < strlen(str); i++)
    {
        printf("%c", str[i]);
    }
}