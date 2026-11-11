# Simulador de Misiones Artemis — Guía simple de GitHub

> Guía breve para un equipo de cinco personas que recién empieza a trabajar con GitHub.

## Qué utilizamos

- **GitHub**: sitio donde vive el código y las tareas.
- **GitHub Desktop**: aplicación visual para trabajar sin memorizar comandos de Git.
- **VS Code u otro editor**: donde escribimos el código C.
- **GCC**: compilador de C. Todos deben tener un modo de compilar el proyecto; se documentará el comando real una vez creada la base.

## Configuración inicial (una sola vez)

1. Una persona crea el repositorio **público** con `README.md`; activa **Issues** y sube los archivos de este kit (`docs/`, `.github/` y `.gitignore`).
2. Invita a los otros cuatro desde **Settings → Collaborators** (la ubicación exacta del menú puede variar).
3. Cada integrante acepta la invitación, instala GitHub Desktop e inicia sesión.
4. En GitHub Desktop, usa **File → Clone repository** y selecciona el repositorio compartido.
5. Como equipo, opcionalmente crean un único Project sencillo con columnas **To do**, **In progress** y **Done**. No necesitan automatizaciones ni campos personalizados.
6. Un integrante confirma que el proyecto compila en su equipo; después todos reproducen la misma instrucción.

## Cómo se trabaja con una tarea

1. El responsable abre su **Issue** y lee objetivo, contexto, pasos, criterio de finalización y lo que no debe tocar.
2. En GitHub Desktop, selecciona `main`, usa **Fetch origin** y **Pull origin** si hay cambios; crea una rama nueva desde esa versión actualizada.
3. La rama describe el resultado en inglés, por ejemplo `feat/mission-menu` o `fix/numeric-input`.
4. Implementa **solo esa tarea** y prueba el resultado. Hace uno o más commits pequeños y descriptivos, por ejemplo `feat: add mission menu`.
5. Selecciona **Push origin** y crea un **Pull Request** hacia `main` desde GitHub Desktop o el sitio web.
6. En el PR, explica qué cambió y enlaza el Issue (`Closes #N`, reemplazando `N` por el número real). Otro integrante revisa y pide correcciones si hacen falta.
7. Tras aprobarse, se integra el PR. El responsable vuelve a `main`, ejecuta **Fetch origin / Pull origin** y crea una nueva rama para su siguiente tarea.

**Reglas de equipo:** no programar directamente en `main`; no subir archivos ejecutables ni temporales; no editar por cuenta propia el código que otra persona tiene asignado sin avisar; no fusionar cambios sin compilar o probar; y registrar commits reales a lo largo de todo el trabajo.

## Cómo nos organizamos

- **Roadmap:** qué etapa estamos atravesando y cuándo termina; archivo `docs/DEVELOPMENT-ROADMAP.md`.
- **Plan de etapa:** decisiones y tareas de implementación, creado por quien coordina a partir de `PLAN.template.md` y guardado en `local-docs/execution/`.
- **Issue:** una tarea autocontenida y asignada a una persona. El equipo trabaja desde los Issues, no necesita acceder al plan local.
- **Pull Request:** propuesta de cambios verificable y revisable antes de unirla a `main`.
- **README:** explicación pública del programa y, obligatoriamente, detalle del uso de IA.

Los cinco deben tener trabajo real de código y participar en la comprensión general del proyecto. Revisar PRs, probar y documentar también ayuda, pero no debe convertirse en una excusa para que una sola persona programe todo.

## Formato mínimo de cada Issue

El repositorio trae `.github/ISSUE_TEMPLATE/task.yml`, basado en el formulario `delegate-task` del playbook, para que GitHub solicite automáticamente estos campos:

```md
### Goal
Qué resultado concreto debe existir.

### Context
Archivos y reglas necesarias para empezar.

### Steps
1. Paso específico.
2. Paso específico.

### Done when
- [ ] Se observa el comportamiento esperado.
- [ ] Se ejecutó la comprobación indicada.

### Deliver
Pull request that closes this issue

### Out of scope
Qué no debe modificarse.
```

En una etapa nueva, distribuir tareas **cuando ya estén decididos los módulos y sus dependencias**. No es obligatorio entregar cinco tareas simultáneas si alguna depende de otra; sí es obligatorio equilibrar los aportes a lo largo del proyecto.

## Qué no configuramos al principio

No hacen falta GitHub Actions, branch protection avanzada, reglas de aprobación complejas, múltiples proyectos, bots, Docker ni herramientas de gestión adicionales. Se pueden incorporar después si resuelven un problema concreto.
