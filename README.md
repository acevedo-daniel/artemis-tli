# ARTEMIS — Simulador de Inyección Translunar (TLI)

Simulador interactivo por consola, escrito en C, del escenario Artemis asignado por la cátedra: la inyección translunar de la nave Orion y su vuelo sin propulsión hacia la Luna.

**Universidad Tecnológica Nacional — Facultad Regional Resistencia**<br>
**Asignatura:** Algoritmos y Estructuras de Datos — Laboratorio de C<br>
**Escenario asignado:** Problema 3, *Inyección Translunar — TLI e Inercia en Trayectoria Libre*<br>
**Comisión:** D

## Integrantes

| Legajo | Nombre y apellido |
| --- | --- |
| 25494 | Fernando Germán Ojeda |
| 31126 | Mario Daniel Acevedo |
| 29264 | Yvonne Vallejos |
| 29226 | María Luna Vaz |
| 31340 | Alan Quenardelle |

## Estado del proyecto

El programa tiene su estructura modular base: compila y separa el flujo principal, la simulación y las validaciones; las funcionalidades del juego se agregan por etapas. La etapa actual y las siguientes están en el [roadmap de desarrollo](docs/DEVELOPMENT-ROADMAP.md), y las tareas se siguen en el [tablero del proyecto](https://github.com/users/acevedo-daniel/projects/4).

## Escenario asignado

Según la [consigna](docs/catedra/consigna.pdf), estando en órbita baja (LEO), la etapa superior ICPS ejecuta la maniobra de Inyección Translunar e impulsa a Orion hasta `v_TLI = 10900 m/s`. Después la ICPS se desacopla, los motores se apagan y la nave (`m = 26000 kg`) inicia una trayectoria de retorno libre hacia la Luna, a una distancia media Tierra-Luna `D = 384000000 m`.

A partir de valores ingresados por el usuario, el programa debe responder:

- cómo se comporta el movimiento de Orion sin propulsión según la 1.ª Ley de Newton y cómo la gravedad terrestre la desacelera;
- la energía cinética inicial al comenzar el viaje translunar;
- el trabajo adicional mínimo por unidad de masa para llevar la nave desde la órbita baja (`h = 192000 m`) hasta el punto de equilibrio gravitatorio Tierra-Luna;
- por qué, desde el punto de vista energético, el viaje Tierra-Luna requiere mucho más combustible que el regreso.

Las reglas del juego están en la [especificación funcional](docs/PROJECT.md).

## Compilación y ejecución

Requiere GCC. Desde la raíz del repositorio:

| Sistema | Compilar | Ejecutar |
| --- | --- | --- |
| Linux o macOS | `make` | `./artemis_tli` |
| Windows (WinLibs) | `mingw32-make` | `.\artemis_tli.exe` |

Sin `make`, el mismo resultado se obtiene con:

    gcc -std=c11 -Wall -Wextra -pedantic -Werror src/main.c src/simulador.c src/validaciones.c -o artemis_tli -lm

Las advertencias se tratan como errores: el código tiene que compilar sin ninguna.

## Documentación

- [Especificación funcional](docs/PROJECT.md): escenario, supuestos del modelo, reglas del juego, entradas, resultados y casos de prueba.
- [Roadmap de desarrollo](docs/DEVELOPMENT-ROADMAP.md): etapas, alcance y criterios de finalización.
- [Tablero del proyecto](https://github.com/users/acevedo-daniel/projects/4): seguimiento de tareas y estado del desarrollo.
- [Guía de trabajo con GitHub](docs/DEVELOPMENT.md): cómo tomar una tarea, crear ramas, abrir Pull Requests y revisar.
- [Cálculos de referencia](docs/calculos/README.md): desarrollos a mano de las preguntas del Problema 3.
- [Consigna de la cátedra](docs/catedra/consigna.pdf).

## Registro de uso de inteligencia artificial

La consigna exige registrar cada uso de herramientas de IA: herramienta, tarea, consultas realizadas y cómo se validó o adaptó el resultado. Cada integrante agrega una línea, con el formato `fecha · integrante · herramienta — tarea. Consulta: … Validación: …`, en el mismo Pull Request que incorpora el trabajo asistido.

- 10/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Auditoría de la documentación inicial y base de colaboración (formularios, plantilla de Pull Request, `.gitignore`, `AGENTS.md`, README, guía de GitHub y roadmap). Consulta: "Audita mis documentos iniciales y cómo se conformó; crea un plan de implementación para las tareas iniciales". Validación: revisión de cada archivo contra la consigna; configuración de GitHub verificada con `gh`.
- 11/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Auditoría de la base y del roadmap; reglas de idioma y traducción de formularios. Consulta: "Audita lo que generó el plan y que el roadmap coincida con la consigna; agrega reglas para que la mayoría esté en español". Validación: roadmap comparado con la consigna y la rúbrica; formularios validados como YAML.
- 11/11/2026 · Mario Daniel Acevedo · Codex (OpenAI) — Pull Request de la base de colaboración y revisión del `.gitignore`. Consulta: "Cerrame el PR que mandó Germán y abrí un PR para hacer merge de todo esto". Validación: cambios comparados con el PR existente; `git diff --check`.
- 12/11/2026 · Alan Quenardelle · Claude (Anthropic) — Cálculo y corrección del punto de equilibrio Tierra-Luna (#12 y #17). Consulta: desarrollo paso a paso del cálculo y de su corrección. Validación: revisión del coordinador contra valores recalculados; corrección del redondeo pedida en el Issue #7.
- 12/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Auditoría de la etapa inicial y plan correctivo. Consulta: "Realiza una auditoría de la fase 1 y de ser necesario crea un plan correctivo". Validación: Issues, Pull Requests y ruleset verificados con `gh`; valores físicos recalculados con Python.
- 12/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Normalización ligera del formato de los cálculos de referencia. Consulta: "Agrega una normalización ligera, respetando las atribuciones de mis compañeros". Validación: diff revisado para conservar el trabajo de cada autor; redondeo del punto de equilibrio corregido.
- 12/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Idea inicial del juego y supuestos del modelo en `docs/PROJECT.md`. Consulta: "Diseñemos la idea inicial del juego respetando la consigna: misión en cuatro etapas, correcciones, eventos y niveles". Validación: consigna y rúbrica releídas; velocidades por tramo y trabajo mínimo recalculados con Python.
- 13/11/2026 · Alan Quenardelle · Claude Sonnet 4.6 (Anthropic) — Especificación de las preguntas a–d, entidades, flujo de consola y casos de entradas inválidas y recorridos (#30, #31, #32, #34 y #35). Consulta: desarrollo de las secciones pedidas en los Issues #24 a #27 y #29. Validación: auditoría del coordinador con valores recalculados en Python; ajustes integrados en #36.
- 13/11/2026 · Mario Daniel Acevedo · Gemini (Google) — Casos de prueba de los cálculos y de las reglas de equilibrio (#37). Consulta: cálculo de los casos normales, límites y de reglas pedidos en el Issue #28. Validación: los 20 resultados esperados recalculados con Python.
- 13/11/2026 · Mario Daniel Acevedo · Claude Code (Anthropic) — Auditoría de la etapa de definición del escenario y plan correctivo. Consulta: "Audita los PR integrados y genera un plan correctivo con solo las correcciones necesarias". Validación: valores de referencia recalculados con Python; correcciones aplicadas en #36.
- 13/11/2026 · Vaz Maria Luna · Claude Code (Anthropic) — Documentar los datos y constantes del escenario. Consulta: Desarrollo paso a paso de cómo documentar los datos y constante. Validación: revisión de los datos y constantes; correcciones aplicadas en #23.