#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"

#define RADIO_ORBITA_BAJA (RADIO_TIERRA + ALTITUD_DEFECTO)

static int total = 0;
static int fallas = 0;

/* Cuenta una prueba y muestra el detalle si el valor obtenido se aleja del esperado. */
static void comprobar(const char *nombre, double obtenido, double esperado, double tolerancia)
{
    total++;
    if (fabs(obtenido - esperado) > tolerancia) {
        fallas++;
        printf("FALLA %s: se obtuvo %.10g y se esperaba %.10g\n", nombre, obtenido, esperado);
    }
}

static void probar_energia_potencial(void)
{
    double velocidad = 0.0;
    int llega;

    comprobar("punto de equilibrio", calcular_punto_equilibrio(), 345662974.0, 1.0);
    comprobar("U(r0)/m", calcular_energia_potencial_por_kg(RADIO_ORBITA_BAJA), -60743034.0, 1.0);
    comprobar("U(x_eq)/m", calcular_energia_potencial_por_kg(calcular_punto_equilibrio()), -1280948.0, 1.0);
    comprobar("U con distancia 0", calcular_energia_potencial_por_kg(0.0), 0.0, 0.0);
    comprobar("U con distancia D", calcular_energia_potencial_por_kg(DISTANCIA_TIERRA_LUNA), 0.0, 0.0);

    llega = calcular_velocidad_en(RADIO_ORBITA_BAJA, 10900.0, 176112987.0, &velocidad);
    comprobar("llega al punto 5", llega, 1, 0);
    comprobar("velocidad en el punto 5", velocidad, 1377.47, 0.01);
    llega = calcular_velocidad_en(RADIO_ORBITA_BAJA, 10900.0, calcular_punto_equilibrio(), &velocidad);
    comprobar("con 10 900 m/s no llega al equilibrio", llega, 0, 0);
    llega = calcular_velocidad_en(RADIO_ORBITA_BAJA, 10900.0, DISTANCIA_TIERRA_LUNA, &velocidad);
    comprobar("distancia D es inválida", llega, 0, 0);
}

static void probar_atracciones_y_telemetria(void)
{
}

static void probar_energia_y_pozos(void)
{
}

static void probar_trabajo_minimo_y_distancia_maxima(void)
{
}

static void probar_estado_nave(void)
{
    EstadoNave nave;
    double velocidades[CANTIDAD_TRAMOS + 1];

    comprobar("distancia del punto 0", calcular_distancia_punto(0), 6563000.0, 1.0);
    comprobar("distancia del punto 5", calcular_distancia_punto(5), 176112987.0, 1.0);
    comprobar("distancia del punto 10", calcular_distancia_punto(10), calcular_punto_equilibrio(), 1.0);
    comprobar("punto inexistente", calcular_distancia_punto(11), 0.0, 0.0);

    inicializar_nave(&nave, 10900.0, 150000.0);
    comprobar("último punto con 10 900 m/s", recorrer_sin_correcciones(&nave, velocidades), 9, 0);
    comprobar("velocidad en el punto 5 con 10 900 m/s", velocidades[5], 1377.47, 0.01);
    comprobar("velocidad en el punto 9 con 10 900 m/s", velocidades[9], 128.93, 0.01);

    inicializar_nave(&nave, 10906.0, 150000.0);
    comprobar("último punto con 10 906 m/s", recorrer_sin_correcciones(&nave, velocidades), 10, 0);
    comprobar("velocidad en el equilibrio con 10 906 m/s", velocidades[10], 129.09, 0.01);

    inicializar_nave(&nave, 10900.0, 150000.0);
    comprobar("corrección mayor que la reserva", aplicar_correccion(&nave, 200000.0), 0, 0);
    comprobar("corrección negativa", aplicar_correccion(&nave, -5.0), 0, 0);

    inicializar_nave(&nave, 10900.0, 150000.0);
    recorrer_sin_correcciones(&nave, velocidades);
    comprobar("corrección a tiempo", aplicar_correccion(&nave, 57100.0), 1, 0);
    comprobar("reserva tras la corrección", nave.reserva, 92900.0, 1e-6);
    comprobar("trabajo aplicado", nave.trabajo_aplicado, 57100.0, 1e-6);
    comprobar("correcciones usadas", nave.correcciones, 1, 0);
    comprobar("avanza al equilibrio tras corregir", avanzar_nave(&nave), 1, 0);
    comprobar("velocidad al llegar tras corregir", nave.velocidad, 5.35, 0.01);
    comprobar("no avanza más allá del equilibrio", avanzar_nave(&nave), 0, 0);
}

static void probar_eventos_y_puntaje(void)
{
}

int main(void)
{
    probar_energia_potencial();
    probar_atracciones_y_telemetria();
    probar_energia_y_pozos();
    probar_trabajo_minimo_y_distancia_maxima();
    probar_estado_nave();
    probar_eventos_y_puntaje();

    printf("%d pruebas, %d fallas\n", total, fallas);
    return fallas == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
