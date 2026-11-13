# Trabajo mínimo por kilogramo hasta el punto de equilibrio (Pregunta c - Problema 3)

## Datos utilizados

| Variable | Valor | Unidad | Fuente |
| --- | --- | --- | --- |
| G | 6,674 × 10⁻¹¹ | N·m²/kg² | Consigna, Problema 2 |
| MT | 5,972 × 10²⁴ | kg | Consigna, Problema 2 |
| ML | 7,346 × 10²² | kg | [NASA Moon Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html) |
| D | 384 000 000 | m | Consigna, Problema 3 |
| r0 | 6 563 000 | m | Consigna, Problemas 2 y 3 |
| x_eq | 345 662 974 | m | [punto-de-equilibrio.md](punto-de-equilibrio.md) |

## Fórmula

U(x)/m = −G · MT / x − G · ML / (D − x)

W/m = U(x_eq)/m − U(r0)/m

## Sustitución de valores

U(r0)/m = −3,9857128 × 10¹⁴ / 6 563 000 − 4,9027204 × 10¹² / 377 437 000 = −60 743 034 J/kg

U(x_eq)/m = −3,9857128 × 10¹⁴ / 345 662 974 − 4,9027204 × 10¹² / 38 337 026 = −1 280 948 J/kg

## Resultado

W/m = −1 280 948 − (−60 743 034) = 59 462 086 J/kg ≈ 59,46 MJ/kg

## Caso con la velocidad de la TLI

v = 10 900 m/s → Ec/m = v² / 2 = 10 900² / 2 = 118 810 000 / 2 = 59 405 000 J/kg

W/m = U(x_eq)/m − (Ec/m + U(r0)/m)
    = −1 280 948 − (59 405 000 + (−60 743 034))
    = −1 280 948 − (−1 338 034)
    = 57 086 J/kg

Con v = 10 900 m/s la nave ya tiene 59 405 000 J/kg de energía cinética; el trabajo adicional se reduce a 57 086 J/kg. Sin ese aporte, la nave se detiene a unos 314 442 km del centro de la Tierra, antes de los 345 663 km del punto de equilibrio. La velocidad mínima de inyección para llegar sin corrección es de unos 10 905,24 m/s.

## Conclusión

Partiendo del reposo en la órbita baja, llevar cada kilogramo hasta el punto de equilibrio requiere unos 59,46 MJ. Casi toda esa energía se usa para salir del pozo gravitatorio de la Tierra; el aporte de la Luna es mínimo.
