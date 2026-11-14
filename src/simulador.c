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

/* a_T = G · M_T / r² */
double calcular_atraccion_tierra(double distancia)
{
    if (!distancia_valida(distancia)) {
        return 0.0;
    }
    return CONSTANTE_G * MASA_TIERRA / (distancia * distancia);
}

/* a_L = G · M_L / (D - r)² */
double calcular_atraccion_luna(double distancia)
{
    if (!distancia_valida(distancia)) {
        return 0.0;
    }
    double d = DISTANCIA_TIERRA_LUNA - distancia;
    return CONSTANTE_G * MASA_LUNA / (d * d);
}

/* Copia posición y velocidad; calcula atracciones, desaceleración neta y energía cinética. */
void calcular_telemetria(const EstadoNave *nave, Telemetria *telemetria)
{
    telemetria->distancia = nave->distancia;
    telemetria->velocidad = nave->velocidad;
    telemetria->atraccion_tierra = calcular_atraccion_tierra(nave->distancia);
    telemetria->atraccion_luna = calcular_atraccion_luna(nave->distancia);
    telemetria->desaceleracion_neta = telemetria->atraccion_tierra - telemetria->atraccion_luna;
    telemetria->energia_cinetica = nave->velocidad * nave->velocidad / 2.0;
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

double calcular_distancia_punto(int punto)
{
    (void)punto;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

void inicializar_nave(EstadoNave *nave, double velocidad_inyeccion, double reserva)
{
    (void)nave;
    (void)velocidad_inyeccion;
    (void)reserva;
    /* Pendiente: se implementa en esta etapa. */
}

int aplicar_correccion(EstadoNave *nave, double trabajo)
{
    (void)nave;
    (void)trabajo;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
}

int avanzar_nave(EstadoNave *nave)
{
    (void)nave;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
}

int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[])
{
    (void)nave;
    (void)velocidades;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
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
