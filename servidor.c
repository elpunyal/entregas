#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int socketEscucha, socketConexion, resultado;
    struct sockaddr_in direccionServidor;
    char solicitud[256];
    char resultadoConversion[256];

    if((socketEscucha = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Error creando socket");

    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;

    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);

    direccionServidor.sin_port = htons(9050);

    if(bind(socketEscucha, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0)
        printf("Error al bindear");

    if(listen(socketEscucha, 3) < 0)
        printf("Error en el listen");

    printf("Escuchando\n");

    while(1)
    {
        socketConexion = accept(socketEscucha, NULL, NULL);
        printf("He recibido conexión\n");

        resultado = read(socketConexion, solicitud, sizeof(solicitud));
        printf("Recibido\n");

        solicitud[resultado] = '\0';

        printf("Solicitud: %s\n", solicitud);

        float temperatura;
        int tipoServicio;
        char *token = strtok(solicitud, "/");

        while(token != NULL)
        {   
            tipoServicio = atoi(token);
            token = strtok(NULL, "/");
            temperatura = atof(token);

            if(tipoServicio == 1)
            {
                temperatura = (temperatura * 9/5.0) + 32;
                sprintf(resultadoConversion, "%s%.1f-", resultadoConversion, temperatura);
            }
            if(tipoServicio == 2)
            {
                temperatura = (temperatura - 32) * 5/9.0;
                sprintf(resultadoConversion, "%s%.1f-", resultadoConversion, temperatura);
            }
            token = strtok(NULL, "/");
        }
        resultadoConversion[strlen(resultadoConversion) - 1] = '\0';

        printf("Resultado: %s\n", resultadoConversion);
        write(socketConexion, resultadoConversion, strlen(resultadoConversion));
    
        close(socketConexion);
      ¡
    }
}
