# Simulador Artemis TLI — Instrucciones para agentes

## Proyecto

Trabajo integrador en equipo (UTN FRRe, Algoritmos y Estructuras de Datos): un simulador interactivo por consola, escrito en C, del escenario Artemis de inyección translunar (Problema 3 de la consigna). El repositorio contiene la documentación y la organización del equipo; todavía no hay código fuente.

## Comandos

- **Verificar y compilar:** `make` (en Windows, `mingw32-make`); compila con `-Werror`, así que una advertencia es un error.
- **Ejecutar:** `./artemis_tli` (en Windows, `.\artemis_tli.exe`).
- **Pruebas:** todavía no hay pruebas automáticas; los casos manuales están en `docs/PROJECT.md` › Casos de prueba.
- **CI:** `.github/workflows/ci.yml` ejecuta `make`; el check obligatorio es `CI Gate`.

## Rutas clave

Consigna: `docs/catedra/consigna.pdf`; documentación: `docs/`; formularios de Issues y plantilla de Pull Request: `.github/`.

## Tecnología

- **Lenguaje:** C, compilado con GCC y ejecutado nativamente en consola
- **Compilación:** GCC y un `Makefile`, que se agrega en la etapa de estructura modular

## Reglas de código

- Respetar la división en módulos que exige la consigna: `main.c` contiene únicamente `main()`, el menú principal y la inicialización del flujo; `simulador.h` contiene constantes, definiciones `struct`/`typedef` y prototipos públicos; `simulador.c` contiene la lógica de simulación y los cálculos físicos; `validaciones.c` y `validaciones.h` contienen el ingreso seguro de enteros, reales y caracteres, y los mensajes de advertencia.
- Validar cada dato ingresado por tipo y rango; una entrada inválida vuelve a pedir el dato y nunca cierra el programa ni genera un bucle infinito.
- Mantener el código lo bastante simple para que cada integrante pueda explicarlo y defenderlo en el coloquio.
- No subir ejecutables, archivos temporales, credenciales ni datos personales más allá de la lista de integrantes del README.

## Siempre

- Ejecutar el comando de verificación antes de dar un trabajo por terminado, cuando exista; nunca afirmar que una comprobación pasó si no se ejecutó.
- Si la misma comprobación falla después de dos intentos de corrección, detenerse e informar el comando, la salida y la hipótesis.
- Nunca debilitar, omitir ni reintentar una comprobación para que pase.
- Escribir en español: documentación, Issues, títulos y cuerpos de Pull Requests, descripciones de commits y ramas, comentarios, identificadores de C y textos de consola. Esta regla reemplaza el inglés por defecto de la skill `git-delivery`.
- Mantener en inglés solo lo que por convención va en inglés: los prefijos de tipo (`feat`, `fix`, `refactor`, `test`, `docs`, `build`, `chore`), los nombres de archivo convencionales (`README.md`, `AGENTS.md`, `Makefile`, `docs/DEVELOPMENT.md`), las palabras clave de C y la biblioteca estándar, la palabra de cierre de GitHub (`Closes #N`) y los estados del tablero (`Backlog`, `In progress`, `In review`, `Done`).
- Títulos de commits y Pull Requests: `<tipo>: <verbo en infinitivo y minúsculas> <resultado>`, sin punto final; por ejemplo `chore: agregar reglas de .gitignore y fin de línea` o `feat: agregar menú de misión`.
- Los nombres de ramas y los identificadores de C usan solo ASCII, sin tildes ni `ñ`: `feat/menu-mision`, `calcular_energia_cinetica`.
- Las etiquetas de planificación (números de etapa, tarea o milestone) nunca aparecen en ramas, commits, Pull Requests, archivos ni identificadores.
- El código actual es la evidencia de lo que existe; `docs/DEVELOPMENT-ROADMAP.md` es el objetivo aprobado.
- Todo cambio asistido por IA agrega una línea en README › Registro de uso de inteligencia artificial, en el mismo Pull Request.
- Cada integrante abre los Pull Requests de su propio trabajo; nunca hacer commits ni abrir Pull Requests en nombre de otro integrante.
- `local-docs/` está ignorado por Git y nunca se enlaza desde archivos versionados: planes activos en `execution/`, borradores de Issues en `issues/`, material que conserva el coordinador en `keep/` y todo otro archivo generado por agentes (scripts, textos de PR, capturas, salidas de compilación) en `tmp/`. Borrar el contenido de `tmp/`, los planes cuyos Pull Requests ya se integraron y los borradores de Issues ya publicados no requiere confirmación; nunca borrar `keep/`.

## Preguntar antes de

- Agregar una biblioteca o dependencia externa.
- Borrar archivos o funciones públicas declaradas en un `.h`.
- Cambiar workflows de CI, la configuración del repositorio o la protección de ramas.
- Editar el roadmap aprobado (`docs/DEVELOPMENT-ROADMAP.md`).
- Crear ramas, commits, pushes o Pull Requests que no se pidieron.

## Skills

El coordinador tiene estas skills del playbook instaladas a nivel usuario; los agentes que no las tienen siguen este archivo y `docs/DEVELOPMENT.md`.

| Trabajo | Skill |
| --- | --- |
| Planificar una etapa o un resultado de varios pasos | `plan-implementation` |
| Implementar una tarea planificada o una corrección | `implement-task` |
| Escribir o reparar pruebas | `write-tests` |
| Configurar o cambiar CI, o proteger `main` | `setup-ci` |
| Textos de ramas, commits, Pull Requests o squash | `git-delivery` |
| Delegar trabajo a un integrante | `delegate-task` |
| Auditar el proyecto contra el playbook | `apply-playbook` |
| Registrar un error recurrente de los agentes | `record-lesson` |

## Delegación

- **Idioma de los Issues:** español
- **Tablero:** GitHub Project `acevedo-daniel` #4 (<https://github.com/users/acevedo-daniel/projects/4>); cada Issue lleva además el milestone de la etapa en curso del roadmap

Los Issues delegados se escriben con la skill `delegate-task` o con el formulario `Tarea` (`.github/ISSUE_TEMPLATE/task.yml`). Si un Issue no coincide con el repositorio o el siguiente paso no está claro, comentar en el Issue y esperar.
