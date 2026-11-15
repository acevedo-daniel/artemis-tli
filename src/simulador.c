#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "simulador.h"
#include "validaciones.h"

/* Pide un trabajo de corrección mayor que 0 y no mayor que la reserva disponible. */
static double pedir_correccion(double reserva)
{
    char aviso[128];
    double trabajo;

    snprintf(aviso, sizeof(aviso),
             "El trabajo no puede superar la reserva disponible (%.2f J/kg). Ingrese nuevamente:",
             reserva);

    do {
        trabajo = leer_real("Ingrese el trabajo de corrección (J/kg): ",
                            0.0, reserva, aviso);
        if (trabajo <= 0.0) {
            printf("El trabajo de corrección debe ser mayor que 0.\n");
        }
    } while (trabajo <= 0.0);

    return trabajo;
}

/* Un turno por tramo. Después de cada corrección se vuelve a mostrar el turno,
   así el jugador ve la nueva predicción antes de decidir si continúa. */
static void jugar_tramos(EstadoNave *nave, ParametrosMision *mision, int probabilidad_eventos)
{
    Evento evento;
    double velocidad_siguiente;
    int alcanza;
    char decision;

    while (nave->punto < CANTIDAD_TRAMOS && mision->resultado == RESULTADO_EN_CURSO) {
        alcanza = calcular_velocidad_en(nave->distancia, nave->velocidad,
                                        calcular_distancia_punto(nave->punto + 1),
                                        &velocidad_siguiente);

        generar_evento(&evento, probabilidad_eventos, alcanza);
        aplicar_evento(nave, &evento);
        mostrar_turno(nave, &evento);

        do {
            decision = leer_caracter("Decisión ([c] continuar, [r] corregir, [a] abortar): ",
                                     "cra", "Opción inválida. Ingrese c, r o a:");

            if (decision == 'r') {
                if (evento.tipo == EVENTO_PERDIDA_SENAL) {
                    printf("No se puede corregir en este turno por pérdida de señal.\n");
                } else if (nave->reserva <= 0.0) {
                    printf("No queda reserva de corrección disponible.\n");
                } else if (aplicar_correccion(nave, pedir_correccion(nave->reserva))) {
                    mostrar_turno(nave, &evento);
                }
            }
        } while (decision == 'r');

        if (decision == 'a') {
            mision->resultado = RESULTADO_ABORTADA;
            printf("Misión abortada por el comandante.\n");
        } else if (!avanzar_nave(nave)) {
            mision->resultado = RESULTADO_FRACASO;
            printf("La nave no alcanza el próximo punto "
                   "(se detiene a %.0f km) y vuelve a caer hacia la Tierra.\n",
                   calcular_distancia_maxima(nave->distancia, nave->velocidad) / 1000.0);
        }
    }
}

/* Juega una misión de punta a punta. El resultado queda en *reporte para que
   main() lo pueda volver a mostrar con la opción 4. */
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

    printf("\nETAPA 1 - INYECCIÓN TRANSLUNAR\n");
    mision.velocidad_inyeccion = leer_real(
        "Velocidad de inyección (10500 a 11200 m/s; la de la consigna es 10900): ",
        VELOCIDAD_TLI_MINIMA, VELOCIDAD_TLI_MAXIMA,
        "La velocidad debe estar entre 10 500 y 11 200 m/s. Ingrese nuevamente:");

    energia_total = calcular_energia_cinetica(mision.masa, mision.velocidad_inyeccion);
    energia_por_kg = calcular_energia_cinetica(1.0, mision.velocidad_inyeccion);
    printf("  Energía cinética total ....... %.5e J\n", energia_total);
    printf("  Energía cinética por kg ...... %.0f J/kg\n", energia_por_kg);

    inicializar_nave(&nave, mision.velocidad_inyeccion, nivel.reserva);

    jugar_tramos(&nave, &mision, nivel.probabilidad_eventos);

    if (mision.resultado == RESULTADO_EN_CURSO && nave.punto == CANTIDAD_TRAMOS) {
        mision.resultado = RESULTADO_EXITO;
        printf("\n¡La nave alcanzó el punto de equilibrio gravitatorio Tierra-Luna!\n");
    }

    armar_reporte(reporte, &mision, &nave);
    mostrar_ultimo_reporte(reporte);
    return 1;
}

/* Devuelve 1 si el usuario elige los valores por defecto del escenario. */
static int usar_valores_por_defecto(void)
{
    return leer_caracter("¿Usar los valores por defecto? (s/n): ", "sn",
                         "Entrada inválida. Ingrese s o n:") == 's';
}

/* Consulta a: vuelo sin propulsión y 1.ª Ley de Newton */
static void consulta_a(void)
{
    double altitud, r, a_tierra, a_luna;

    printf("\n--- CONSULTA A: 1.ª LEY DE NEWTON ---\n");

    if (usar_valores_por_defecto()) {
        altitud = ALTITUD_DEFECTO;
        r = RADIO_TIERRA + altitud;
    } else {
        altitud = leer_real("Ingrese la altitud h (m): ", ALTITUD_MINIMA, ALTITUD_MAXIMA,
                            "La altitud debe estar entre 160 000 y 2 000 000 m. Ingrese nuevamente:");
        r = leer_real("Ingrese la distancia r desde el centro de la Tierra (m): ",
                      RADIO_TIERRA + altitud, calcular_punto_equilibrio(),
                      "La distancia debe estar entre la órbita baja y el punto de equilibrio. Ingrese nuevamente:");
    }

    a_tierra = calcular_atraccion_tierra(r);
    a_luna = calcular_atraccion_luna(r);

    printf("\nSin fuerza neta, la nave seguiría a velocidad constante en línea recta (inercia).\n");
    printf("La gravedad terrestre la frena con una aceleración que disminuye con 1/r² a medida que se aleja.\n");
    printf("Distancia r: %.2f m\n", r);
    printf("Atracción de la Tierra (a_T): %.5f m/s²\n", a_tierra);
    printf("Atracción de la Luna (a_L): %e m/s²\n", a_luna);
}

/* Consulta b: energía cinética inicial */
static void consulta_b(void)
{
    double masa, velocidad;

    printf("\n--- CONSULTA B: ENERGÍA CINÉTICA INICIAL ---\n");

    if (usar_valores_por_defecto()) {
        masa = MASA_DEFECTO;
        velocidad = VELOCIDAD_TLI_DEFECTO;
    } else {
        masa = leer_real("Ingrese la masa m (kg): ", MASA_MINIMA, MASA_MAXIMA,
                         "La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:");
        velocidad = leer_real("Ingrese la velocidad de inyección v (m/s): ",
                              VELOCIDAD_TLI_MINIMA, VELOCIDAD_TLI_MAXIMA,
                              "La velocidad debe estar entre 10 500 y 11 200 m/s. Ingrese nuevamente:");
    }

    printf("Masa: %.2f kg\n", masa);
    printf("Velocidad: %.2f m/s\n", velocidad);
    printf("Energía cinética total (Ec): %e J\n", calcular_energia_cinetica(masa, velocidad));
    printf("Energía cinética por kilogramo (Ec/m): %.2f J/kg\n",
           calcular_energia_cinetica(1.0, velocidad));
}

/* Consulta c: trabajo mínimo hasta el punto de equilibrio */
static void consulta_c(void)
{
    double altitud, velocidad, trabajo;
    double x_eq = calcular_punto_equilibrio();

    printf("\n--- CONSULTA C: TRABAJO MÍNIMO HASTA EL PUNTO DE EQUILIBRIO ---\n");

    if (usar_valores_por_defecto()) {
        altitud = ALTITUD_DEFECTO;
        velocidad = VELOCIDAD_CONSULTA_MINIMA;
    } else {
        altitud = leer_real("Ingrese la altitud h (m): ", ALTITUD_MINIMA, ALTITUD_MAXIMA,
                            "La altitud debe estar entre 160 000 y 2 000 000 m. Ingrese nuevamente:");
        velocidad = leer_real("Ingrese la velocidad v (m/s): ",
                              VELOCIDAD_CONSULTA_MINIMA, VELOCIDAD_TLI_MAXIMA,
                              "La velocidad debe estar entre 0 y 11 200 m/s. Ingrese nuevamente:");
    }

    trabajo = calcular_trabajo_minimo(velocidad, altitud);

    printf("Punto de equilibrio (x_eq): %.2f m (%.2f km)\n", x_eq, x_eq / 1000.0);
    printf("U(r0)/m: %.2f J/kg\n", calcular_energia_potencial_por_kg(RADIO_TIERRA + altitud));
    printf("U(x_eq)/m: %.2f J/kg\n", calcular_energia_potencial_por_kg(x_eq));

    if (trabajo <= 0.0) {
        printf("Trabajo adicional mínimo (W/m): 0.00 J/kg\n");
        printf("Con esa velocidad, Orion alcanza el punto de equilibrio sin trabajo adicional.\n");
    } else {
        printf("Trabajo adicional mínimo (W/m): %.2f J/kg\n", trabajo);
        if (velocidad > 0.0) {
            printf("Con esa velocidad, Orion no alcanza el punto de equilibrio sin trabajo adicional.\n");
        }
    }
}

/* Consulta d: comparación energética entre la ida y el regreso */
static void consulta_d(void)
{
    double masa, altitud, pozo_tierra, pozo_luna;

    printf("\n--- CONSULTA D: COMPARACIÓN ENERGÉTICA ENTRE LA IDA Y EL REGRESO ---\n");

    if (usar_valores_por_defecto()) {
        masa = MASA_DEFECTO;
        altitud = ALTITUD_DEFECTO;
    } else {
        masa = leer_real("Ingrese la masa m (kg): ", MASA_MINIMA, MASA_MAXIMA,
                         "La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:");
        altitud = leer_real("Ingrese la altitud h (m): ", ALTITUD_MINIMA, ALTITUD_MAXIMA,
                            "La altitud debe estar entre 160 000 y 2 000 000 m. Ingrese nuevamente:");
    }

    pozo_tierra = calcular_pozo_tierra(altitud);
    pozo_luna = calcular_pozo_luna();

    printf("Masa: %.2f kg\n", masa);
    printf("Altitud: %.2f m\n", altitud);
    printf("Pozo gravitatorio terrestre: %.2f MJ/kg\n", pozo_tierra / 1e6);
    printf("Pozo gravitatorio lunar: %.2f MJ/kg\n", pozo_luna / 1e6);
    printf("Razón Tierra / Luna: %.2f\n", pozo_tierra / pozo_luna);
    printf("Energía total ida: %e J\n", pozo_tierra * masa);
    printf("Energía total regreso: %e J\n", pozo_luna * masa);
    printf("Salir del pozo terrestre exige mucha más energía que salir del lunar:\n");
    printf("por eso la ida necesita mucho más combustible que el regreso.\n");
}

/* Cada pregunta del Problema 3 se puede consultar por separado, con datos propios o por defecto. */
void submenu_consultas(void)
{
    char opcion;

    do {
        printf("\nCONSULTAS DEL PROBLEMA 3\n");
        printf("  a. Vuelo sin propulsión y 1.ª Ley de Newton\n");
        printf("  b. Energía cinética inicial\n");
        printf("  c. Trabajo mínimo hasta el punto de equilibrio\n");
        printf("  d. Comparación energética entre la ida y el regreso\n");
        printf("  v. Volver al menú principal\n");

        opcion = leer_caracter("Elija una opción: ", "abcdv",
                               "Opción inválida. Ingrese a, b, c, d o v:");

        switch (opcion) {
            case 'a':
                consulta_a();
                break;
            case 'b':
                consulta_b();
                break;
            case 'c':
                consulta_c();
                break;
            case 'd':
                consulta_d();
                break;
            default:
                break;
        }
    } while (opcion != 'v');
}

/* Constantes y simplificaciones que usa el modelo. */
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

/* Se llama al terminar cada misión y desde la opción 4; con NULL avisa que todavía no se jugó. */
void mostrar_ultimo_reporte(const ReporteFinal *reporte)
{
    const char *resultado;

    if (reporte == NULL) {
        printf("Todavía no hay misiones jugadas\n");
        return;
    }

    if (reporte->resultado == RESULTADO_EXITO) {
        resultado = "Éxito";
    } else if (reporte->resultado == RESULTADO_FRACASO) {
        resultado = "Fracaso";
    } else {
        resultado = "Abortada";
    }

    printf("\nREPORTE FINAL DE LA MISIÓN\n");
    printf("  Resultado: %s\n", resultado);
    printf("  Masa: %.2f kg | Velocidad de inyección: %.2f m/s\n",
           reporte->masa, reporte->velocidad_inyeccion);
    printf("  Energía cinética inicial: %e J\n", reporte->energia_cinetica_inicial);
    printf("  Distancia máxima: %.2f km\n", reporte->distancia_maxima / 1000.0);
    printf("  Velocidad final: %.2f m/s\n", reporte->velocidad_final);
    printf("  Trabajo aplicado en correcciones: %.2f J/kg (trabajo mínimo: %.2f J/kg)\n",
           reporte->trabajo_aplicado, fmax(0.0, reporte->trabajo_minimo));
    printf("  Ida (pozo terrestre): %.2f MJ/kg (total: %e J)\n",
           reporte->pozo_tierra / 1e6, reporte->pozo_tierra * reporte->masa);
    printf("  Regreso (pozo lunar): %.2f MJ/kg (total: %e J)\n",
           reporte->pozo_luna / 1e6, reporte->pozo_luna * reporte->masa);
    printf("  Razón Tierra / Luna: %.2f\n", reporte->pozo_tierra / reporte->pozo_luna);
    printf("  Correcciones usadas: %d\n", reporte->correcciones_usadas);
    printf("  Puntaje final: %.1f / 100\n", reporte->puntaje);
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

/* Copia el nivel pedido en *nivel; devuelve 0 si ese índice no existe. */
int obtener_nivel(int indice, Nivel *nivel)
{
    if (nivel == NULL || indice < 0 || indice >= CANTIDAD_NIVELES_PREDEFINIDOS) {
        return 0;
    }
    *nivel = niveles[indice];
    return 1;
}

/* Muestra los niveles; en el personalizado, el jugador elige masa, reserva y eventos. */
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

/* Dibuja la posición de la nave entre la Tierra y el punto de equilibrio. */
static void mostrar_barra_progreso(int punto)
{
    int i;
    printf("Tierra ●");
    for (i = 0; i < punto; i++) {
        printf("─");
    }
    printf("▶");
    for (i = punto; i < CANTIDAD_TRAMOS; i++) {
        printf("─");
    }
    printf(" ○ Luna\n");
}

/* Muestra un turno: barra, telemetría, predicción del tramo siguiente, reserva y evento. */
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
    printf("  Atracción Tierra:   %.5g m/s²\n", tel.atraccion_tierra);
    printf("  Atracción Luna:     %.5g m/s²\n", tel.atraccion_luna);
    printf("  Desaceleración:     %.5g m/s²\n", tel.desaceleracion_neta);
    printf("  Energía cinética:   %.0f J/kg\n", tel.energia_cinetica);
    printf("\n  1.ª Ley de Newton: la nave conserva su movimiento por inercia;\n"
           "  solo la frena la gravedad terrestre, cada vez menos a medida que se aleja.\n");

    printf("\n");
    if (evento->tipo == EVENTO_FALLA_SENSOR) {
        printf("  Falla de sensor: predicción no disponible en este turno\n");
    } else {
        alcanza = calcular_velocidad_en(nave->distancia, nave->velocidad,
                                        calcular_distancia_punto(nave->punto + 1),
                                        &velocidad_siguiente);
        if (alcanza) {
            printf("  Velocidad al final del tramo: %.1f m/s\n", velocidad_siguiente);
        } else {
            printf("  La nave no alcanza el próximo punto\n");
        }
    }

    printf("  Reserva disponible: %.0f J/kg\n", nave->reserva);

    if (evento->tipo == EVENTO_FUGA_PROPULSION) {
        printf("\n  Fuga de propulsión: se perdió el %.0f %% de la reserva.\n",
               evento->porcentaje_reserva);
    } else if (evento->tipo == EVENTO_PERDIDA_SENAL) {
        printf("\n  En este turno no se puede corregir\n");
    }
}

/* Reporte */

/* Arma el reporte final a partir de la misión y del estado en que terminó la nave. */
void armar_reporte(ReporteFinal *reporte, const ParametrosMision *mision, const EstadoNave *nave)
{
    if (reporte == NULL || mision == NULL || nave == NULL) {
        return;
    }

    reporte->masa = mision->masa;
    reporte->velocidad_inyeccion = mision->velocidad_inyeccion;
    reporte->resultado = mision->resultado;
    reporte->energia_cinetica_inicial = calcular_energia_cinetica(mision->masa,
                                                                  mision->velocidad_inyeccion);

    /* Éxito: llegó al equilibrio; fracaso: se detuvo antes; abortada: quedó donde estaba. */
    if (mision->resultado == RESULTADO_EXITO) {
        reporte->distancia_maxima = calcular_punto_equilibrio();
    } else if (mision->resultado == RESULTADO_FRACASO) {
        reporte->distancia_maxima = calcular_distancia_maxima(nave->distancia, nave->velocidad);
    } else {
        reporte->distancia_maxima = nave->distancia;
    }

    reporte->velocidad_final = nave->velocidad;
    reporte->trabajo_aplicado = nave->trabajo_aplicado;
    reporte->correcciones_usadas = nave->correcciones;
    reporte->trabajo_minimo = calcular_trabajo_minimo(mision->velocidad_inyeccion, ALTITUD_DEFECTO);
    reporte->pozo_tierra = calcular_pozo_tierra(ALTITUD_DEFECTO);
    reporte->pozo_luna = calcular_pozo_luna();
    reporte->puntaje = calcular_puntaje(mision->resultado, mision->velocidad_inyeccion,
                                        nave->trabajo_aplicado, nave->correcciones);
}
