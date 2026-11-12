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

El repositorio contiene la organización del equipo; todavía no hay código fuente. La etapa actual y las siguientes están en el [roadmap de desarrollo](docs/DEVELOPMENT-ROADMAP.md), y las tareas se siguen en el [tablero del proyecto](https://github.com/users/acevedo-daniel/projects/4).

## Escenario asignado

Según la [consigna](docs/catedra/consigna.pdf), estando en órbita baja (LEO), la etapa superior ICPS ejecuta la maniobra de Inyección Translunar e impulsa a Orion hasta `v_TLI = 10900 m/s`. Después la ICPS se desacopla, los motores se apagan y la nave (`m = 26000 kg`) inicia una trayectoria de retorno libre hacia la Luna, a una distancia media Tierra-Luna `D = 384000000 m`.

A partir de valores ingresados por el usuario, el programa debe responder:

- cómo se comporta el movimiento de Orion sin propulsión según la 1.ª Ley de Newton y cómo la gravedad terrestre la desacelera;
- la energía cinética inicial al comenzar el viaje translunar;
- el trabajo adicional mínimo por unidad de masa para llevar la nave desde la órbita baja (`h = 192000 m`) hasta el punto de equilibrio gravitatorio Tierra-Luna;
- por qué, desde el punto de vista energético, el viaje Tierra-Luna requiere mucho más combustible que el regreso.

Las reglas del juego, el menú y las instrucciones de compilación se agregan a este documento a medida que el equipo las define e implementa.

## Documentación

- [Roadmap de desarrollo](docs/DEVELOPMENT-ROADMAP.md): etapas, alcance y criterios de finalización.
- [Guía de trabajo con GitHub](docs/DEVELOPMENT.md): cómo tomar una tarea, crear ramas, abrir Pull Requests y revisar.
- [Consigna de la cátedra](docs/catedra/consigna.pdf).

## Registro de uso de inteligencia artificial

La consigna exige registrar cada uso de herramientas de IA: herramienta, tarea, consultas realizadas y cómo se validó o adaptó el resultado. Cada integrante agrega una fila en el mismo Pull Request que incorpora el trabajo asistido.

| Fecha | Integrante | Herramienta | Tarea | Consultas | Validación y adaptación |
| --- | --- | --- | --- | --- | --- |
| 10/11/2026 | Mario Daniel Acevedo | Claude Code (Anthropic) | Auditoría de la documentación inicial y preparación de la base de colaboración: formularios de Issues, plantilla de Pull Request, `.gitignore`, instrucciones para agentes (`AGENTS.md`), README, guía de GitHub y roadmap | "Audita mis documentos iniciales y cómo se conformó; crea un plan de implementación para las tareas iniciales" | Revisión manual de cada archivo contra la consigna; configuración de GitHub verificada con `gh` |
| 11/11/2026 | Mario Daniel Acevedo | Claude Code (Anthropic) | Auditoría de la base implementada y del roadmap contra la consigna; reglas de idioma y traducción de formularios y plantilla de Pull Request | "Audita lo que generó el plan y que el roadmap coincida con la consigna; agrega reglas para que la mayoría esté en español" | Comparación del roadmap con cada sección de la consigna y la rúbrica; formularios validados como YAML |
| 12/11/2026 | Mario Daniel Acevedo | Antigravity (Google Gemini) | Redacción del cálculo del trabajo mínimo por kilogramo hasta el punto de equilibrio Tierra-Luna en `docs/calculos/trabajo-minimo.md` | "Leer el Issue del trabajo mínimo por kilogramo, implementarlo y hacer el commit siguiendo las reglas del repositorio" | Verificación de fórmulas contra la consigna y comprobación numérica en Python sin redondeos intermedios |
