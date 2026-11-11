# ARTEMIS — Simulador de Inyección Translunar (TLI)

**Universidad Tecnológica Nacional — Facultad Regional Resistencia**  
**Asignatura:** Algoritmos y Estructuras de Datos — Laboratorio de C  
**Escenario asignado:** Problema 3, *Inyección Translunar — TLI e Inercia en Trayectoria Libre*  
**Comisión:** D  
**Fecha de elaboración del prototipo:** 31/10/2026

## Integrantes

| Legajo | Nombre y apellido |
|---|---|
| 25494 | Fernando Germán Ojeda |
| 31126 | Mario Daniel Acevedo |
| 29264 | Yvonne Vallejos |
| 29226 | María Luna Vaz |
| 31340 | Alan Quenardelle |


## Descripción del juego y objetivo

La etapa superior ICPS impulsa a Orion y se separa. La nave inicia una fase sin propulsión principal. El usuario configura la masa de la nave, altitud de partida, distancia Tierra-Luna, velocidad **radial** inicial y un paso máximo de integración. El simulador calcula la fuerza neta radial y la evolución de posición y velocidad en un modelo de dos centros gravitatorios **fijos** sobre una dimensión.

**Objetivo de juego:** superar la barrera de potencial y llegar al umbral de proximidad lunar situado a 10.000 km sobre la superficie de la Luna. La misión puede terminar en: llegada exitosa a la zona de aproximación, retorno por energía insuficiente, límite numérico/temporal o abandono voluntario. Se registran las últimas 12 simulaciones en memoria.

**Advertencia científica:** no se calcula una trayectoria real de retorno libre. Una trayectoria real requiere movimiento tridimensional, geometría orbital, movimiento de Tierra y Luna, velocidades tangenciales y, eventualmente, correcciones. Este modelo 1D **no permite demostrar un retorno a la Tierra** ni alunizaje. Los tiempos de vuelo son los de este modelo simplificado, no predicciones de la misión Artemis.

### Reglas del juego

1. Ir a `1) Nueva misión TLI`; elegir parámetros estándar o personalizados. Se validan tipo y rango de cada dato.
2. Elegir por evento: un paso de integración, 100 pasos, ejecución automática hasta evento, consultar análisis energético o abandonar.
3. Observar en cada turno: tiempo, posición, rapidez radial, punto de equilibrio y error normalizado de conservación de energía.
4. Al terminar, leer el reporte y revisar el historial de hasta 12 misiones.
5. Se puede volver a jugar con una velocidad mayor o menor para estudiar el umbral energético.

### Menú

```text
1) Nueva mision TLI (configurable)
2) Analisis energetico del caso base
3) Fundamentos y limitaciones fisicas
4) Historial de misiones en memoria
0) Salir
```

## Fundamento físico

Datos base provistos por la cátedra: `m = 26000 kg`, `v0 = 10900 m/s`, `h = 192000 m`, `D = 384000000 m`. Se complementan con constantes astronómicas aproximadas citadas en el informe: `G = 6.674e-11`, `MT = 5.972e24 kg`, `ML = 7.346e22 kg`, `RT = 6371000 m`, `RL = 1737400 m`.

En el eje Tierra → Luna (`x` = distancia al centro de la Tierra):

- `a(x) = -G*MT/x^2 + G*ML/(D-x)^2`.
- `U(x)/m = -G*MT/x - G*ML/(D-x)`.
- `E = (1/2)*m*v^2 + U(x)`.
- `x_eq = D/(1 + sqrt(ML/MT))`. Es el punto de **equilibrio de atracciones en el modelo estático 1D**, **no** el punto lagrangiano L1 del problema real rotante.
- `Wmin/m = [U(x_eq) - U(RT+h)]/m`, cuando se parte del reposo en el punto inicial idealizado.
- `vmin = sqrt(2*Wmin/m)` para el movimiento radial supuesto.

En el caso base se obtiene `x_eq ≈ 345662.97 km`, `Wmin/m ≈ 5.9462e7 J/kg`, `vmin ≈ 10905.24 m/s` y `K0 = 1.54453e12 J`. Por esta **idealización radial y elección de constantes**, `10900 m/s` no alcanza el equilibrio: faltan aproximadamente 5,24 m/s. **Trabajo externo adicional mínimo posterior a TLI:** `W_adicional/m = max(0, Wmin/m − v0²/2) ≈ 57085.69 J/kg`, equivalente a aproximadamente `1.4842e9 J` para la masa indicada, suponiendo una intervención externa ideal sin pérdidas. La energía desde reposo hasta la barrera (`5.9462e7 J/kg`) no es la misma que la energía **adicional** faltante después de TLI. Esto no invalida una misión espacial real; las trayectorias orbitales no son radiales.

> `Wmin` es una diferencia de energía potencial, no directamente la masa de combustible. Para evaluar propulsantes se necesita delta-v, impulso específico, masa variable y ecuación del cohete, entre otros datos.

## Estructura del proyecto

```text
Artemis_TLI_Grupo3/
├─ README.md                  # Instrucciones + declaración de IA
├─ Makefile                  # Entradas para compilar/probar
├─ compilar_windows.bat      # Compilación local con GCC en Windows
├─ codigo/
│  ├─ main.c                 # Interfaz, menú, coordinación
│  ├─ simulador.h            # Constantes, estructuras, prototipos
│  ├─ simulador.c            # Mecánica, integrador, reportes
│  ├─ validaciones.h         # Prototipos de captura segura
│  ├─ validaciones.c         # Validación de entero, real, s/n
│  ├─ tests.c                # Pruebas unitarias
│  └─ Makefile               # Compilación del programa
├─ documentacion/            # Informe y guion editables
└─ pruebas/                  # Casos de prueba reproducibles
```

### Modularidad, estructuras y punteros

- `Parametros`: configuración de cada misión.
- `Mision`: evolución del estado físico, tiempo y resultado.
- `Historial`: **arreglo de 12 registros** y contador de ocupación.
- `const Mision *`, `Mision *`, `Historial *` y `const Parametros *` permiten pasar y actualizar estructuras sin copiar registros grandes.
- `validaciones.c` centraliza entrada segura mediante `fgets` + `strtol`/`strtod`, rechazando cadenas, desbordes, valores fuera de rango, `NaN` e infinito. Se permite coma o punto decimal.
- El algoritmo de integración es **Velocity Verlet**, con paso máximo configurable y reducción adaptativa cerca de los cuerpos.

## Instalación y compilación

**Linux / macOS** (GCC y Make instalados):

```bash
make
./codigo/artemis_tli
make test
```

**Windows**: instalar GCC (por ejemplo, MSYS2/MinGW-w64), añadirlo al `PATH` y ejecutar `compilar_windows.bat` desde el Explorador. También puede compilarse en terminal:

```bat
cd codigo
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic main.c simulador.c validaciones.c -o artemis_tli.exe -lm
artemis_tli.exe
```

El ZIP se distribuye como **código fuente**, sin afirmar compatibilidad de un ejecutable compilado para todos los sistemas. Para volver a compilar se necesita un entorno con GCC.

## Demostración sugerida (video)

**Caso de éxito:** menú `1` → personalizar `n` → `26000`, `192000`, `384000000`, **`11000`**, `600` → elegir `3` para ejecutar automático. Se debería observar el estado **EXITOSA: ZONA LUNAR** y luego historial con la misión guardada.

**Caso de validación:** en el menú escribir `hola` o `9`; se rechaza y vuelve a pedir valor. Al personalizar masa ingresar `abc` o `-1`; el sistema repregunta sin terminar el programa.

**Caso base de la cátedra:** utilizar parámetros predeterminados (`s`) y ejecutar automático; debe observarse **SIN ENERGIA: RETROCESO** en el modelo simplificado, con explicación del umbral.

## Pruebas

`make test` ejecuta pruebas analíticas y de simulación: posición de equilibrio, trabajo por kilogramo, energía cinética, umbral radial, dos desenlaces, error energético e historial circular. Ver `pruebas/CASOS_DE_PRUEBA.md`.

El error mostrado en consola es `|E(t)-E0| / (K0 + |U0|)`, para evitar que la normalización sea inestable cuando `E0` es cercano a cero. Es un **error relativo respecto de una escala inicial de energía**, no un error porcentual relativo a `E0`.

## Fuentes

- Cátedra AED, *Trabajo práctico integrador final — Simulador de Misiones Artemis (Lenguaje C)*, consigna 2026.
- NASA, *Artemis I Press Kit*. https://www.nasa.gov/artemis-i-press-kit/
- NASA, *SLS Interim Cryogenic Propulsion Stage*. https://www.nasa.gov/reference/icps/
- NASA NSSDC, *Moon Fact Sheet*. https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html
- NASA, *Earth Fact Sheet*. https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html
- NASA NTRS, *Mission Trajectory Design*. https://ntrs.nasa.gov/api/citations/20140006564/downloads/20140006564.pdf

## Registro de utilización de inteligencia artificial (obligatorio)

**Herramienta:** ChatGPT (OpenAI). **Fecha del prototipo:** 31 de octubre de 2026. **Consultas utilizadas:** “Realiza este trabajo por completo... escenario 3 TLI... informe DOCX y guion”; búsquedas documentales sobre ICPS, TLI, gravedad, propiedades Tierra-Luna y referencias NASA.

**Tareas asistidas:** identificación de requisitos de la consigna, planteo de un simulador de consola en C, diseño inicial de módulos, propuestas de validación, redacción de documentación, guion de video, cálculos de control y diseño de pruebas.

**Validación efectuada para este prototipo:** compilación con GCC usando `-Wall -Wextra -Wpedantic`, ejecución de `make test` y ejecuciones por consola de caso exitoso, caso base e inputs inválidos (consultar archivos de pruebas). **Pendiente del equipo:** comprensión y revisión exhaustiva de cada función, validación en sus PCs, commits individuales reales, revisión científica del nivel de simplificación y registro de cualquier nueva consulta a IA. No atribuir commits ni participación no realizados.

**Adaptaciones realizadas durante la preparación:** selección explícita del modelo radial, separación en módulos, pruebas numéricas con umbral de conservación energético y corrección de criterio de normalización de error.

## Participación y entrega

El repositorio público y su historial de commits deben crearlo y mantenerlo los estudiantes. Se sugieren responsabilidades en `documentacion/PLAN_DE_COLABORACION.md`; son asignaciones propuestas y **no** evidencia de tareas realizadas. Subir el enlace de repositorio y el enlace del video con aparición y voces reales de los cinco integrantes. Completar datos de comisión y fecha final de entrega conforme aula virtual.
