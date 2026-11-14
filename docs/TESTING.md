# Simulador Artemis TLI — Pruebas

> Cómo se comprueban los cálculos y el estado de la misión contra los casos de referencia.

## Cómo ejecutarlas

Desde la raíz del repositorio:

- Linux o macOS: `make pruebas`
- Windows (WinLibs): `mingw32-make pruebas`

El comando compila `pruebas/pruebas_simulador.c` junto con `src/simulador.c` usando las mismas banderas estrictas del proyecto (`-Werror`) y ejecuta el binario resultante. Muestra `N pruebas, 0 fallas` cuando todo coincide con la referencia, o una línea `FALLA` con el valor obtenido y el esperado por cada comprobación fuera de tolerancia (terminando con código de error). La CI ejecuta `make pruebas` automáticamente en cada Pull Request.

## Qué cubren

| Función de prueba | Funciones comprobadas | Casos de PROJECT.md |
| --- | --- | --- |
| `probar_energia_potencial` | `calcular_energia_potencial_por_kg`, `calcular_punto_equilibrio`, `calcular_velocidad_en` | Pregunta c, Recorrido de referencia |
| `probar_atracciones_y_telemetria` | `calcular_atraccion_tierra`, `calcular_atraccion_luna`, `calcular_telemetria` | Pregunta a |
| `probar_energia_y_pozos` | `calcular_energia_cinetica`, `calcular_pozo_tierra`, `calcular_pozo_luna` | Preguntas b y d |
| `probar_trabajo_minimo_y_distancia_maxima` | `calcular_trabajo_minimo`, `calcular_distancia_maxima` | Pregunta c, Regla 1 |
| `probar_estado_nave` | `calcular_distancia_punto`, `inicializar_nave`, `aplicar_correccion`, `avanzar_nave`, `recorrer_sin_correcciones` | Recorrido de referencia, corrección |
| `probar_eventos_y_puntaje` | `generar_evento`, `aplicar_evento`, `calcular_puntaje` | Reglas 2 y 3, Eventos, Desempeño |
| `probar_niveles` | `obtener_nivel` | Regla 2, Niveles |
| `probar_reporte` | `armar_reporte` | Desempeño y reporte final |

## Cómo agregar una prueba

1. Ubicar la función `probar_*` que corresponde al tema en [pruebas/pruebas_simulador.c](../pruebas/pruebas_simulador.c).
2. Llamar a `comprobar("nombre descriptivo", obtenido, esperado, tolerancia);`.
3. Tomar el valor esperado siempre de [docs/PROJECT.md](PROJECT.md) o de [docs/calculos/](calculos/README.md), nunca de ejecutar el propio programa.

## Pruebas manuales

Las pruebas de entradas inválidas y los recorridos interactivos del juego se siguen verificando manualmente según [docs/PROJECT.md](PROJECT.md) › Entradas inválidas y recorridos del juego.
