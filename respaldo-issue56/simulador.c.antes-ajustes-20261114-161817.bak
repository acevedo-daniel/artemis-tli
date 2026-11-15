#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "simulador.h"

int nueva_mision(ReporteFinal *reporte)
{
    (void)reporte;
    printf("La nueva misión todavía no está disponible.\n");
    return 0;
}

void submenu_consultas(void)
{
    printf("Las consultas del Problema 3 todavía no están disponibles.\n");
}

void mostrar_datos_y_supuestos(void)
{
    printf("\n");
    printf("DATOS DEL ESCENARIO (Problema 3)\n");
    printf("  Masa de Orion por defecto ........... %.0f kg\n", MASA_DEFECTO);
    printf("  Velocidad de inyección por defecto .. %.0f m/s\n", VELOCIDAD_TLI_DEFECTO);
    printf("  Altitud de la órbita baja ........... %.0f m\n", ALTITUD_DEFECTO);
    printf("  Distancia media Tierra-Luna ......... %.0f m\n", DISTANCIA_TIERRA_LUNA);
    printf("  Constante de gravitación G .......... %.3e N·m²/kg²\n", CONSTANTE_G);
    printf("  Masa de la Tierra ................... %.3e kg\n", MASA_TIERRA);
    printf("  Radio medio de la Tierra ............ %.0f m\n", RADIO_TIERRA);
    printf("  Masa de la Luna ..................... %.3e kg\n", MASA_LUNA);
    printf("  Radio medio de la Luna .............. %.0f m\n", RADIO_LUNA);
    printf("\n");
    printf("SUPUESTOS DEL MODELO\n");
    printf("  - Una dimensión: la nave se mueve sobre la recta Tierra-Luna.\n");
    printf("  - Luna fija: se usa la distancia media Tierra-Luna.\n");
    printf("  - Sin propulsión: después de la TLI solo actúan las gravedades de la Tierra\n");
    printf("    y de la Luna, salvo las correcciones que aplica el jugador.\n");
    printf("  - Conservación de la energía: la velocidad en cada punto se obtiene de Em = K + U.\n");
}

void mostrar_ultimo_reporte(const ReporteFinal *reporte)
{
    if (reporte == NULL) {
        printf("Todavía no hay misiones jugadas\n");
        return;
    }
    printf("El reporte detallado todavía no está disponible.\n");
}

/* Devuelve 1 si la distancia está entre la Tierra y la Luna, donde las fórmulas no dividen por cero. */
static int distancia_valida(double distancia)
{
    return distancia > 0.0 && distancia < DISTANCIA_TIERRA_LUNA;
}

/* U(x)/m = -G·M_T / x - G·M_L / (D - x), en J/kg */
double calcular_energia_potencial_por_kg(double distancia)
{
    if (!distancia_valida(distancia)) {
        return 0.0;
    }
    return -CONSTANTE_G * MASA_TIERRA / distancia
           - CONSTANTE_G * MASA_LUNA / (DISTANCIA_TIERRA_LUNA - distancia);
}

/* Punto donde las dos atracciones son iguales: x = D / (1 + raíz(M_L / M_T)), en m */
double calcular_punto_equilibrio(void)
{
    return DISTANCIA_TIERRA_LUNA / (1.0 + sqrt(MASA_LUNA / MASA_TIERRA));
}

/* Conservación de la energía: v² = v0² + 2·(U(r0)/m - U(x)/m).
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

/* Atracciones y telemetría */

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

/* Energía cinética y pozos gravitatorios */

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

/* Trabajo mínimo y distancia máxima */

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
    if (nave == NULL) {
        return;
    }

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
    double nueva_velocidad;

    if (nave == NULL || !isfinite(trabajo) || trabajo <= 0.0 ||
        trabajo > nave->reserva) {
        return 0;
    }

    nueva_velocidad = sqrt(nave->velocidad * nave->velocidad + 2.0 * trabajo);
    if (!isfinite(nueva_velocidad)) {
        return 0;
    }

    nave->velocidad = nueva_velocidad;
    nave->reserva -= trabajo;
    nave->trabajo_aplicado += trabajo;
    nave->correcciones++;
    return 1;
}

/* Intenta avanzar un tramo; solo actualiza el estado si puede llegar. */
int avanzar_nave(EstadoNave *nave)
{
    double velocidad_siguiente;
    int siguiente_punto;
    double siguiente_distancia;

    if (nave == NULL || nave->punto < 0 || nave->punto >= CANTIDAD_TRAMOS) {
        return 0;
    }

    siguiente_punto = nave->punto + 1;
    siguiente_distancia = calcular_distancia_punto(siguiente_punto);

    if (!calcular_velocidad_en(nave->distancia, nave->velocidad,
                               siguiente_distancia, &velocidad_siguiente)) {
        return 0;
    }

    nave->punto = siguiente_punto;
    nave->distancia = siguiente_distancia;
    nave->velocidad = velocidad_siguiente;
    return 1;
}

/* Recorre todos los puntos alcanzables sin correcciones y registra las velocidades. */
int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[])
{
    if (nave == NULL || velocidades == NULL || nave->punto < 0 ||
        nave->punto > CANTIDAD_TRAMOS) {
        return 0;
    }

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
