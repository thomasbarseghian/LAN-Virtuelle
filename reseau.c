#include "reseau.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void afficherIP(AdresseIP ip);

uint32_t convertIpToInterger(char *str)
{
    uint32_t ip = 0;
    if (str[0] != '1')
        return ip;

    for (size_t i = 20; i < strlen(str); i++)
    {
        printf("%c", str[i]);
    }
    return ip;
}