# Simulador de Misiones Artemis — Guía de trabajo con GitHub

> Cómo colabora el equipo de cinco integrantes: herramientas, tablero, Issues, ramas y Pull Requests.

## Requisitos

| Herramienta | Uso |
| --- | --- |
| Cuenta de GitHub con acceso de colaborador | Ver Issues, crear ramas y abrir Pull Requests |
| [GitHub Desktop](https://desktop.github.com/) | Clonar, crear ramas, hacer commits y push sin usar la consola |
| VS Code u otro editor | Escribir el código C |
| GCC | Compilar el programa; los comandos están en el README, sección Compilación y ejecución |

## Primera configuración

Cada integrante, una sola vez:

1. Aceptar la invitación al repositorio desde el correo o desde <https://github.com/acevedo-daniel/trabajo-final-tpi-artemis/invitations>.
2. Instalar GitHub Desktop e iniciar sesión con la cuenta de GitHub.
3. En GitHub Desktop, usar **File → Clone repository** y elegir `acevedo-daniel/trabajo-final-tpi-artemis`.
4. Abrir el [tablero del proyecto](https://github.com/users/acevedo-daniel/projects/4) y confirmar que se pueden mover tarjetas.
5. En una terminal, ejecutar `gcc --version` y confirmar que GCC responde.

## Tablero del proyecto

Las tareas son Issues del repositorio y se siguen en el tablero [artemis-development](https://github.com/users/acevedo-daniel/projects/4). Los Issues nuevos se agregan solos en `Backlog`.

| Status | Significado | Quién lo mueve |
| --- | --- | --- |
| Backlog | Tarea creada, todavía no empezada | Automático al crear el Issue |
| In progress | El responsable trabaja en su rama | El responsable, al empezar |
| In review | Hay un Pull Request abierto esperando revisión | El responsable, al abrir el PR |
| Done | El Pull Request se integró y el Issue se cerró | Automático al integrar |

Cada Issue tiene un único responsable (Assignees) y el milestone de la etapa del roadmap en curso. `Priority` (`P0` a `P2`) indica el orden y `Size` (`XS` a `XL`), el tamaño estimado.

## Cómo se trabaja con una tarea

1. El responsable abre su Issue y lee Objetivo, Contexto, Pasos, Listo cuando y Fuera de alcance. Mueve la tarjeta a `In progress`.
2. En GitHub Desktop selecciona `main`, usa **Fetch origin** y **Pull origin**, y crea una rama nueva desde esa versión actualizada.
3. Implementa solo esa tarea, la compila y la prueba. Hace uno o más commits pequeños.
4. Usa **Push origin** y crea un Pull Request hacia `main`. Completa `Resumen` y `Cambios` de la plantilla; el último punto de `Resumen` es `Closes #N`, con el número real del Issue. Mueve la tarjeta a `In review`.
5. El coordinador revisa el Pull Request y lo aprueba o comenta qué corregir. El autor corrige en la misma rama y avisa en el Pull Request.
6. El coordinador integra con **Squash and merge**, revisando que el título siga el formato de nombres. La rama se borra sola y el Issue pasa a `Done`.
7. El autor vuelve a `main`, usa **Fetch origin** y **Pull origin**, y sigue con su próxima tarea.

Con **Squash and merge**, cada Pull Request queda en `main` como un único commit a nombre de quien lo abrió. Por eso cada integrante abre los Pull Requests de su propio trabajo: así el historial refleja los aportes reales de los cinco.

## Nombres de ramas, commits y Pull Requests

Se escriben en español y describen el resultado, sin números de etapa ni de tarea. El prefijo de tipo queda en inglés porque es la convención de GitHub.

| Elemento | Formato | Ejemplo |
| --- | --- | --- |
| Rama | `<tipo>/<resultado-en-kebab-case>`, sin tildes ni `ñ` | `feat/menu-mision` |
| Commit | `<tipo>: <verbo en infinitivo y minúsculas> <resultado>`, sin punto final | `feat: agregar menú de misión` |
| Pull Request | igual que un commit, describiendo el resultado completo | `fix: rechazar letras en el menú` |

Tipos: `feat` (funcionalidad nueva), `fix` (corrección), `refactor` (reorganización sin cambiar el comportamiento), `test` (pruebas), `docs` (documentación), `build` (compilación) y `chore` (mantenimiento).

## Idioma

Todo se escribe en español: documentación, Issues, Pull Requests, commits, comentarios del código, nombres de funciones y variables, y mensajes de consola. Los nombres en el código van sin tildes ni `ñ` (`calcular_energia_cinetica`).

Quedan en inglés solo las convenciones que no se traducen:

- los prefijos de tipo (`feat`, `fix`, `docs`, `chore`…);
- los nombres de archivo convencionales (`README.md`, `Makefile`, `docs/DEVELOPMENT.md`);
- las palabras clave de C y las funciones de la biblioteca estándar (`struct`, `printf`, `fgets`);
- `Closes #N` en el Pull Request, porque GitHub solo cierra el Issue con esa palabra;
- los estados del tablero (`Backlog`, `In progress`, `In review`, `Done`).

## Reglas del equipo

- No programar directamente en `main`.
- No subir ejecutables ni archivos temporales; `.gitignore` ya los excluye.
- No modificar el código asignado a otra persona sin avisar en su Issue.
- No integrar cambios sin compilar y probar.
- Registrar commits reales durante todo el trabajo.
- Registrar todo uso de IA en el README, en el mismo Pull Request.

## Cómo informar un bloqueo

Comentar en el Issue qué se intentó, qué falló (con el mensaje de error completo) y mencionar al coordinador. La tarjeta queda en `In progress`. Si el Issue no coincide con el repositorio o el siguiente paso no está claro, comentar y esperar antes de cambiar el alcance.

## Cómo nos organizamos

- **Roadmap:** etapa actual, alcance y criterios de finalización; archivo [DEVELOPMENT-ROADMAP.md](DEVELOPMENT-ROADMAP.md).
- **Plan de etapa:** al comenzar cada etapa, el coordinador revisa el repositorio y arma un plan con decisiones y tareas. A partir de él crea los Issues; el equipo trabaja solo desde los Issues.
- **Issue:** una tarea autocontenida, asignada a una persona y creada con el formulario Tarea. Los errores encontrados se informan con el formulario Reporte de error.
- **Pull Request:** propuesta de cambios que revisa e integra el coordinador.
- **README:** explicación pública del programa y registro obligatorio del uso de IA.

Los cinco deben tener trabajo real de código y comprender el proyecto completo. Revisar Pull Requests, probar y documentar también suma, pero no reemplaza programar.

Las tareas de una etapa se distribuyen cuando ya están decididos los módulos y sus dependencias. No hace falta repartir cinco tareas simultáneas si alguna depende de otra; sí hay que equilibrar los aportes a lo largo del proyecto.

## Qué no configuramos todavía

No usamos Docker, bots ni herramientas de gestión adicionales. La verificación automática de compilación con GitHub Actions se agrega en la etapa de estructura modular, cuando exista código para compilar.
