# Simulador Artemis TLI - Pruebas

> Cómo se comprueban los cálculos y el estado de la misión contra los casos de referencia.

## Cómo ejecutarlas

Desde la raíz del repositorio:

- Linux o macOS: `make pruebas`
- Windows (WinLibs): `mingw32-make pruebas`

El comando compila y ejecuta `pruebas/pruebas_simulador.c` y `pruebas/pruebas_consultas.c` junto con `src/simulador.c` y `src/validaciones.c`, usando las mismas banderas estrictas del proyecto (`-Werror`). Muestra el total de pruebas y fallas de cada grupo, o una línea `FALLA` con el valor obtenido y el esperado por cada comprobación fuera de tolerancia (terminando con código de error). La CI ejecuta `make pruebas` automáticamente en cada Pull Request.

Las pruebas de consultas ingresan datos al submenú real y comprueban su salida: trabajo mínimo desde el reposo, con 10 900 m/s y con 10 906 m/s; potenciales gravitatorios; pozos en MJ/kg y energías totales en J. Los dos archivos temporales de entrada y salida se eliminan al terminar.

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

### Registro - 2026-11-14 · Windows 11 · MinGW-w64 GCC 14

Nivel personalizado, eventos desactivados, salvo que se indique otra cosa.

#### Entradas inválidas

| Caso | Entrada | Resultado obtenido | ¿Coincide? |
| --- | --- | --- | --- |
| Letra donde se espera número | `a` en menú principal | `Entrada inválida. Ingrese un número entre 0 y 4:` y vuelve a pedir | ✓ |
| Línea vacía (solo Enter) | *(vacío)* en menú principal | `Entrada inválida. Ingrese un número entre 0 y 4:` y vuelve a pedir | ✓ |
| Número fuera del rango válido | `9` en menú principal | `Opción inexistente. Ingrese un número entre 0 y 4:` y vuelve a pedir | ✓ |
| Decimal donde se espera entero | `1.5` en menú principal | `Entrada inválida. Ingrese un número entre 0 y 4:` y vuelve a pedir | ✓ |
| Masa negativa o cero | `0` en ingreso de masa (nivel personalizado) | `La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:` y vuelve a pedir | ✓ |
| Masa fuera del rango superior | `200000` en ingreso de masa | `La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:` y vuelve a pedir | ✓ |
| Velocidad fuera del rango | `9000` en velocidad de inyección | `La velocidad debe estar entre 10 500 y 11 200 m/s. Ingrese nuevamente:` y vuelve a pedir | ✓ |
| Trabajo de corrección mayor que la reserva | `200000` con reserva `100000 J/kg` | `El trabajo no puede superar la reserva disponible (100000.00 J/kg). Ingrese nuevamente:` y vuelve a pedir | ✓ |
| Letra en ingreso de trabajo | `abc` en corrección | `Entrada inválida. Ingrese un número positivo:` y vuelve a pedir | ✓ |
| Fin de entrada (Ctrl+Z + Enter en Windows) | *(EOF en menú principal)* | `No hay más datos de entrada. El programa se cierra.` y termina limpiamente | ✓ |

Ningún caso cerró el programa ni entró en bucle infinito.

#### Recorridos del juego

| Caso | Pasos | Resultado obtenido | ¿Coincide? |
| --- | --- | --- | --- |
| Misión por defecto sin corregir (fracaso) | m=26000, v=10900, 10 × continuar | Fracasa en el tramo 10; distancia máxima 314 442 km; `Resultado: Fracaso` | ✓ |
| Misión por defecto corrigiendo a tiempo (éxito) | m=26000, v=10900, reserva=150000; 9 × continuar, turno 10 → corrección 60000 J/kg, continuar | `¡La nave alcanzó el punto de equilibrio!`; `Resultado: Éxito`; puntaje 94.9/100 | ✓ |
| Inyección con v=10906 sin correcciones (éxito) | m=26000, v=10906, 10 × continuar | `¡La nave alcanzó el punto de equilibrio!`; `Resultado: Éxito`; 0 correcciones; puntaje 99.6/100 | ✓ |
| Abortar la misión | m=26000, v=10900; turno 1 → abortar | `Resultado: Abortada`; distancia máxima 6563 km; puntaje 0.0/100 | ✓ |
| Último reporte sin misiones jugadas | Opción 4 antes de jugar | `Todavía no hay misiones jugadas` | ✓ |
| Consultar pregunta a con valores por defecto | Menú 2 → a → valores por defecto | a_T = 9.25340 m/s²; a_L = 3.441503×10⁻⁵ m/s² | ✓ |
| Consultar pregunta b con valores por defecto | Menú 2 → b → valores por defecto | Ec = 1.544530×10¹² J; Ec/m = 59 405 000 J/kg | ✓ |
| Consultar pregunta c con v=0 | Menú 2 → c → v=0, h=192000 | W/m = 59 462 086 J/kg; sin mensaje de advertencia | ✓ |
| Consultar pregunta c con v=10900 | Menú 2 → c → v=10900, h=192000 | W/m = 57 086 J/kg; `Con esa velocidad, Orion no alcanza el punto de equilibrio sin trabajo adicional` | ✓ |
| Consultar pregunta d con valores por defecto | Menú 2 → d → valores por defecto | Pozo Tierra 60.73 MJ/kg; Pozo Luna 2.82 MJ/kg; razón 21.52; energía ida 1.579×10¹² J; energía regreso 7.337×10¹⁰ J | ✓ |
| Salir del programa | Opción 0 | `Saliendo del simulador. ¡Hasta la próxima!` sin mensaje de error | ✓ |

### Registro - 2026-11-15 · Windows 11 Pro · WinLibs MinGW-w64 GCC 16.2.0

#### Prueba rápida del README

Verificación repetida por Daniel Acevedo durante la revisión del Pull Request, sobre una clonación nueva del commit `3d04250`, cuyos fuentes coinciden con los de este Pull Request. Compilación con `mingw32-make` y `mingw32-make pruebas`: 96 pruebas del simulador y 9 de consultas, sin fallas. Se conserva íntegro el registro anterior.

Se suministraron por entrada estándar las mismas respuestas de teclado indicadas abajo. Cada recorrido terminó con la opción `0`, código de salida 0 y sin fin de entrada inesperado. Las entradas inválidas volvieron a pedir el dato y aceptaron el valor válido siguiente. Los resultados reemplazan los mensajes y el entorno no acreditados del registro anterior del Pull Request.

En las misiones: `1` → nivel `4` → masa `26000`, reserva `150000`, eventos `n`. Cada respuesta se confirma con Enter.

| Caso | Entrada | Resultado obtenido | ¿Coincide? |
| --- | --- | --- | --- |
| Misión fallida | Velocidad `10900`, `c` en los 10 turnos | `Resultado: Fracaso` | Sí |
| Misión exitosa corrigiendo | Velocidad `10900`, 9 veces `c`, luego `r`, `60000`, `c` | `Resultado: Éxito`; `Correcciones usadas: 1`; `Puntaje final: 94.9 / 100` | Sí |
| Misión exitosa sin corregir | Velocidad `10906`, `c` en los 10 turnos | `Resultado: Éxito`; `Correcciones usadas: 0`; `Puntaje final: 99.6 / 100` | Sí |
| Último reporte | `4` después del éxito sin corregir | Se repite el reporte: `Resultado: Éxito`; `Puntaje final: 99.6 / 100` | Sí |
| Consulta a | `2`, `a`, `s` | `Atracción de la Tierra (a_T): 9.25340 m/s²`; `Atracción de la Luna (a_L): 3.441503e-05 m/s²` | Sí |
| Consulta b | En consultas, `b`, `s` | `Energía cinética total (Ec): 1.544530e+12 J` | Sí |
| Consulta c | En consultas, `c`, `s` | `Trabajo adicional mínimo (W/m): 59462085.69 J/kg` | Sí; 59 462 086 J/kg redondeado, como indica el README. |
| Consulta d | En consultas, `d`, `s`; después `v` | `Pozo gravitatorio terrestre: 60.73 MJ/kg`; `Pozo gravitatorio lunar: 2.82 MJ/kg`; `Razón Tierra / Luna: 21.52`; vuelve al menú. | Sí |
| Letra en menú | `a`, después `0` | `Entrada inválida. Ingrese un número entre 0 y 4:` | Sí |
| Línea vacía en menú | Enter, después `0` | `Entrada inválida. Ingrese un número entre 0 y 4:` | Sí |
| Entero fuera de rango en menú | `9`, después `0` | `Opción inexistente. Ingrese un número entre 0 y 4:` | Sí |
| Decimal en menú | `1.5`, después `0` | `Entrada inválida. Ingrese un número entre 0 y 4:` | Sí |
| Masa cero | `0`, después `26000` | `La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:` | Sí |
| Masa sobre el máximo | `200000`, después `26000` | `La masa debe estar entre 1 000 y 100 000 kg. Ingrese nuevamente:` | Sí |
| Velocidad fuera de rango | `9000`, después `10900` | `La velocidad debe estar entre 10 500 y 11 200 m/s. Ingrese nuevamente:` | Sí |
| Letra en trabajo de corrección | `r`, `abc`, después `60000` | `Entrada inválida. Ingrese un número positivo:`; acepta la corrección. Luego `a` aborta y vuelve al menú. | Sí |
| Salir | `0` | `Saliendo del simulador. ¡Hasta la próxima!` | Sí |

El README original coincide con las ejecuciones; se conserva su prueba rápida, sus instrucciones y el historial de uso de IA.
