#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

/* Comprueba la salida real del submenú, incluidos argumentos y unidades. */
static int comprobar_valor(const char *salida, const char *etiqueta,
                          double esperado, double tolerancia)
{
    const char *posicion = strstr(salida, etiqueta);
    double obtenido;

    if (posicion == NULL || sscanf(posicion + strlen(etiqueta), "%lf", &obtenido) != 1) {
        fprintf(stderr, "FALLA: falta el valor de %s\n", etiqueta);
        return 1;
    }
    if (!isfinite(obtenido) || fabs(obtenido - esperado) > tolerancia) {
        fprintf(stderr, "FALLA %s: se obtuvo %.10g y se esperaba %.10g\n",
                etiqueta, obtenido, esperado);
        return 1;
    }
    return 0;
}

int main(void)
{
    const char *entrada = "pruebas_consultas_entrada.tmp";
    const char *captura = "pruebas_consultas_salida.tmp";
    /* Reposo, velocidad de misión, velocidad suficiente y comparación de pozos. */
    const char *datos = "c\ns\nc\nn\n192000\n10900\nc\nn\n192000\n10906\nd\ns\nv\n";
    const char *etiqueta_trabajo = "Trabajo adicional minimo (W/m): ";
    const char *siguiente;
    FILE *archivo;
    char salida[16000];
    size_t leidos;
    int fallas = 0;

    archivo = fopen(entrada, "w");
    if (archivo == NULL) {
        return EXIT_FAILURE;
    }
    fputs(datos, archivo);
    fclose(archivo);
    if (freopen(entrada, "r", stdin) == NULL || freopen(captura, "w", stdout) == NULL) {
        remove(entrada);
        return EXIT_FAILURE;
    }
    submenu_consultas();
    fclose(stdin);
    fclose(stdout);
    archivo = fopen(captura, "r");
    if (archivo == NULL) {
        remove(entrada);
        return EXIT_FAILURE;
    }
    leidos = fread(salida, 1, sizeof(salida) - 1, archivo);
    salida[leidos] = '\0';
    fclose(archivo);
    remove(entrada);
    remove(captura);

    fallas += comprobar_valor(salida, etiqueta_trabajo, 59462086.0, 1.0);
    fallas += comprobar_valor(salida, "U(r0)/m: ", -60743034.0, 1.0);
    fallas += comprobar_valor(salida, "U(x_eq)/m: ", -1280948.0, 1.0);
    siguiente = strstr(salida, etiqueta_trabajo);
    siguiente = siguiente == NULL ? "" : siguiente + strlen(etiqueta_trabajo);
    fallas += comprobar_valor(siguiente, etiqueta_trabajo, 57086.0, 1.0);
    siguiente = strstr(siguiente, etiqueta_trabajo);
    siguiente = siguiente == NULL ? "" : siguiente + strlen(etiqueta_trabajo);
    fallas += comprobar_valor(siguiente, etiqueta_trabajo, 0.0, 0.0);
    fallas += comprobar_valor(salida, "Pozo gravitatorio terrestre: ", 60.73, 0.01);
    fallas += comprobar_valor(salida, "Pozo gravitatorio lunar: ", 2.82, 0.01);
    fallas += comprobar_valor(salida, "Energia total ida: ", 1.579e12, 1e9);
    fallas += comprobar_valor(salida, "Energia total regreso: ", 7.337e10, 1e7);
    fprintf(stderr, "9 pruebas de consultas, %d fallas\n", fallas);
    return fallas == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
