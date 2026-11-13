# Simulador Artemis TLI — Especificación funcional

> Qué ingresa el usuario, qué sucede en cada paso y qué responde el programa.

## Escenario

Problema 3 de la [consigna](catedra/consigna.pdf): estando en órbita baja (`h = 192000 m`), la etapa ICPS ejecuta la Inyección Translunar (TLI) y lleva a Orion (`m = 26000 kg`) hasta `v_TLI = 10900 m/s`. Después la ICPS se desacopla, los motores se apagan y la nave sigue una trayectoria libre hacia la Luna, a `D = 384000000 m`. Los desarrollos a mano están en [cálculos de referencia](calculos/README.md).

El programa es un juego por consola: el jugador dirige la misión desde la inyección hasta el punto de equilibrio gravitatorio Tierra-Luna, y en el camino el programa responde las cuatro preguntas del Problema 3 con los valores que el jugador ingresa.

## Supuestos del modelo

- **Una dimensión:** la nave se mueve sobre la recta Tierra-Luna, alejándose del centro de la Tierra.
- **Luna fija:** la Luna no se mueve durante el viaje; se usa la distancia media `D`.
- **Sin propulsión:** después de la TLI solo actúan las gravedades de la Tierra y de la Luna, salvo las correcciones que aplica el jugador.
- **Conservación de la energía:** la velocidad en cada punto se obtiene de `Em = K + U`, sin integrar en el tiempo.

Energía potencial por kilogramo:

```text
U(x)/m = −G·M_T / x − G·M_L / (D − x)
```

Velocidad a una distancia `x`, partiendo de `r0` con velocidad `v0`:

```text
v(x) = √( v0² + 2·(U(r0)/m − U(x)/m) )
```

Si el valor dentro de la raíz es negativo, la nave no alcanza esa distancia.

**Consecuencia:** con estos supuestos y `v_TLI = 10900 m/s`, Orion no alcanza el punto de equilibrio: se detiene a unos 314 442 km del centro de la Tierra, antes de los 345 663 km del punto. La velocidad mínima para llegar es de unos 10 905,24 m/s, y el trabajo adicional mínimo es de unos 57 086 J/kg. En una misión real la diferencia se compensa porque la Luna se mueve y su gravedad atrae la nave a lo largo de una trayectoria que no es recta; además, las misiones aplican correcciones de trayectoria después de la TLI. El juego convierte esa diferencia en su desafío: el jugador debe aportar el trabajo que falta.

## Datos, constantes y unidades

| Simbolo | Descripción | Valor | Unidad | Fuente |
| --- | --- | --- | --- | --- |
| $m$ | masa de Orion | 26 000 | kg | Consigna, Problema 3 |
| $v_TLI$ | velocidad de inyección | 10 900 | m/s | Consigna, Problema 3 |
| $h$ | altitud de la órbita baja | 192 000 | m | Consigna, Problema 3 |
| $D$ | distancia media Tierra-Luna | 384 000 000 | m | Consigna, Problema 3 |
| $G$ | constante de gravitación universal | 6,674 × 10⁻¹¹ | N·m²/kg² | Consigna, Problema 2 |
| $M_T$ | masa de Tierra | 5,972 × 10²⁴ | kg | Consigna, Problema 2 |
| $R_T$ | radio medio de la Tierra | 6 371 000 | m | Consigna, Problema 2 |
| $r0 = R_T + h$ | radio de la órbita baja | 6 563 000 | m | Consigna, Problema 2 y 3 |
| $M_L$ | masa de la Luna | 7,346 × 10²² | kg | [NASA Moon Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html) |
| $R_L$ | radio medio de la Luna | 1 737 400 | m | [NASA Moon Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html) |

Los datos que el usuario debe ingresar son: masa de Orion ($m$), la velocidad de inyección ($v_TLI$) y la altitud de la órbita baja ($h$), los cuales utilizan los valores de la tabla como valores por defecto. El resto de los parámetros funcionan como constantes fijas en el programa.

## Dinámica del juego

### Menú principal

| Opción | Acción |
| --- | --- |
| `1` | Nueva misión |
| `2` | Consultas del Problema 3: submenú con las preguntas `a`, `b`, `c` y `d` |
| `3` | Datos y supuestos del modelo |
| `4` | Último reporte; si todavía no se jugó ninguna misión, muestra `Todavía no hay misiones jugadas` |
| `0` | Salir |

El menú se repite hasta que el jugador elige `0`. Una opción inexistente muestra un aviso y vuelve a pedir la opción.

### Misión en cuatro etapas

**Etapa 1 — Inyección (pregunta b).** El jugador ingresa la masa de la nave (por defecto, 26 000 kg) y la velocidad de inyección (por defecto, 10 900 m/s). El programa muestra la energía cinética total y por kilogramo.

**Etapa 2 — Vuelo sin propulsión (pregunta a).** El trayecto desde `r0` hasta el punto de equilibrio se divide en 10 tramos iguales. En cada turno el programa muestra:

- la barra de progreso, por ejemplo `Tierra ●────────▶──────○ Luna`;
- la telemetría: distancia al centro de la Tierra, velocidad, atracción de la Tierra y de la Luna por separado, desaceleración neta y energía cinética;
- la predicción: velocidad al final del tramo siguiente, o el aviso `La nave no alcanza el próximo punto`;
- la reserva de corrección disponible.

Después el jugador decide:

| Decisión | Efecto |
| --- | --- |
| Continuar | La nave avanza al tramo siguiente |
| Corregir | El jugador ingresa un trabajo `W` en J/kg, no mayor que la reserva; la velocidad pasa a `√(v² + 2·W)` y la reserva baja en `W` |
| Abortar | La misión termina como abortada |

Después de una corrección, el programa recalcula la predicción y el jugador vuelve a decidir en el mismo turno. Puede corregir varias veces mientras le quede reserva.

La explicación de la 1.ª Ley de Newton acompaña la telemetría: la nave conserva su estado de movimiento por inercia, y lo único que la frena es la gravedad terrestre, cada vez menos a medida que se aleja.

**Etapa 3 — Punto de equilibrio (pregunta c).** Si la nave alcanza el punto de equilibrio, la misión es un éxito: a partir de ahí domina la atracción de la Luna. El programa compara el trabajo aplicado por el jugador con el trabajo adicional mínimo.

**Etapa 4 — Regreso (pregunta d).** No se juega. El reporte compara la energía por kilogramo necesaria para salir del pozo gravitatorio de la Tierra con la necesaria para salir del de la Luna, y la convierte en energía total con la masa ingresada.

### Corrección como trabajo adicional

La reserva de corrección se mide en trabajo por kilogramo (J/kg). Corregir es aplicar un trabajo externo que aumenta la energía mecánica, `Em = K + U`: es el trabajo adicional de la pregunta c. Como se mide por kilogramo, el momento de la corrección no cambia la energía total que se necesita, pero sí importa corregir antes de que la nave se detenga.

La masa no decide si la nave llega: la trayectoria depende de la energía por kilogramo. La masa solo cambia la energía total, en joules, que muestra el reporte.

### Condiciones de fin

| Resultado | Condición |
| --- | --- |
| Éxito | La nave alcanza el punto de equilibrio |
| Fracaso | El jugador elige continuar y la nave no alcanza el próximo punto; el programa informa la distancia máxima alcanzada y que la nave vuelve a caer hacia la Tierra |
| Abortada | El jugador elige abortar |

### Desempeño y reporte final

El puntaje va de 0 a 100 y nunca es negativo. Solo una misión exitosa suma puntos: parte de 100 y descuenta el exceso de energía y cada corrección usada. Gana quien llega gastando lo justo.

```text
energía aportada / m = v_TLI² / 2 + trabajo aplicado en correcciones
exceso / m           = energía aportada / m − (U(x_eq)/m − U(r0)/m)
```

El mínimo `U(x_eq)/m − U(r0)/m` es el trabajo de la pregunta c: 59 462 086 J/kg. Quien calcula la pregunta c puede elegir una inyección que llega sin correcciones: el juego premia usar la física.

El reporte final muestra:

- la masa, la velocidad de inyección y la energía cinética inicial (pregunta b);
- el resultado, la distancia máxima y la velocidad al llegar;
- el trabajo aplicado en correcciones frente al trabajo adicional mínimo (pregunta c);
- la comparación energética entre la ida y el regreso (pregunta d);
- el puntaje y las correcciones usadas.

### Reglas de equilibrio

Todo valor del juego tiene que cumplir estas reglas; cada una se convierte en un caso de prueba.

1. Sin correcciones, la misión por defecto (`v_TLI = 10900 m/s`) no alcanza el punto de equilibrio.
2. Jugando bien, cada misión predefinida se puede ganar aun con el peor caso de eventos: la reserva que queda después de la peor fuga alcanza para aportar el trabajo que falta.
3. Ningún evento modifica la velocidad ni la energía de la nave.

Valores iniciales; se confirman al implementar el núcleo de cálculo, comprobando las reglas. Con la reserva propuesta, la peor fuga deja 112 500 J/kg, más que los 57 086 J/kg que faltan en la misión por defecto.

| Parámetro | Valor inicial |
| --- | --- |
| Rango de la velocidad de inyección | 10 500 a 11 200 m/s |
| Rango de la masa | 1 000 a 100 000 kg |
| Reserva de corrección | 150 000 J/kg |
| Descuento por exceso de energía | 0,05 puntos por kJ/kg |
| Descuento por corrección | 5 puntos |

### Recorrido de referencia

Misión por defecto, sin correcciones ni eventos. Desaceleración neta: atracción de la Tierra menos la de la Luna.

| Punto | Distancia (km) | Velocidad con 10 900 m/s | Velocidad con 10 906 m/s | Desaceleración (m/s²) |
| --- | --- | --- | --- | --- |
| 0 | 6 563 | 10 900,0 | 10 906,0 | 9,25336 |
| 1 | 40 473 | 4 128,9 | 4 144,8 | 0,24328 |
| 2 | 74 383 | 2 841,2 | 2 864,1 | 0,07199 |
| 3 | 108 293 | 2 172,7 | 2 202,6 | 0,03392 |
| 4 | 142 203 | 1 723,4 | 1 761,0 | 0,01963 |
| 5 | 176 113 | 1 377,5 | 1 424,2 | 0,01274 |
| 6 | 210 023 | 1 084,3 | 1 143,1 | 0,00887 |
| 7 | 243 933 | 813,5 | 890,3 | 0,00645 |
| 8 | 277 843 | 534,2 | 645,1 | 0,00473 |
| 9 | 311 753 | 128,9 | 384,0 | 0,00316 |
| 10 | 345 663 | no llega | 129,1 | 0 |

Con el valor de la consigna, los primeros turnos se ven bien y el problema aparece recién al final: el jugador tiene que leer la telemetría y corregir a tiempo.

### Eventos

En cada turno ocurre como máximo un evento. Los eventos son físicamente creíbles, no agregan fórmulas nuevas y afectan la reserva o la información del jugador, nunca la física de la nave.

| Evento | Efecto |
| --- | --- |
| Fuga en el sistema de propulsión | La reserva de corrección baja entre 10 % y 25 % |
| Pérdida de señal | En ese turno no se puede corregir; no ocurre en un turno en el que la predicción avisa que la nave no alcanza el próximo punto |
| Falla de sensor | En ese turno no se muestra la predicción |

Para que las pruebas y la demostración del video sean repetibles, la misión puede jugarse con una semilla fija o con los eventos desactivados.

### Niveles

| Misión | Masa | Reserva | Eventos |
| --- | --- | --- | --- |
| Prueba sin tripulación | 26 000 kg | amplia | pocos |
| Misión tripulada | 26 000 kg | 150 000 J/kg | normales |
| Misión con carga | mayor: más energía total en juego, la trayectoria no cambia | justa | frecuentes |
| Personalizada | la ingresa el jugador | la ingresa el jugador | activados o desactivados |

### Orden de implementación

Cada capa se integra completa antes de empezar la siguiente; si el tiempo no alcanza, lo ya integrado cumple la consigna.

1. **Núcleo:** menú, consultas a–d, las cuatro etapas, correcciones con reserva de trabajo, abortar, reporte con puntaje, validaciones y barra de progreso.
2. **Eventos:** los tres de la tabla, con semilla fija y opción para desactivarlos.
3. **Niveles:** las tres misiones predefinidas y la personalizada.
4. **Extras, si sobra tiempo:** modo entrenamiento y modo misión (el segundo oculta la predicción), pronóstico del jugador antes de inyectar, historial de intentos de la sesión, regreso jugable y dispersión de la inyección (la velocidad real difiere un poco de la elegida). La dispersión cambia la energía de la nave: si se agrega, primero hay que modificar la regla 3 y recalcular la reserva con la regla 2.

## Preguntas del Problema 3

Cada pregunta tiene una opción en el submenú de consultas, donde el usuario ingresa sus propios valores, y además aparece dentro de la misión.

### Pregunta a — Vuelo sin propulsión y 1.ª Ley de Newton

#### Entradas

| Entrada | Unidad | Valor por defecto | Rango válido |
| --- | --- | --- | --- |
| h (altitud de la órbita baja) | m | 192 000 | 160 000 a 2 000 000 |
| r (distancia al centro de la Tierra donde se evalúa) | m | 6 563 000 | 6 531 000 a 345 663 000 |

Rango de h: mismo que en la Pregunta d (160 km a 2 000 km, límites convencionales del LEO). Rango de r: desde r0 (órbita baja) hasta el punto de equilibrio Tierra-Luna, que es el tramo que recorre la nave.

#### Salida

- Explicación breve de la 1.ª Ley de Newton: sin fuerza neta la nave seguiría a velocidad constante en línea recta; la gravedad terrestre la frena con una aceleración que disminuye con 1/r² a medida que se aleja.
- Atracción de la Tierra: G·M_T / r², en m/s².
- Atracción de la Luna: G·M_L / (D − r)², en m/s².

#### Fórmula

Véase [calculos/primera-ley-newton.md](calculos/primera-ley-newton.md):

```text
a_T = G · M_T / r²
a_L = G · M_L / (D − r)²
```

#### Resultado de referencia

Calculado con r = r0 = 6 563 000 m:

| Magnitud | Valor |
| --- | --- |
| Atracción de la Tierra en r0 | 9,25 m/s² |

#### En la misión

Etapa 2 — vuelo sin propulsión: la explicación y las atracciones acompañan la telemetría en cada turno (véase `### Misión en cuatro etapas`).

### Pregunta b — Energía cinética inicial

#### Entradas

| Entrada | Unidad | Valor por defecto | Rango válido |
| --- | --- | --- | --- |
| m (masa de la nave) | kg | 26 000 | 1 000 a 100 000 |
| v (velocidad de inyección) | m/s | 10 900 | 10 500 a 11 200 |

#### Salida

- Energía cinética total: Ec = ½·m·v², en J.
- Energía cinética por kilogramo: Ec/m = ½·v², en J/kg.

#### Fórmula

Véase [calculos/energia-cinetica-inicial.md](calculos/energia-cinetica-inicial.md):

```text
Ec   = ½ · m · v²
Ec/m = ½ · v²
```

#### Resultado de referencia

Calculado con los valores por defecto (m = 26 000 kg, v = 10 900 m/s):

| Magnitud | Valor |
| --- | --- |
| Ec | 1,54453 × 10¹² J |
| Ec/m | 59 405 000 J/kg |

#### En la misión

Etapa 1 — inyección: el programa muestra Ec y Ec/m después de que el jugador ingresa la masa y la velocidad de inyección (véase `### Misión en cuatro etapas`).

### Pregunta c — Trabajo mínimo hasta el punto de equilibrio

#### Entradas

| Entrada | Unidad | Valor por defecto | Rango válido |
| --- | --- | --- | --- |
| v (velocidad en la órbita baja) | m/s | 0 | 0 a 11 200 |
| h (altitud de la órbita baja) | m | 192 000 | 160 000 a 2 000 000 |

Rango de v: de 0 (caso de la consigna, partiendo del reposo) hasta el máximo de inyección del juego (11 200 m/s, de `### Reglas de equilibrio`). Rango de h: mismo que en las demás preguntas (LEO convencional).

#### Salida

- x_eq (punto de equilibrio): en m y en km.
- U(r0)/m: energía potencial por kilogramo en la órbita baja, en J/kg.
- U(x_eq)/m: energía potencial por kilogramo en el punto de equilibrio, en J/kg.
- W/m: trabajo adicional mínimo por kilogramo, en J/kg.
- Si v > 0 y W/m > 0: `Con esa velocidad, Orion no alcanza el punto de equilibrio sin trabajo adicional`.

#### Fórmula

Véase [calculos/punto-de-equilibrio.md](calculos/punto-de-equilibrio.md) y [calculos/trabajo-minimo.md](calculos/trabajo-minimo.md):

```text
U(x)/m = −G·M_T / x − G·M_L / (D − x)
W/m    = U(x_eq)/m − (v²/2 + U(r0)/m)
```

#### Resultado de referencia

| v | W/m | Notas |
| --- | --- | --- |
| 0 m/s | 59 462 086 J/kg | Respuesta de la consigna (partiendo del reposo) |
| 10 900 m/s | 57 086 J/kg | Caso de la misión; nave se detiene a 314 442 km; v_min ≈ 10 905,24 m/s |

#### En la misión

Etapas 2 y 3: el trabajo W/m es la reserva de corrección que el jugador puede aplicar durante el vuelo sin propulsión (etapa 2) y el umbral que debe superar para alcanzar el punto de equilibrio (etapa 3); véase `### Corrección como trabajo adicional`.

### Pregunta d — Comparación energética entre la ida y el regreso

#### Entradas

| Entrada | Unidad | Valor por defecto | Rango válido |
| --- | --- | --- | --- |
| m (masa de la nave) | kg | 26 000 | 1 000 a 100 000 |
| h (altitud de la órbita baja) | m | 192 000 | 160 000 a 2 000 000 |

Rango de h: las órbitas bajas terrestres van desde los 160 km (mínimo estable antes de que el rozamiento atmosférico sea significativo) hasta los 2 000 km (límite superior convencional del LEO).

#### Salida

- Pozo gravitatorio terrestre: G·M_T / r0, con r0 = R_T + h, en MJ/kg.
- Pozo gravitatorio lunar: G·M_L / R_L, en MJ/kg.
- Razón Tierra / Luna (adimensional).
- Energía total para salir del pozo terrestre: Pozo Tierra × m, en J.
- Energía total para salir del pozo lunar: Pozo Luna × m, en J.
- Conclusión de una o dos líneas sobre la diferencia entre la ida y el regreso.

#### Fórmula

Véase [calculos/energia-ida-vuelta.md](calculos/energia-ida-vuelta.md):

```text
Pozo Tierra = G · M_T / r0    (r0 = R_T + h)
Pozo Luna   = G · M_L / R_L
```

#### Resultado de referencia

Calculado con los valores por defecto (m = 26 000 kg, h = 192 000 m, r0 = 6 563 000 m):

| Magnitud | Valor |
| --- | --- |
| Pozo Tierra | 60,73 MJ/kg |
| Pozo Luna | 2,82 MJ/kg |
| Razón Tierra / Luna | 21,52 |
| Energía total ida (m = 26 000 kg) | 1,579 × 10¹² J |
| Energía total regreso (m = 26 000 kg) | 7,332 × 10¹⁰ J |

#### En la misión

Etapa 4 del juego y parte del reporte final (véase `### Desempeño y reporte final`): el programa muestra la comparación energética después de que la nave alcanza (o no) el punto de equilibrio.

## Entidades, estados y módulos

_Pendiente._

## Flujo de consola

_Pendiente._

## Casos de prueba

### Cálculos

_Pendiente._

### Entradas inválidas y recorridos del juego

_Pendiente._
