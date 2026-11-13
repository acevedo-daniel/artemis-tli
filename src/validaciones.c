#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validaciones.h"

#define TAMANIO_LINEA 100

/* Lee una línea sin el salto final. Devuelve 1 si entró completa y 0 si era demasiado larga.
   Ante el fin de la entrada termina el programa, para no reintentar para siempre. */
static int leer_linea(char *linea, int tamanio)
{
    size_t largo;
    int caracter;

    if (fgets(linea, tamanio, stdin) == NULL) {
        printf("\nNo hay más datos de entrada. El programa se cierra.\n");
        exit(EXIT_SUCCESS);
    }
    largo = strlen(linea);
    if (largo > 0 && linea[largo - 1] == '\n') {
        linea[--largo] = '\0';
        if (largo > 0 && linea[largo - 1] == '\r') {
            linea[--largo] = '\0';
        }
        return 1;
    }
    if (feof(stdin)) {
        return 1;
    }
    while ((caracter = getchar()) != '\n' && caracter != EOF) {
        /* descarta el resto de la línea */
    }
    return 0;
}

int leer_entero(const char *mensaje, int minimo, int maximo, const char *aviso_rango)
{
    char linea[TAMANIO_LINEA];
    int valor;

    printf("%s", mensaje);
    while (1) {
        if (leer_linea(linea, TAMANIO_LINEA) && sscanf(linea, "%d", &valor) == 1
            && valor >= minimo && valor <= maximo) {
            return valor;
        }
        printf("%s ", aviso_rango);
    }
}

double leer_real(const char *mensaje, double minimo, double maximo, const char *aviso_rango)
{
    (void)mensaje;
    (void)maximo;
    (void)aviso_rango;
    return minimo;
}

char leer_caracter(const char *mensaje, const char *opciones_validas, const char *aviso_invalido)
{
    (void)mensaje;
    (void)aviso_invalido;
    return opciones_validas[0];
}
