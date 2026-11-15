#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "simulador.h"

int nueva_mision(ReporteFinal *reporte)
{
    (void)reporte;
    printf("La nueva misiÃ³n todavÃ­a no estÃ¡ disponible.\n");
    return 0;
}

void submenu_consultas(void)
{
    printf("Las consultas del Problema 3 todavÃ­a no estÃ¡n disponibles.\n");
}

void mostrar_datos_y_supuestos(void)
{
    printf("\n");
    printf("DATOS DEL ESCENARIO (Problema 3)\n");
    printf("  Masa de Orion por defecto ........... %.0f kg\n", MASA_DEFECTO);
    printf("  Velocidad de inyecciÃ³n por defecto .. %.0f m/s\n", VELOCIDAD_TLI_DEFECTO);
    printf("  Altitud de la Ã³rbita baja ........... %.0f m\n", ALTITUD_DEFECTO);
    printf("  Distancia media Tierra-Luna ......... %.0f m\n", DISTANCIA_TIERRA_LUNA);
    printf("  Constante de gravitaciÃ³n G .......... %.3e NÂ·mÂ²/kgÂ²\n", CONSTANTE_G);
    printf("  Masa de la Tierra ................... %.3e kg\n", MASA_TIERRA);
    printf("  Radio medio de la Tierra ............ %.0f m\n", RADIO_TIERRA);
    printf("  Masa de la Luna ..................... %.3e kg\n", MASA_LUNA);
    printf("  Radio medio de la Luna .............. %.0f m\n", RADIO_LUNA);
    printf("\n");
    printf("SUPUESTOS DEL MODELO\n");
    printf("  - Una dimensiÃ³n: la nave se mueve sobre la recta Tierra-Luna.\n");
    printf("  - Luna fija: se usa la distancia media Tierra-Luna.\n");
    printf("  - Sin propulsiÃ³n: despuÃ©s de la TLI solo actÃºan las gravedades de la Tierra\n");
    printf("    y de la Luna, salvo las correcciones que aplica el jugador.\n");
    printf("  - ConservaciÃ³n de la energÃ­a: la velocidad en cada punto se obtiene de Em = K + U.\n");
}

void mostrar_ultimo_reporte(const ReporteFinal *reporte)
{
    if (reporte == NULL) {
        printf("TodavÃ­a no hay misiones jugadas\n");
        return;
    }
    printf("El reporte detallado todavÃ­a no estÃ¡ disponible.\n");
}

/* Devuelve 1 si la distancia estÃ¡ entre la Tierra y la Luna, donde las fÃ³rmulas no dividen por cero. */
static int distancia_valida(double distancia)
{
    return distancia > 0.0 && distancia < DISTANCIA_TIERRA_LUNA;
}

/* U(x)/m = -GÂ·M_T / x - GÂ·M_L / (D - x), en J/kg */
double calcular_energia_potencial_por_kg(double distancia)
{
    if (!distancia_valida(distancia)) {
        return 0.0;
    }
    return -CONSTANTE_G * MASA_TIERRA / distancia
           - CONSTANTE_G * MASA_LUNA / (DISTANCIA_TIERRA_LUNA - distancia);
}

/* Punto donde las dos atracciones son iguales: x = D / (1 + raÃ­z(M_L / M_T)), en m */
double calcular_punto_equilibrio(void)
{
    return DISTANCIA_TIERRA_LUNA / (1.0 + sqrt(MASA_LUNA / MASA_TIERRA));
}

/* ConservaciÃ³n de la energÃ­a: vÂ² = v0Â² + 2Â·(U(r0)/m - U(x)/m).
   Devuelve 1 y guarda la velocidad si la nave llega a esa distancia; 0 si no llega. */
int calcular_velocidad_en(double distancia_inicial, double velocidad_inicial,
                          double distancia, double *velocidad)
{
    double radicando;

    if (!distancia_valida(distancia_inicial) || !distancia_valida(distancia)) {
        return 0;
    }
    radicando = velocidad_inicial * velocidad_inicial
                + 2.0 * (calcular_energia_potencial_por_kg(distancia_inicial)
                         - calcular_energia_potencial_por_kg(distancia));
    if (radicando < 0.0) {
        return 0;
    }
    *velocidad = sqrt(radicando);
    return 1;
}

/* Atracciones y telemetrÃ­a */

double calcular_atraccion_tierra(double distancia)
{
    (void)distancia;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

double calcular_atraccion_luna(double distancia)
{
    (void)distancia;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

void calcular_telemetria(const EstadoNave *nave, Telemetria *telemetria)
{
    (void)nave;
    (void)telemetria;
    /* Pendiente: se implementa en esta etapa. */
}

/* EnergÃ­a cinÃ©tica y pozos gravitatorios */

double calcular_energia_cinetica(double masa, double velocidad)
{
    (void)masa;
    (void)velocidad;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

double calcular_pozo_tierra(double altitud)
{
    (void)altitud;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

double calcular_pozo_luna(void)
{
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

/* Trabajo mÃ­nimo y distancia mÃ¡xima */

double calcular_trabajo_minimo(double velocidad, double altitud)
{
    (void)velocidad;
    (void)altitud;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

double calcular_distancia_maxima(double distancia_inicial, double velocidad_inicial)
{
    (void)distancia_inicial;
    (void)velocidad_inicial;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

/* Estado de la nave */

/* Calcula la distancia correspondiente a un punto de la trayectoria. */
double calcular_distancia_punto(int punto)
{
    double distancia_inicial = RADIO_TIERRA + ALTITUD_DEFECTO;

    if (punto < 0 || punto > CANTIDAD_TRAMOS) {
        return 0.0;
    }
    return distancia_inicial
           + punto * (calcular_punto_equilibrio() - distancia_inicial) / CANTIDAD_TRAMOS;
}

/* Inicializa todos los datos de la nave en la órbita baja. */
void inicializar_nave(EstadoNave *nave, double velocidad_inyeccion, double reserva)
{
    nave->punto = 0;
    nave->distancia = calcular_distancia_punto(0);
    nave->velocidad = velocidad_inyeccion;
    nave->reserva = reserva;
    nave->trabajo_aplicado = 0.0;
    nave->correcciones = 0;
}

/* Aplica trabajo adicional a la nave y descuenta ese trabajo de la reserva. */
int aplicar_correccion(EstadoNave *nave, double trabajo)
{
    if (trabajo <= 0.0 || trabajo > nave->reserva) {
        return 0;
    }
    nave->velocidad = sqrt(nave->velocidad * nave->velocidad + 2.0 * trabajo);
    nave->reserva -= trabajo;
    nave->trabajo_aplicado += trabajo;
    nave->correcciones++;
    return 1;
}

/* Intenta avanzar un tramo y actualiza el estado solo si la nave puede llegar. */
int avanzar_nave(EstadoNave *nave)
{
    double velocidad;
    int siguiente_punto;

    if (nave->punto == CANTIDAD_TRAMOS) {
        return 0;
    }
    siguiente_punto = nave->punto + 1;
    if (!calcular_velocidad_en(nave->distancia, nave->velocidad,
                               calcular_distancia_punto(siguiente_punto), &velocidad)) {
        return 0;
    }
    nave->punto = siguiente_punto;
    nave->distancia = calcular_distancia_punto(siguiente_punto);
    nave->velocidad = velocidad;
    return 1;
}

/* Recorre todos los puntos alcanzables sin correcciones y registra sus velocidades. */
int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[])
{
    velocidades[nave->punto] = nave->velocidad;
    while (avanzar_nave(nave)) {
        velocidades[nave->punto] = nave->velocidad;
    }
    return nave->punto;
}

/* Eventos y puntaje */

void generar_evento(Evento *evento, int probabilidad, int nave_alcanza_siguiente)
{
    (void)evento;
    (void)probabilidad;
    (void)nave_alcanza_siguiente;
    /* Pendiente: se implementa en esta etapa. */
}

void aplicar_evento(EstadoNave *nave, const Evento *evento)
{
    (void)nave;
    (void)evento;
    /* Pendiente: se implementa en esta etapa. */
}

double calcular_puntaje(ResultadoMision resultado, double velocidad_inyeccion,
                        double trabajo_aplicado, int correcciones)
{
    (void)resultado;
    (void)velocidad_inyeccion;
    (void)trabajo_aplicado;
    (void)correcciones;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}
