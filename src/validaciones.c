#include <ctype.h>
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
    char linea[TAMANIO_LINEA];
    char *fin;
    double valor;
    const char *inicio;

    printf("%s", mensaje);
    while (1) {
        if (!leer_linea(linea, TAMANIO_LINEA)) {
            printf("Entrada inválida. Ingrese un número positivo: ");
            continue;
        }

        inicio = linea;
        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }
        if (*inicio == '\0') {
            printf("Entrada inválida. Ingrese un número positivo: ");
            continue;
        }

        fin = NULL;
        valor = strtod(inicio, &fin);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }
        if (fin == inicio || *fin != '\0') {
            printf("Entrada inválida. Ingrese un número positivo: ");
            continue;
        }

        if (!(valor >= minimo && valor <= maximo)) {
            printf("%s ", aviso_rango);
            continue;
        }

        return valor;
    }
}

char leer_caracter(const char *mensaje, const char *opciones_validas, const char *aviso_invalido)
{
    char linea[TAMANIO_LINEA];
    const char *inicio;
    const char *fin;
    char letra;

    printf("%s", mensaje);
    while (1) {
        if (!leer_linea(linea, TAMANIO_LINEA)) {
            printf("%s ", aviso_invalido);
            continue;
        }

        inicio = linea;
        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }

        /* debe ser exactamente un carácter no-espacio */
        if (*inicio == '\0') {
            printf("%s ", aviso_invalido);
            continue;
        }
        fin = inicio + 1;
        while (isspace((unsigned char)*fin)) {
            fin++;
        }
        if (*fin != '\0') {
            printf("%s ", aviso_invalido);
            continue;
        }

        letra = (char)tolower((unsigned char)*inicio);
        if (strchr(opciones_validas, letra) == NULL) {
            printf("%s ", aviso_invalido);
            continue;
        }

        return letra;
    }
}
