---
mode: decision
status: active
---

# Simulador de Misiones Artemis — Development roadmap

> Secuencia de etapas, límites de alcance y criterios de finalización. Las tareas y sus responsables se definen al comenzar cada etapa, no en este documento.

## Objective

Desarrollar en equipo un simulador/juego interactivo por consola en lenguaje C que represente correctamente el escenario Artemis que asigne la cátedra. Construirlo de forma gradual, comprensible para cinco integrantes y verificable mediante GitHub, pruebas y una defensa técnica.

**Current phase:** Preparación del equipo y GitHub

## Constraints

- El escenario específico será uno de los cuatro previstos por la consigna y se asignará por la cátedra. Este roadmap no presupone cuál será.
- Equipo de cinco estudiantes, con participación real y progresiva de todos en el repositorio.
- Lenguaje C; aplicación ejecutada nativamente en consola, sin interfaz gráfica ni servicios externos.
- Menú principal, simulación interactiva por turnos o eventos y reporte final de desempeño.
- Uso de condicionales y ciclos, funciones con parámetros, `struct`/`typedef`, arreglos, punteros, validación de entradas y manejo de errores.
- Separación lógica mínima entre `main.c`, archivos de cabecera `.h`, módulo de simulación `.c` y módulo de validación/interfaz `.c/.h`.
- Repositorio **público** de GitHub, con commits graduales de todos los integrantes. Se recomiendan ramas y Pull Requests.
- Entrega con código fuente, `Makefile` o scripts de compilación, `README.md` completo, registro transparente de IA y video de hasta cinco minutos con participación y voces reales de los cinco integrantes.
- Priorizar la claridad académica y la facilidad de mantenimiento: evitar dependencias, automatizaciones o arquitectura que no aporten al trabajo.
- En GitHub, usar ramas y mensajes de commit breves en inglés, siguiendo `git-delivery` del playbook. El material de coordinación para el equipo puede mantenerse en español.

## Phases

| Phase | Outcome | Depends on |
| --- | --- | --- |
| Preparación del equipo y GitHub | Los cinco pueden colaborar con un flujo simple y trazable | Ninguna |
| Definición del escenario y reglas | El problema asignado queda traducido a comportamientos y casos verificables | Preparación del equipo y GitHub |
| Estructura modular del programa | Existe una aplicación base en C que compila y tiene interfaces entre módulos | Definición del escenario y reglas |
| Núcleo de cálculo y simulación | Los cálculos y cambios de estado del escenario producen resultados correctos | Estructura modular del programa |
| Interacción, validaciones y reportes | El usuario puede completar una simulación sin fallas ante entradas comunes o erróneas | Núcleo de cálculo y simulación |
| Integración, revisión y entrega | El proyecto cumple la consigna, está documentado y puede ser defendido por los cinco | Interacción, validaciones y reportes |

## Phase: Preparación del equipo y GitHub

### Objective

Dejar un entorno colaborativo mínimo, donde cada integrante pueda aportar y revisar cambios sin necesidad de dominar Git por consola.

### In scope

- Crear un repositorio público específico para el integrador; habilitar Issues e invitar a los otros cuatro integrantes como colaboradores.
- Usar GitHub Desktop y el editor que ya conozca cada integrante; verificar que todos puedan clonar el repositorio y utilizar el compilador C elegido.
- Registrar instrucciones cortas de colaboración en `docs/DEVELOPMENT.md`.
- Crear un único tablero sencillo, si resulta útil: `To do`, `In progress`, `Done`. Cada Issue tendrá un responsable.
- Practicar el circuito completo: rama → cambio pequeño → commit → push → Pull Request → revisión → integración.
- Incorporar `README.md`, `.gitignore` y este roadmap al repositorio.

### Out of scope

- Programar todavía la simulación, distribuir permanentemente roles o configurar CI, reglas avanzadas y automatizaciones complejas.

### Exit criteria

- El repositorio es público, tiene Issues disponibles y sus cinco integrantes cuentan con acceso.
- Cada integrante demuestra que puede clonar, crear una rama, registrar un commit y proponer un Pull Request; sus primeros aportes quedan trazables.
- Existe un procedimiento acordado para revisar e integrar cambios, y `main` se mantiene como versión compartida estable.
- Todos saben dónde mirar la próxima tarea y cómo informar un bloqueo.

## Phase: Definición del escenario y reglas

### Objective

Transformar la situación física asignada en una especificación sencilla: qué ingresa el usuario, qué sucede en cada paso y qué debe responder el programa.

### In scope

- Confirmar el escenario adjudicado y registrar datos, constantes físicas, restricciones, unidades y preguntas que debe resolver la simulación.
- Definir la dinámica del juego: opciones de menú, decisiones del usuario, turnos o eventos, condición de finalización y reporte.
- Identificar entidades y estados que podrían requerir `struct`, arreglos y funciones; diseñar un flujo de consola en pseudocódigo o diagrama breve.
- Establecer ejemplos calculados a mano, casos normales, límites y entradas inválidas que luego servirán como pruebas.
- Resolver con la cátedra las ambigüedades físicas que cambien el comportamiento del juego antes de implementarlas.

### Out of scope

- Escribir todas las funciones o agregar mecánicas ajenas a la situación asignada.

### Exit criteria

- El escenario, las reglas, las entradas, los resultados y las condiciones de fin están documentados de modo que cualquier integrante pueda explicarlos.
- Hay ejemplos de cálculo esperados y casos de prueba concretos para las operaciones principales.
- Se conocen los módulos y sus responsabilidades, sin definir de forma prematura detalles innecesarios.
- El equipo aprueba la interpretación funcional antes de empezar la implementación completa.

## Phase: Estructura modular del programa

### Objective

Disponer de una base compilable y navegable para que distintos integrantes puedan trabajar sobre responsabilidades separadas.

### In scope

- Crear `src/main.c`, `src/simulador.c`, `src/simulador.h`, `src/validaciones.c` y `src/validaciones.h`, ajustando nombres si el equipo aprueba una alternativa consistente.
- Definir `struct`, `typedef`, constantes y prototipos públicos alineados con las reglas del escenario.
- Implementar un menú mínimo y un flujo de salida correcto, aunque algunas opciones todavía sean provisionales.
- Añadir `Makefile` o scripts simples para compilar todo el proyecto y documentar cómo ejecutarlo.
- Acordar contratos entre funciones y parámetros para evitar que varios desarrolladores modifiquen el mismo bloque a la vez.

### Out of scope

- Completar todos los cálculos físicos, todas las mecánicas o añadir dependencias complejas.

### Exit criteria

- El programa completo compila sin errores usando el comando documentado.
- Al iniciarlo, el menú aparece, permite una elección básica y se puede salir correctamente.
- Los módulos se comunican mediante cabeceras y llamadas a funciones; `main.c` no concentra los cálculos de dominio.
- Dos integrantes diferentes pueden trabajar sobre módulos distintos y fusionar sus cambios mediante Pull Requests.

## Phase: Núcleo de cálculo y simulación

### Objective

Lograr que el sistema resuelva correctamente la parte física y actualice su estado de acuerdo con las decisiones y eventos definidos.

### In scope

- Implementar funciones de cálculo por responsabilidad, respetando fórmulas, unidades, constantes y restricciones del escenario.
- Representar el estado de la misión con estructuras apropiadas y usar arreglos y punteros con un propósito claro, no solamente para cumplir una lista.
- Definir y procesar los cambios de estado propios de los turnos o eventos.
- Comparar resultados contra ejercicios resueltos manualmente y probar casos límite relevantes.
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
- Completar el `README.md`: integrantes y comisión, escenario, reglas, funcionamiento, compilación/ejecución y registro detallado de IA (herramienta, tarea, consultas/prompts y validación o adaptación).
- Revisar contribuciones graduales de todos, Issues cerrados y Pull Requests integrados.
- Grabar un video de hasta cinco minutos con los cinco integrantes, utilizando sus voces reales, explicando módulos y mostrando un caso exitoso y entradas inválidas.
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

Al completar una etapa, actualizar **Current phase** con el nombre exacto de la siguiente. Según el playbook, eliminar del documento activo la sección y la fila de la etapa terminada: el historial de Git conserva el registro anterior.

Al **comenzar cada etapa**, inspeccionar el estado real del repositorio y generar su plan en `local-docs/execution/<outcome-slug>.md` usando `PLAN.template.md` del `engineering-playbook`. Ese plan detalla decisiones, dependencias, tareas concretas, criterios de aceptación y verificaciones. A partir de él, crear Issues independientes con la plantilla `delegate-task`, asignándolos a los integrantes según capacidad y dependencias. Los Issues —no el documento local ignorado por Git— son la fuente de instrucciones compartida con el equipo.

**Flujo de ejecución:** roadmap → plan específico de etapa → Issues asignados → ramas y commits → Pull Requests revisados → verificación de criterios de salida → siguiente etapa.
