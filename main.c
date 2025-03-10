#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char solicitud[50];
    char resultado[50];

    strcpy(solicitud, "1/Miguel/47/2/Juan/12/3/Maria/22/1/Marcos/19");

    char nombrePersona[10];
    int tipoServicio;
    int edadPersona;
    char esPalindromo[10];
    char *token = strtok(solicitud, "/");

    while(token != NULL)
    {
        tipoServicio = atoi(token);
        token = strtok(NULL, "/");
        strcpy(nombrePersona, token);
        token = strtok(NULL, "/");
        edadPersona = atoi(token);

        if (tipoServicio == 1)
        {
            if(edadPersona > 18)
            {
                sprintf(resultado, "%s%s*%d*-", resultado, nombrePersona, edadPersona);
            }
        }
        else if (tipoServicio == 2)
        {
            int longitudNombre = strlen(nombrePersona);
            strcpy(esPalindromo, "SI");
            for (int i = 0; i < longitudNombre / 2; i++) 
            {
                if (tolower(nombrePersona[i]) != tolower(nombrePersona[longitudNombre - i - 1])) 
                {
                    strcpy(esPalindromo, "NO");
                    break;
                }
            }
            sprintf(resultado, "%s%s*%s*-", resultado, nombrePersona, esPalindromo);
        }
        else 
        {
            int longitudNombre = strlen(nombrePersona);
            for (int i = 0; i < longitudNombre; i++) 
            {
                nombrePersona[i] = toupper(nombrePersona[i]);
            }
            sprintf(resultado, "%s*%s-", resultado, nombrePersona);
        }
        token = strtok(NULL, "/");
    }
    resultado[strlen(resultado) - 1] = '\0';

    printf("Resultado: %s\n", resultado);
}

