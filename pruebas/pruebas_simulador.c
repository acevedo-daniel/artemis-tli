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
}

static void probar_eventos_y_puntaje(void)
{
    Evento evento;
    EstadoNave nave;
    int i, cantidad;

    srand(1);

    /* Probabilidad 0: nunca debe haber evento */
    cantidad = 0;
    for (i = 0; i < 1000; i++) {
        generar_evento(&evento, 0, 1);
        if (evento.tipo != EVENTO_NINGUNO) {
            cantidad++;
        }
    }
    comprobar("sin eventos con probabilidad 0", cantidad, 0, 0);

    /* Probabilidad 100: nunca debe quedar EVENTO_NINGUNO;
       además contar fugas fuera de rango */
    cantidad = 0;
    int fugas_fuera_rango = 0;
    for (i = 0; i < 1000; i++) {
        generar_evento(&evento, 100, 1);
        if (evento.tipo == EVENTO_NINGUNO) {
            cantidad++;
        }
        if (evento.tipo == EVENTO_FUGA_PROPULSION &&
            (evento.porcentaje_reserva < FUGA_MINIMA ||
             evento.porcentaje_reserva > FUGA_MAXIMA)) {
            fugas_fuera_rango++;
        }
    }
    comprobar("siempre hay evento con probabilidad 100", cantidad, 0, 0);
    comprobar("fuga entre 10 y 25 %", fugas_fuera_rango, 0, 0);

    /* Probabilidad 100 con nave que no alcanza siguiente: sin EVENTO_PERDIDA_SENAL */
    cantidad = 0;
    for (i = 0; i < 1000; i++) {
        generar_evento(&evento, 100, 0);
        if (evento.tipo == EVENTO_PERDIDA_SENAL) {
            cantidad++;
        }
    }
    comprobar("sin perdida de senal si la nave no llega", cantidad, 0, 0);

    /* aplicar_evento: fuga del 25 % sobre reserva 150000 */
    nave.reserva = 150000.0;
    evento.tipo = EVENTO_FUGA_PROPULSION;
    evento.porcentaje_reserva = 25;
    aplicar_evento(&nave, &evento);
    comprobar("reserva tras la peor fuga", nave.reserva, 112500.0, 1e-6);
    comprobar("la peor fuga deja reserva suficiente", nave.reserva > 57086.0, 1, 0);

    /* Los eventos no tocan velocidad ni distancia */
    nave.punto = 5;
    nave.distancia = 176112987.0;
    nave.velocidad = 1377.47;
    nave.reserva = 150000.0;
    nave.trabajo_aplicado = 0.0;
    nave.correcciones = 0;

    evento.tipo = EVENTO_FUGA_PROPULSION;
    evento.porcentaje_reserva = 25;
    aplicar_evento(&nave, &evento);

    evento.tipo = EVENTO_PERDIDA_SENAL;
    evento.porcentaje_reserva = 0;
    aplicar_evento(&nave, &evento);

    evento.tipo = EVENTO_FALLA_SENSOR;
    evento.porcentaje_reserva = 0;
    aplicar_evento(&nave, &evento);

    comprobar("los eventos no cambian la velocidad", nave.velocidad, 1377.47, 0.0);
    comprobar("los eventos no cambian la distancia", nave.distancia, 176112987.0, 0.0);

    /* calcular_puntaje */
    comprobar("puntaje con 10 906 m/s sin correcciones",
              calcular_puntaje(RESULTADO_EXITO, 10906.0, 0.0, 0), 99.583, 0.001);
    comprobar("puntaje con una correccion",
              calcular_puntaje(RESULTADO_EXITO, 10900.0, 60000.0, 1), 94.854, 0.001);
    comprobar("puntaje de una mision fracasada",
              calcular_puntaje(RESULTADO_FRACASO, 10900.0, 0.0, 0), 0.0, 0.0);
    comprobar("puntaje de una mision abortada",
              calcular_puntaje(RESULTADO_ABORTADA, 10900.0, 0.0, 0), 0.0, 0.0);
    comprobar("puntaje nunca negativo",
              calcular_puntaje(RESULTADO_EXITO, 11200.0, 150000.0, 3), 0.0, 0.0);
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
