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
    EstadoNave nave = {0, 6563000.0, 10900.0, 150000.0, 0.0, 0};
    Telemetria telemetria;

    calcular_telemetria(&nave, &telemetria);

    comprobar("a_T en r0",              calcular_atraccion_tierra(6563000.0),  9.253397,    1e-6);
    comprobar("a_L en r0",              calcular_atraccion_luna(6563000.0),    3.44150e-5,  1e-9);
    comprobar("a_T en el punto 5",      calcular_atraccion_tierra(176113000.0), 0.01285059, 1e-8);
    comprobar("a_L en el punto 5",      calcular_atraccion_luna(176113000.0),  1.134442e-4, 1e-9);
    comprobar("a_T en el borde inferior", calcular_atraccion_tierra(6531000.0), 9.344297,  1e-6);
    comprobar("a_T en el borde superior", calcular_atraccion_tierra(345663000.0), 3.335802e-3, 1e-8);
    comprobar("a_L en el borde superior", calcular_atraccion_luna(345663000.0),  3.335807e-3, 1e-8);
    comprobar("a_T con distancia 0",    calcular_atraccion_tierra(0.0),        0.0,         0.0);
    comprobar("desaceleracion neta en r0", telemetria.desaceleracion_neta,     9.253363,    1e-6);
    comprobar("energia cinetica por kg en r0", telemetria.energia_cinetica,    59405000.0,  1.0);
}

static void probar_energia_y_pozos(void)
{
    comprobar("Ec por defecto",
              calcular_energia_cinetica(26000.0, 10900.0),
              1.54453e12, 1.0);

    comprobar("Ec intermedia",
              calcular_energia_cinetica(50000.0, 11000.0),
              3.025e12, 1.0);

    comprobar("Ec borde inferior",
              calcular_energia_cinetica(1000.0, 10500.0),
              5.5125e10, 1.0);

    comprobar("Ec borde superior",
              calcular_energia_cinetica(100000.0, 11200.0),
              6.272e12, 1.0);

    comprobar("Ec por kg",
              calcular_energia_cinetica(1.0, 10900.0),
              59405000.0, 1e-6);

    comprobar("pozo Tierra por defecto",
              calcular_pozo_tierra(192000.0),
              60730044.0, 1.0);

    comprobar("pozo Tierra 160 km",
              calcular_pozo_tierra(160000.0),
              61027604.0, 1.0);

    comprobar("pozo Tierra 500 km",
              calcular_pozo_tierra(500000.0),
              58007754.0, 1.0);

    comprobar("pozo Tierra 2000 km",
              calcular_pozo_tierra(2000000.0),
              47613341.0, 1.0);

    comprobar("pozo Luna",
              calcular_pozo_luna(),
              2821872.0, 1.0);

    comprobar("razón Tierra / Luna",
              calcular_pozo_tierra(192000.0) / calcular_pozo_luna(),
              21.5212, 1e-4);

    comprobar("energía de regreso",
              calcular_pozo_luna() * 26000.0,
              7.337e10, 1e7);

    comprobar("pozo Tierra con altitud negativa",
              calcular_pozo_tierra(-1.0),
              0.0, 0.0);
}

static void probar_trabajo_minimo_y_distancia_maxima(void)
{
}

static void probar_estado_nave(void)
{
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
