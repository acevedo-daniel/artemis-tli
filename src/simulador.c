#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "simulador.h"
#include "validaciones.h"

static double pedir_correccion(double reserva)
{
    char aviso[128];
    double trabajo;

    snprintf(aviso, sizeof(aviso),
             "El trabajo no puede superar la reserva disponible (%.2f J/kg). Ingrese nuevamente:",
             reserva);

    do {
        trabajo = leer_real("Ingrese el trabajo de correcci\xc3\xb3n (J/kg): ",
                            0.0, reserva, aviso);
        if (trabajo <= 0.0) {
            printf("El trabajo de correcci\xc3\xb3n debe ser mayor que 0.\n");
        }
    } while (trabajo <= 0.0);

    return trabajo;
}

static void jugar_tramos(EstadoNave *nave, ParametrosMision *mision, int probabilidad_eventos)
{
    Evento evento;
    double velocidad_siguiente;
    int alcanza;
    char decision;

    while (nave->punto < CANTIDAD_TRAMOS && mision->resultado == RESULTADO_EN_CURSO) {
        alcanza = calcular_velocidad_en(
            nave->distancia,
            nave->velocidad,
            calcular_distancia_punto(nave->punto + 1),
            &velocidad_siguiente);

        generar_evento(&evento, probabilidad_eventos, alcanza);
        aplicar_evento(nave, &evento);
        mostrar_turno(nave, &evento);

        do {
            decision = leer_caracter(
                "Decisi\xc3\xb3n ([c] continuar, [r] corregir, [a] abortar): ",
                "cra",
                "Opci\xc3\xb3n inv\xc3\xa1lida. Ingrese c, r o a:");

            if (decision == 'r') {
                if (evento.tipo == EVENTO_PERDIDA_SENAL) {
                    printf("No se puede corregir en este turno por p\xc3\xa9rdida de se\xc3\xb1" "al.\n");
                } else if (nave->reserva <= 0.0) {
                    printf("No queda reserva de correcci\xc3\xb3n disponible.\n");
                } else if (aplicar_correccion(nave, pedir_correccion(nave->reserva))) {
                    mostrar_turno(nave, &evento);
                }
            }
        } while (decision == 'r');

        if (decision == 'a') {
            mision->resultado = RESULTADO_ABORTADA;
            printf("Misi\xc3\xb3n abortada por el comandante.\n");
        } else if (!avanzar_nave(nave)) {
            mision->resultado = RESULTADO_FRACASO;
            printf("La nave no alcanza el pr\xc3\xb3ximo punto "
                   "(se detiene a %.0f km) y vuelve a caer hacia la Tierra.\n",
                   calcular_distancia_maxima(nave->distancia, nave->velocidad) / 1000.0);
        }
    }
}

int nueva_mision(ReporteFinal *reporte)
{
    Nivel nivel;
    ParametrosMision mision;
    EstadoNave nave;
    double energia_total;
    double energia_por_kg;

    if (reporte == NULL) {
        return 0;
    }

    srand((unsigned)time(NULL));
    elegir_nivel(&nivel);

    mision.masa = nivel.masa;
    mision.indice_nivel = 0;
    mision.resultado = RESULTADO_EN_CURSO;

    printf("\nETAPA 1 - INYECCI\xc3\x93N TRANSLUNAR\n");
    mision.velocidad_inyeccion = leer_real(
        "Velocidad de inyecci\xc3\xb3n (10500 a 11200 m/s, defecto 10900): ",
        VELOCIDAD_TLI_MINIMA,
        VELOCIDAD_TLI_MAXIMA,
        "La velocidad debe estar entre 10 500 y 11 200 m/s. Ingrese nuevamente:");

    energia_total = calcular_energia_cinetica(mision.masa, mision.velocidad_inyeccion);
    energia_por_kg = calcular_energia_cinetica(1.0, mision.velocidad_inyeccion);
    printf("  Energ\xc3\xad" "a cin\xc3\xa9tica total ....... %.5e J\n", energia_total);
    printf("  Energ\xc3\xad" "a cin\xc3\xa9tica por kg ...... %.0f J/kg\n", energia_por_kg);

    inicializar_nave(&nave, mision.velocidad_inyeccion, nivel.reserva);

    jugar_tramos(&nave, &mision, nivel.probabilidad_eventos);

    if (mision.resultado == RESULTADO_EN_CURSO && nave.punto == CANTIDAD_TRAMOS) {
        mision.resultado = RESULTADO_EXITO;
        printf("\n\xc2\xa1La nave alcanz\xc3\xb3 el punto de equilibrio gravitatorio Tierra-Luna!\n");
    }

    armar_reporte(reporte, &mision, &nave);
    mostrar_ultimo_reporte(reporte);
    return 1;
}

void submenu_consultas(void)
{
    /* Pendiente: se implementa en esta etapa. */
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
    /* Pendiente: se implementa en esta etapa. */
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

/* Energía cinética: Ec = 0.5 * masa * velocidad², en J. */
double calcular_energia_cinetica(double masa, double velocidad)
{
    return 0.5 * masa * velocidad * velocidad;
}

/* Pozo terrestre: G * MT / (RT + altitud), en J/kg. */
double calcular_pozo_tierra(double altitud)
{
    if (altitud < 0.0) {
        return 0.0;
    }

    return CONSTANTE_G * MASA_TIERRA / (RADIO_TIERRA + altitud);
}

/* Pozo lunar: G * ML / RL, en J/kg. */
double calcular_pozo_luna(void)
{
    return CONSTANTE_G * MASA_LUNA / RADIO_LUNA;
}

/* Trabajo mínimo y distancia máxima */

/* Calcula el trabajo adicional por kilogramo necesario para alcanzar
   el punto de equilibrio, descontando la energía cinética inicial. */
double calcular_trabajo_minimo(double velocidad, double altitud)
{
    double r0 = RADIO_TIERRA + altitud;

    return calcular_energia_potencial_por_kg(calcular_punto_equilibrio())
           - (velocidad * velocidad / 2.0
              + calcular_energia_potencial_por_kg(r0));
}

/* Busca la mayor distancia que puede alcanzar la nave.
   Se utiliza búsqueda binaria porque no resulta sencillo despejar
   directamente la distancia donde la velocidad se vuelve cero. */
double calcular_distancia_maxima(double distancia_inicial, double velocidad_inicial)
{
    double inferior;
    double superior;
    double medio;
    double velocidad;
    int i;

    if (!distancia_valida(distancia_inicial)) {
        return 0.0;
    }

    superior = calcular_punto_equilibrio();

    if (calcular_velocidad_en(distancia_inicial, velocidad_inicial,
                              superior, &velocidad)) {
        return superior;
    }

    inferior = distancia_inicial;

    for (i = 0; i < 100; i++) {
        medio = (inferior + superior) / 2.0;

        if (calcular_velocidad_en(distancia_inicial, velocidad_inicial,
                                  medio, &velocidad)) {
            inferior = medio;
        } else {
            superior = medio;
        }
    }

    return inferior;
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

/* Sortea un incidente con la probabilidad dada (0–100). La pérdida de señal
   se suprime si la nave no alcanza el siguiente tramo, para no bloquear correcciones. */
void generar_evento(Evento *evento, int probabilidad, int nave_alcanza_siguiente)
{
    if (rand() % 100 >= probabilidad) {
        evento->tipo = EVENTO_NINGUNO;
        evento->porcentaje_reserva = 0;
        return;
    }
    evento->tipo = 1 + rand() % 3;
    if (evento->tipo == EVENTO_FUGA_PROPULSION) {
        evento->porcentaje_reserva = FUGA_MINIMA + rand() % (FUGA_MAXIMA - FUGA_MINIMA + 1);
    } else {
        evento->porcentaje_reserva = 0;
    }
    if (evento->tipo == EVENTO_PERDIDA_SENAL && !nave_alcanza_siguiente) {
        evento->tipo = EVENTO_NINGUNO;
        evento->porcentaje_reserva = 0;
    }
}

/* Solo la fuga de propulsión reduce la reserva; ningún evento toca velocidad, distancia ni punto. */
void aplicar_evento(EstadoNave *nave, const Evento *evento)
{
    if (evento->tipo == EVENTO_FUGA_PROPULSION) {
        nave->reserva -= nave->reserva * evento->porcentaje_reserva / 100.0;
    }
}

/* Puntaje de 0 a 100; 0 si la misión no fue exitosa.
   Se descuenta por energía en exceso (J/kg) y por correcciones usadas. */
double calcular_puntaje(ResultadoMision resultado, double velocidad_inyeccion,
                        double trabajo_aplicado, int correcciones)
{
    double r0, exceso, puntaje, u_eq, u_r0;

    if (resultado != RESULTADO_EXITO) {
        return 0.0;
    }
    r0 = RADIO_TIERRA + ALTITUD_DEFECTO;
    u_r0 = calcular_energia_potencial_por_kg(r0);
    u_eq = calcular_energia_potencial_por_kg(calcular_punto_equilibrio());
    exceso = velocidad_inyeccion * velocidad_inyeccion / 2.0 + trabajo_aplicado
             - (u_eq - u_r0);
    if (exceso < 0.0) {
        exceso = 0.0;
    }
    puntaje = PUNTAJE_MAXIMO
              - DESCUENTO_POR_KJ_EXCESO * exceso / 1000.0
              - DESCUENTO_POR_CORRECCION * correcciones;
    return puntaje > 0.0 ? puntaje : 0.0;
}

/* Niveles */

static const Nivel niveles[CANTIDAD_NIVELES_PREDEFINIDOS] = {
    {"Prueba sin tripulación", 26000.0, 300000.0,                 10},
    {"Misión tripulada",       26000.0, RESERVA_CORRECCION_DEFECTO, 25},
    {"Misión con carga",       60000.0, 80000.0,                  50}
};

/* Copia el nivel en *nivel; devuelve 1 si el índice es válido y nivel != NULL. */
int obtener_nivel(int indice, Nivel *nivel)
{
    if (nivel == NULL || indice < 0 || indice >= CANTIDAD_NIVELES_PREDEFINIDOS) {
        return 0;
    }
    *nivel = niveles[indice];
    return 1;
}

/* Muestra los niveles disponibles y llena *nivel con la elección del jugador. */
void elegir_nivel(Nivel *nivel)
{
    int opcion;
    int i;
    char eventos_char;

    printf("\nNIVELES DE MISIÓN\n");
    for (i = 0; i < CANTIDAD_NIVELES_PREDEFINIDOS; i++) {
        printf("  %d. %-24s  masa %.0f kg  reserva %.0f J/kg  eventos %d%%\n",
               i + 1, niveles[i].nombre, niveles[i].masa,
               niveles[i].reserva, niveles[i].probabilidad_eventos);
    }
    printf("  4. Personalizada\n");

    opcion = leer_entero("\nElija una opción (1-4): ", 1, 4,
                         "Opción inválida. Ingrese un número entre 1 y 4:");

    if (opcion <= CANTIDAD_NIVELES_PREDEFINIDOS) {
        obtener_nivel(opcion - 1, nivel);
        return;
    }

    nivel->nombre = "Personalizada";
    nivel->masa = leer_real("Masa (kg): ", MASA_MINIMA, MASA_MAXIMA,
                            "La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:");
    nivel->reserva = leer_real("Reserva de corrección (J/kg): ", RESERVA_MINIMA, RESERVA_MAXIMA,
                               "La reserva debe estar entre 0 y 500 000 J/kg. Ingrese nuevamente:");
    eventos_char = leer_caracter("¿Activar eventos? (s/n): ", "sn",
                                 "Ingrese s o n:");
    nivel->probabilidad_eventos = (eventos_char == 's') ? PROBABILIDAD_EVENTOS_PERSONALIZADA : 0;
}

/* Turno en pantalla */

static void mostrar_barra_progreso(int punto)
{
    int i;
    printf("Tierra \xe2\x97\x8f");
    for (i = 0; i < punto; i++) {
        printf("\xe2\x94\x80");
    }
    printf("\xe2\x96\xb6");
    for (i = punto; i < CANTIDAD_TRAMOS; i++) {
        printf("\xe2\x94\x80");
    }
    printf(" \xe2\x97\x8b Luna\n");
}

void mostrar_turno(const EstadoNave *nave, const Evento *evento)
{
    Telemetria tel;
    double velocidad_siguiente;
    int alcanza;

    printf("\nTRAMO %d de %d\n", nave->punto + 1, CANTIDAD_TRAMOS);
    mostrar_barra_progreso(nave->punto);

    calcular_telemetria(nave, &tel);
    printf("\n");
    printf("  Distancia:          %.0f km\n", tel.distancia / 1000.0);
    printf("  Velocidad:          %.1f m/s\n", tel.velocidad);
    printf("  Atracci\xc3\xb3n Tierra:   %.5g m/s\xc2\xb2\n", tel.atraccion_tierra);
    printf("  Atracci\xc3\xb3n Luna:     %.5g m/s\xc2\xb2\n", tel.atraccion_luna);
    printf("  Desaceleraci\xc3\xb3n:     %.5g m/s\xc2\xb2\n", tel.desaceleracion_neta);
    printf("  Energ\xc3\xad" "a cin\xc3\xa9tica:   %.0f J/kg\n", tel.energia_cinetica);
    printf("\n  1.\xc2\xaa Ley de Newton: la nave conserva su movimiento por inercia;\n"
           "  solo la frena la gravedad terrestre, cada vez menos a medida que se aleja.\n");

    printf("\n");
    if (evento->tipo == EVENTO_FALLA_SENSOR) {
        printf("  Falla de sensor: predicci\xc3\xb3n no disponible en este turno\n");
    } else {
        alcanza = calcular_velocidad_en(nave->distancia, nave->velocidad,
                                        calcular_distancia_punto(nave->punto + 1),
                                        &velocidad_siguiente);
        if (alcanza) {
            printf("  Velocidad al final del tramo: %.1f m/s\n", velocidad_siguiente);
        } else {
            printf("  La nave no alcanza el pr\xc3\xb3ximo punto\n");
        }
    }

    printf("  Reserva disponible: %.0f J/kg\n", nave->reserva);

    if (evento->tipo == EVENTO_FUGA_PROPULSION) {
        printf("\n  Fuga de propulsi\xc3\xb3n: se perdi\xc3\xb3 el %.0f %% de la reserva.\n",
               evento->porcentaje_reserva);
    } else if (evento->tipo == EVENTO_PERDIDA_SENAL) {
        printf("\n  En este turno no se puede corregir\n");
    }
}

/* Reporte */

void armar_reporte(ReporteFinal *reporte, const ParametrosMision *mision, const EstadoNave *nave)
{
    (void)reporte;
    (void)mision;
    (void)nave;
    /* Pendiente: se implementa en esta etapa. */
}
