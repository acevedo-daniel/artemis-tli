# Pruebas del Proyecto artemis-tli

Este documento contiene el registro de pruebas manuales y verificaciones de compilación del simulador.

---

### Registro - 2026-11-14 · Linux x86_64 · GCC 11.4.0

#### Compilación inicial y pruebas unitarias

| Paso | Comando | Resultado obtenido | ¿Coincide? |
| :--- | :--- | :--- | :--- |
| Compilación | `make` | Genera ejecutable `artemis_tli` sin advertencias. | Sí |
| Pruebas | `make pruebas` | 0 fallas registradas en todos los módulos. | Sí |

---

### Registro - 2026-11-15 · Windows 11 / MSYS2 UCRT64 · GCC 13.2.0

#### Compilación desde copia limpia

| Paso | Comando | Resultado obtenido | ¿Coincide? |
| :--- | :--- | :--- | :--- |
| 2. Requisitos y Clonado | `gcc --version` <br> `git clone https://github.com/acevedo-daniel/artemis-tli.git` | Muestra `gcc.exe (Rev2, Built by MSYS2 project) 13.2.0` y clona el repositorio sin errores. | Sí |
| 3. Compilación con Make | `mingw32-make` | Compila todos los módulos (`main.c`, `simulador.c`, `validaciones.c`) y genera `artemis_tli.exe` sin advertencias ni errores. | Sí |
| 4. Ejecución de Pruebas | `mingw32-make pruebas` | Compila y ejecuta los tests unitarios (`pruebas_simulador` y `pruebas_consultas`). Ambos terminan con 0 fallas. | Sí |
| 5. Ejecución del Programa | `.rtemis_tli.exe` | Muestra el menú principal, al ingresar la opción de salida muestra `Saliendo del simulador. ¡Hasta la próxima!` y finaliza con código 0. | Sí |
| 6. Compilación Directa | `gcc -Wall -Wextra -std=c11 src/*.c -o artemis_tli.exe` | Compila el ejecutable directamente con GCC sin advertencias ni errores. | Sí |
