# Punto de Equilibrio Gravitatorio Tierra-Luna

## Datos utilizados

| Variable | Valor | Unidad | Fuente |
|----------|-------|--------|--------|
| D (distancia media Tierra-Luna) | 384 000 000 | m | Consigna, Problema 3 |
| M_T (masa de la Tierra) | 5,972 × 10²⁴ | kg | Consigna, Problema 2 |
| M_L (masa de la Luna) | 7,346 × 10²² | kg | [NASA Moon Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html) |

## Fórmula

El punto de equilibrio x_eq es aquel donde la atracción gravitatoria de la Tierra iguala a la de la Luna:

$$x_{eq} = \frac{D}{1 + \sqrt{\dfrac{M_L}{M_T}}}$$

## Sustitución de valores

**Paso 1 — cociente de masas:**

$$\frac{M_L}{M_T} = \frac{7{,}346 \times 10^{22}}{5{,}972 \times 10^{24}} = 0{,}012301$$

**Paso 2 — raíz cuadrada del cociente:**

$$\sqrt{0{,}012301} = 0{,}110913$$

**Paso 3 — denominador:**

$$1 + 0{,}110913 = 1{,}110913$$

**Paso 4 — punto de equilibrio:**

$$x_{eq} = \frac{384\,000\,000 \text{ m}}{1{,}110913} \approx 345\,650\,000 \text{ m}$$

**Paso 5 — distancia desde el punto de equilibrio hasta la Luna:**

$$D - x_{eq} = 384\,000\,000 - 345\,650\,000 = 38\,350\,000 \text{ m}$$

## Resultado

| Magnitud | Valor en metros | Valor en kilómetros |
|----------|----------------|---------------------|
| x_eq (desde el centro de la Tierra) | 345 650 000 m | 345 650 km |
| D − x_eq (desde el punto hasta la Luna) | 38 350 000 m | 38 350 km |

## Conclusión

El punto de equilibrio gravitatorio Tierra-Luna se encuentra a aproximadamente 345 650 km del centro de la Tierra, lo que equivale a cerca del 90 % de la distancia total entre ambos cuerpos. Esto refleja la enorme diferencia de masas entre la Tierra y la Luna: como la Tierra es unas 81 veces más masiva, su influencia gravitatoria domina casi hasta las cercanías de la Luna. A partir de ese punto, la atracción lunar supera a la terrestre y la nave comienza a ser "capturada" gravitacionalmente por la Luna.
