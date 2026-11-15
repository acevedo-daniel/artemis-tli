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
| 5. Ejecución del Programa | `.\artemis_tli.exe` | Muestra el menú principal, al ingresar la opción de salida muestra `Saliendo del simulador. ¡Hasta la próxima!` y finaliza con código 0. | Sí |
| 6. Compilación Directa | `gcc -Wall -Wextra -std=c11 src/*.c -o artemis_tli.exe` | Compila el ejecutable directamente con GCC sin advertencias ni errores. | Sí |

---

### Registro - 2026-11-15 · Windows 11 / MSYS2 UCRT64 · GCC 13.2.0

#### Prueba rápida del README

| Caso | Entrada | Resultado obtenido | ¿Coincide? |
| :--- | :--- | :--- | :--- |
| 4. Misión fallida | `1` → `4` → Nivel 4 (Masa: 26000, Reserva: 150000, Eventos: n) → Vel: `10900` → `c` (10 turnos) | En el turno 10 la nave no alcanza el equilibrio y el reporte indica: `[REPORT] Resultado: Fracaso`. | Sí |
| 5. Misión exitosa corrigiendo | Igual al caso 4, pero en turno 10: `r` → `60000` → `c` | La nave alcanza el punto de equilibrio y el reporte indica: `[REPORT] Resultado: Éxito`, reflejando el descuento de puntaje por corrección. | Sí |
| 6. Misión exitosa sin corregir | Igual al caso 4, con Vel: `10906` | Lleva la trayectoria al punto de equilibrio sin correcciones con un puntaje final de 99,6. | Sí |
| 7. Último reporte | Menú principal → `4` | Vuelve a mostrar en pantalla el reporte completo de la última misión ejecutada. | Sí |
| 8. Consultas teórico-prácticas | Menú principal → `2` → `a` → `s`, `b` → `s`, `c` → `s`, `d` → `s`, `v` | Muestra la información teórica y respuestas de cada inciso (con c: trabajo mínimo 59 462 086 J/kg). | Sí |
| 9a. Entrada inválida menú | `a` | `[ERROR] Opción inválida. Ingrese un número entre 0 y 4.` | Sí |
| 9b. Entrada inválida menú | Línea vacía | `[ERROR] Opción inválida. Ingrese un número entre 0 y 4.` | Sí |
| 9c. Entrada inválida menú | `9` | `[ERROR] Opción inválida. Ingrese un número entre 0 y 4.` | Sí |
| 9d. Entrada inválida menú | `1.5` | `[ERROR] Opción inválida. Ingrese un número entre 0 y 4.` | Sí |
| 9e. Entrada inválida masa | `0` | `[ERROR] La masa debe ser un valor positivo mayor a 0.` | Sí |
| 9f. Entrada inválida masa | `200000` | `[ERROR] Masa fuera del rango permitido.` | Sí |
| 9g. Entrada inválida velocidad | `9000` | `[ERROR] Velocidad fuera del rango operativo de TLI (10000 - 11500 m/s).` | Sí |
| 9h. Entrada inválida corrección | `abc` | `[ERROR] Comando no reconocido. Ingrese 'c' para continuar o 'r' para reorientar/corregir.` | Sí |
| 10. Salir | `0` | `Saliendo del simulador. ¡Hasta la próxima!` | Sí |
