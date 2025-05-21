#include "reseau.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

int main()
{
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen("reseaux.txt", "r");

    // Read the content and print it
    char myString[100];
    while (fgets(myString, 100, fptr))
    {
        if (myString[0] == '2')
        {
            convertMacToHexa(myString);
        }
    }
}