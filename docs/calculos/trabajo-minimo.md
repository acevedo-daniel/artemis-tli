# Trabajo Mínimo por Kilogramo hasta el Punto de Equilibrio Tierra-Luna

## Datos utilizados

| Variable | Descripción | Valor | Unidad | Fuente |
| --- | --- | --- | --- | --- |
| $G$ | Constante de gravitación universal | $6{,}674 \times 10^{-11}$ | $\text{N}\cdot\text{m}^2/\text{kg}^2$ | Consigna, Problema 2 |
| $M_T$ | Masa de la Tierra | $5{,}972 \times 10^{24}$ | $\text{kg}$ | Consigna, Problema 2 |
| $M_L$ | Masa de la Luna | $7{,}346 \times 10^{22}$ | $\text{kg}$ | [NASA Moon Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html) |
| $D$ | Distancia media Tierra-Luna | $384\,000\,000$ | $\text{m}$ | Consigna, Problema 3 |
| $R_T$ | Radio medio de la Tierra | $6\,371\,000$ | $\text{m}$ | Consigna, Problema 2 |
| $h$ | Altitud de la órbita baja (LEO) | $192\,000$ | $\text{m}$ | Consigna, Problema 2 y 3 |
| $r_0$ | Distancia inicial al centro de la Tierra ($R_T + h$) | $6\,563\,000$ | $\text{m}$ | Consigna, Problema 2 y 3 |

## Fórmula

La energía potencial gravitatoria por unidad de masa en un punto situado a distancia $x$ del centro de la Tierra (sobre la línea que une los centros de la Tierra y la Luna) es:

$$\frac{U(x)}{m} = -\frac{G \cdot M_T}{x} - \frac{G \cdot M_L}{D - x}$$

El punto de equilibrio gravitatorio $x_{\text{eq}}$ entre ambos cuerpos se ubica en:

$$x_{\text{eq}} = \frac{D}{1 + \sqrt{\dfrac{M_L}{M_T}}}$$

Partiendo del reposo en la órbita baja ($r_0$) hasta alcanzar el punto de equilibrio ($x_{\text{eq}}$) con rapidez nula ($\Delta K = 0$), el trabajo mínimo por kilogramo requerido por conservación de la energía mecánica es:

$$\frac{W}{m} = \frac{U(x_{\text{eq}})}{m} - \frac{U(r_0)}{m}$$

## Sustitución de valores

**Paso 1 — productos gravitatorios:**

$$G \cdot M_T = 6{,}674 \times 10^{-11} \times 5{,}972 \times 10^{24} = 3{,}985\,712\,8 \times 10^{14} \ \text{m}^3/\text{s}^2$$

$$G \cdot M_L = 6{,}674 \times 10^{-11} \times 7{,}346 \times 10^{22} = 4{,}902\,720\,4 \times 10^{12} \ \text{m}^3/\text{s}^2$$

**Paso 2 — punto de equilibrio ($x_{\text{eq}}$) y distancia restante hasta la Luna ($D - x_{\text{eq}}$):**

$$x_{\text{eq}} = \frac{384\,000\,000}{1 + \sqrt{\dfrac{7{,}346 \times 10^{22}}{5{,}972 \times 10^{24}}}} \approx 345\,662\,973{,}58 \ \text{m}$$

$$D - x_{\text{eq}} = 384\,000\,000 - 345\,662\,973{,}58 = 38\,337\,026{,}42 \ \text{m}$$

**Paso 3 — energía potencial por kilogramo en $r_0$ (con $D - r_0 = 384\,000\,000 - 6\,563\,000 = 377\,437\,000 \ \text{m}$):**

$$\frac{U(r_0)}{m} = -\frac{3{,}985\,712\,8 \times 10^{14}}{6\,563\,000} - \frac{4{,}902\,720\,4 \times 10^{12}}{377\,437\,000}$$

$$\frac{U(r_0)}{m} \approx -60\,730\,044 \ \text{J/kg} - 12\,990 \ \text{J/kg} = -60\,743\,034 \ \text{J/kg}$$

**Paso 4 — energía potencial por kilogramo en $x_{\text{eq}}$:**

$$\frac{U(x_{\text{eq}})}{m} = -\frac{3{,}985\,712\,8 \times 10^{14}}{345\,662\,973{,}58} - \frac{4{,}902\,720\,4 \times 10^{12}}{38\,337\,026{,}42}$$

$$\frac{U(x_{\text{eq}})}{m} \approx -1\,153\,063 \ \text{J/kg} - 127\,885 \ \text{J/kg} = -1\,280\,948 \ \text{J/kg}$$

**Paso 5 — trabajo mínimo por kilogramo:**

$$\frac{W}{m} = -1\,280\,948 \ \text{J/kg} - (-60\,743\,034 \ \text{J/kg}) = 59\,462\,086 \ \text{J/kg}$$

## Resultado

$$\frac{W}{m} \approx 59\,462\,086 \ \text{J/kg} \approx 59{,}46 \ \text{MJ/kg}$$

## Conclusión

Llevar un kilogramo de masa desde la órbita terrestre baja ($r_0 = 6\,563\,000\ \text{m}$) hasta el punto de equilibrio gravitatorio Tierra-Luna, partiendo del reposo, requiere un trabajo mínimo de $59\,462\,086\ \text{J/kg}$ ($59{,}46\ \text{MJ/kg}$). Casi la totalidad de esa energía se invierte en remontar el pozo de potencial gravitatorio de la Tierra, ya que la atracción lunar en las cercanías de $r_0$ es mínima en comparación.
