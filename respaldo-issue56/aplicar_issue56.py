from pathlib import Path
root=Path.cwd()
if not (root / 'src/simulador.c').is_file() or not (root / 'pruebas/pruebas_simulador.c').is_file():
    raise SystemExit('Ejecutá este script desde la raíz del repositorio artemis-tli.')
if 'feat/estado-nave' not in __import__('subprocess').check_output(['git','branch','--show-current'], text=True).strip():
    raise SystemExit('La rama actual no es feat/estado-nave; no se modificó ningún archivo.')
p=root/'src/simulador.c'; s=p.read_text()
old='''/* Estado de la nave */

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
'''
new='''/* Estado de la nave */

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
'''
assert s.count(old)==1
p.write_text(s.replace(old,new), encoding='utf-8')

p=root/'pruebas/pruebas_simulador.c'; s=p.read_text()
old='''static void probar_estado_nave(void)
{
}
'''
new='''static void probar_estado_nave(void)
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
'''
assert s.count(old)==1
p.write_text(s.replace(old,new), encoding='utf-8')

p=root/'README.md'; s=p.read_text()
needle='- 14/11/2026 · Mario Daniel Acevedo · Gemini 4 Hydrogen / Antigravity (Google) — Creación de la base del núcleo de cálculo y pruebas automáticas (contrato en `simulador.h`, base física en `simulador.c`, esqueletos y `pruebas/pruebas_simulador.c`). Consulta: "crea una nueva rama e implementa nucleo-calculo-simulacion tasl 1". Validación: compilación sin advertencias y ejecución de pruebas con `mingw32-make && mingw32-make pruebas` (9 pruebas, 0 fallas).\n'
assert s.count(needle)==1
entry='- 14/11/2026 · Fernando Germán Ojeda · ChatGPT (OpenAI) — Implementación del estado de la nave y sus dieciocho pruebas automáticas (#56). Consulta: "Implementar calcular_distancia_punto, inicializar_nave, aplicar_correccion, avanzar_nave y recorrer_sin_correcciones según el Issue #56". Validación: compilación sin advertencias y ejecución de mingw32-make pruebas (27 pruebas, 0 fallas).\n'
p.write_text(s.replace(needle,needle+entry), encoding='utf-8')
