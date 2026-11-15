# artemis-tli

Simulador de mecánica celeste y análisis de la misión Artemis TLI (Trans-Lunar Injection).

## Compilación y ejecución

### Requisitos
- GCC (compilador C11)
- Make (o mingw32-make en Windows)

### Pasos
1. Clonar el repositorio:
   ```bash
   git clone https://github.com/acevedo-daniel/artemis-tli.git
   cd artemis-tli
   ```
2. Compilar el proyecto:
   - En Linux / macOS: `make`
   - En Windows: `mingw32-make`
3. Ejecutar las pruebas:
   - En Linux / macOS: `make pruebas`
   - En Windows: `mingw32-make pruebas`
4. Ejecutar el simulador:
   - En Linux / macOS: `./artemis_tli`
   - En Windows: `artemis_tli.exe`
5. Compilación directa sin Make (opcional):
   ```bash
   gcc -Wall -Wextra -std=c11 src/*.c -o artemis_tli.exe
   ```

---

## Registro de uso de inteligencia artificial

* **2026-11-14** · Estructura inicial del proyecto y generación de documentación de pruebas.
* **2026-11-15** · Verificación de compilación desde copia limpia (Issue #94) y estructuración del registro en `docs/TESTING.md`.
* **2026-11-15** · Ejecución y registro de la prueba rápida del README e identificación de entradas inválidas (Issue #95).
