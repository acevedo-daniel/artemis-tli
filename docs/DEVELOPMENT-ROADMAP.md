---
mode: decision
status: active
---

# Simulador de Misiones Artemis — Development roadmap

> Secuencia de etapas, límites de alcance y criterios de finalización. Las tareas y sus responsables se definen al comenzar cada etapa, no en este documento.

## Objective

Desarrollar en equipo un simulador/juego interactivo por consola en lenguaje C que represente correctamente el escenario Artemis asignado por la cátedra: Problema 3, inyección translunar (TLI). Construirlo de forma gradual, comprensible para cinco integrantes y verificable mediante GitHub, pruebas y una defensa técnica.

**Current phase:** Estructura modular del programa

## Constraints

- El escenario asignado es el Problema 3, *Inyección Translunar — TLI e Inercia en Trayectoria Libre*; la consigna completa está en `docs/catedra/consigna.pdf`.
- Equipo de cinco estudiantes, con participación real y progresiva de todos en el repositorio.
- Lenguaje C; aplicación ejecutada nativamente en consola, sin interfaz gráfica ni servicios externos.
- Menú principal, simulación interactiva por turnos o eventos y reporte final de desempeño.
- Uso de condicionales y ciclos, funciones con parámetros, `struct`/`typedef`, arreglos, punteros, validación de entradas y manejo de errores.
- Separación lógica mínima entre `main.c`, archivos de cabecera `.h`, módulo de simulación `.c` y módulo de validación/interfaz `.c/.h`.
- Repositorio **público** de GitHub, con commits graduales de todos los integrantes. Se recomiendan ramas y Pull Requests.
- `main` se integra con squash: cada Pull Request queda como un solo commit. Para que el historial muestre múltiples commits equitativos, cada integrante integra su trabajo en Pull Requests pequeños y frecuentes durante todo el período, no en uno grande al final.
- Entrega con código fuente, `Makefile` o scripts de compilación, `README.md` completo, registro transparente de IA y video de hasta cinco minutos con participación y voces reales de los cinco integrantes.
- Priorizar la claridad académica y la facilidad de mantenimiento: evitar dependencias, automatizaciones o arquitectura que no aporten al trabajo.
- Todo se escribe en español: documentación, Issues, Pull Requests, descripciones de commits y ramas, comentarios, identificadores y textos de consola. Se mantienen en inglés los prefijos de tipo (`feat`, `fix`, `docs`, `chore`…), los nombres de archivo convencionales (`README.md`, `Makefile`, `docs/DEVELOPMENT.md`) y las palabras clave de C y de GitHub. El detalle está en `docs/DEVELOPMENT.md`.

## Phases

| Phase | Outcome | Depends on |
| --- | --- | --- |
| Estructura modular del programa | Existe una aplicación base en C que compila y tiene interfaces entre módulos | Ninguna |
| Núcleo de cálculo y simulación | Los cálculos y cambios de estado del escenario producen resultados correctos | Estructura modular del programa |
| Interacción, validaciones y reportes | El usuario puede completar una simulación sin fallas ante entradas comunes o erróneas | Núcleo de cálculo y simulación |
| Integración, revisión y entrega | El proyecto cumple la consigna, está documentado y puede ser defendido por los cinco | Interacción, validaciones y reportes |

## Phase: Estructura modular del programa

### Objective

Disponer de una base compilable y navegable para que distintos integrantes puedan trabajar sobre responsabilidades separadas.

### In scope

- Crear `src/main.c`, `src/simulador.c`, `src/simulador.h`, `src/validaciones.c` y `src/validaciones.h`, ajustando nombres si el equipo aprueba una alternativa consistente.
- Definir `struct`, `typedef`, constantes y prototipos públicos alineados con las reglas del escenario.
- Implementar un menú mínimo y un flujo de salida correcto, aunque algunas opciones todavía sean provisionales.
- Añadir `Makefile` o scripts simples para compilar todo el proyecto y documentar cómo ejecutarlo.
- Acordar contratos entre funciones y parámetros para evitar que varios desarrolladores modifiquen el mismo bloque a la vez.
- Documentar módulos, estructuras y flujo entre archivos en `docs/ARCHITECTURE.md`, con el diagrama que luego usará el README.
- Agregar una verificación automática en GitHub Actions que compile el proyecto con advertencias tratadas como errores y sea obligatoria para integrar Pull Requests en `main`.

### Out of scope

- Completar todos los cálculos físicos, todas las mecánicas o añadir dependencias complejas.

### Exit criteria

- El programa completo compila sin errores usando el comando documentado.
- Al iniciarlo, el menú aparece, permite una elección básica y se puede salir correctamente.
- Los módulos se comunican mediante cabeceras y llamadas a funciones; `main.c` contiene únicamente `main()`, el menú principal y la inicialización del flujo, los cálculos viven en `simulador.c` y el ingreso seguro de datos en `validaciones.c`.
- Dos integrantes diferentes pueden trabajar sobre módulos distintos y fusionar sus cambios mediante Pull Requests.
- La verificación automática de compilación se ejecuta en cada Pull Request y bloquea la integración si falla.

## Phase: Núcleo de cálculo y simulación

### Objective

Lograr que el sistema resuelva correctamente la parte física y actualice su estado de acuerdo con las decisiones y eventos definidos.

### In scope

- Implementar funciones de cálculo por responsabilidad, respetando fórmulas, unidades, constantes y restricciones del escenario.
- Representar el estado de la misión con estructuras apropiadas y usar arreglos y punteros con un propósito claro, no solamente para cumplir una lista.
- Definir y procesar los cambios de estado propios de los turnos o eventos.
- Comparar resultados contra ejercicios resueltos manualmente y probar casos límite relevantes; registrar los casos y cómo ejecutarlos en `docs/TESTING.md`.
- Integrar los cálculos progresivamente, evitando esperar al final para unir el trabajo de cada integrante.

### Out of scope

- Embellecer toda la consola, preparar el video o incorporar escenarios Artemis no asignados.

### Exit criteria

- Las operaciones centrales del escenario entregan resultados consistentes con los casos de referencia.
- La misión evoluciona de un estado inicial a estados posteriores de manera predecible.
- El programa no divide por cero ni procesa estados físicamente inválidos sin control.
- Cada función relevante y el uso de estructuras/punteros pueden ser explicados por más de una persona del equipo.

## Phase: Interacción, validaciones y reportes

### Objective

Convertir el núcleo técnico en un simulador usable, robusto y demostrable de principio a fin.

### In scope

- Conectar menú, entradas del usuario, turnos/eventos, decisiones, cálculos y condiciones de fin.
- Validar tipos y rangos; permitir reintentos ante letras donde se esperan números, valores fuera de rango o entradas incompletas.
- Mostrar mensajes comprensibles y reportes finales con resultados y evaluación del desempeño.
- Verificar recorridos completos: misión exitosa, resultado alternativo/fallido y distintos errores de entrada.
- Ajustar los mensajes de consola para facilitar la demostración y la comprensión de quien evalúe.

### Out of scope

- Incorporar interfaz gráfica, guardar partidas, crear servicios web o ampliar el alcance con funcionalidades no exigidas.

### Exit criteria

- Una persona que no desarrolló el programa puede completar una simulación siguiendo el menú.
- Las entradas inválidas previstas no provocan cierres inesperados ni bucles infinitos.
- El reporte final refleja lo ocurrido en la simulación y los resultados de los cálculos.
- Se han ejecutado y registrado pruebas manuales de éxito, error y límites.

## Phase: Integración, revisión y entrega

### Objective

Entregar una versión reproducible que cumpla la consigna y pueda ser defendida técnicamente por cualquiera de los cinco integrantes.

### In scope

- Revisar en conjunto cálculo físico, menú, validaciones, arreglos, punteros, módulos y manejo de errores.
- Compilar desde una copia limpia del repositorio con las instrucciones publicadas, sin depender de archivos locales no incluidos.
- Completar el `README.md`: integrantes y comisión, escenario, reglas, funcionamiento, diagrama de módulos, compilación/ejecución y registro detallado de IA (herramienta, tarea, consultas/prompts y validación o adaptación).
- Revisar contribuciones graduales de todos, Issues cerrados y Pull Requests integrados.
- Grabar un video de hasta cinco minutos, subido a YouTube, Drive o Loom, en el que los cinco integrantes aparecen y se presentan con sus voces reales, explican la arquitectura y la modularización, y muestran un caso exitoso y las validaciones ante entradas erróneas. No se usa IA para generar o modificar voces ni para crear avatares.
- Preparar una explicación compartida de decisiones, fórmulas, flujo de ejecución y gestión de memoria para un posible coloquio.
- Efectuar la entrega digital formal en el aula virtual según indique la cátedra.

### Out of scope

- Nuevas características grandes o refactorizaciones cosméticas sin relación con defectos reales.

### Exit criteria

- La versión final compila desde cero y su ejecución coincide con el README.
- Los entregables solicitados existen, los enlaces funcionan y el video cumple las condiciones de la cátedra.
- Se verifica el cumplimiento de la rúbrica en modularización, validaciones, escenario, GitHub, README y defensa técnica.
- Cada uno de los cinco integrantes puede explicar el flujo, las estructuras principales y las decisiones críticas del código.

## Maintenance

Al completar una etapa, actualizar **Current phase** con el nombre exacto de la siguiente. Eliminar del documento activo la sección y la fila de la etapa terminada: el historial de Git conserva el registro anterior.

Al **comenzar cada etapa**, quien coordina revisa el estado real del repositorio y prepara un plan local de la etapa con decisiones, dependencias, tareas, criterios de aceptación y verificaciones. A partir de él crea Issues con el formulario Tarea, los asigna según capacidad y dependencias, y los agrega al tablero con el milestone de la etapa. Los Issues son la fuente de instrucciones compartida con el equipo.

**Flujo de ejecución:** roadmap → plan específico de etapa → Issues asignados → ramas y commits → Pull Requests revisados → verificación de criterios de salida → siguiente etapa.
