# Explicación del Vuelo sin Propulsión y la 1.ª Ley de Newton

## Datos utilizados

| Variable | Descripción | Valor | Unidad | Fuente |
|---|---|---|---|---|
| $G$ | Constante de gravitación universal | $6{,}674 \times 10^{-11}$ | $\text{N}\cdot\text{m}^2/\text{kg}^2$ | Consigna, Problema 2 |
| $M_T$ | Masa de la Tierra | $5{,}972 \times 10^{24}$ | $\text{kg}$ | Consigna, Problema 2 |
| $R_T$ | Radio medio terrestre | $6{,}371 \times 10^6$ ($6.371.000$) | $\text{m}$ | Consigna, Problema 2 |
| $h$ | Altura de la órbita LEO previa | $192.000$ | $\text{m}$ | Consigna, Problema 2 y 3 |
| $r_0$ | Distancia inicial al centro de la Tierra ($R_T + h$) | $6.563.000$ | $\text{m}$ | Consigna, Problema 2 |
| $m$ | Masa de la nave Orion | $26.000$ | $\text{kg}$ | Consigna, Problema 3 |
| $v_{\text{TLI}}$ | Rapidez inicial de inyección translunar | $10.900$ | $\text{m/s}$ | Consigna, Problema 3 |

## Cálculo de apoyo: Desaceleración gravitatoria inicial

Para cuantificar cómo la gravedad terrestre frena a la nave inmediatamente después del apagado de motores, se calcula la aceleración gravitatoria que experimenta Orion en el radio inicial $r_0$.

### Fórmula

A partir de la Ley de Gravitación Universal ($F_g = G \frac{M_T \cdot m}{r^2}$) y la Segunda Ley de Newton ($F_g = m \cdot a$):

$$a = \frac{G \cdot M_T}{r_0^2}$$

### Sustitución de valores

$$a = \frac{6{,}674 \times 10^{-11} \ \frac{\text{N}\cdot\text{m}^2}{\text{kg}^2} \times 5{,}972 \times 10^{24} \ \text{kg}}{(6.563.000 \ \text{m})^2}$$

1. Producto en el numerador ($G \cdot M_T$):
   $$G \cdot M_T = 3{,}9857128 \times 10^{14} \ \text{m}^3/\text{s}^2$$

2. Denominador ($r_0^2$):
   $$r_0^2 = (6{,}563 \times 10^6 \ \text{m})^2 = 4{,}3072969 \times 10^{13} \ \text{m}^2$$

3. Aceleración:
   $$a = \frac{3{,}9857128 \times 10^{14}}{4{,}3072969 \times 10^{13}} \approx 9{,}2534 \ \text{m/s}^2$$

### Resultado

$$a \approx 9{,}25 \ \text{m/s}^2$$

## Explicación física (1.ª Ley de Newton)

La Primera Ley de Newton (Principio de Inercia) establece que todo cuerpo permanece en su estado de reposo o de movimiento rectilíneo uniforme a menos que actúe sobre él una fuerza neta no nula. Al finalizar la inyección translunar (TLI) y desacoplarse la etapa ICPS, los motores se apagan por completo ($F_{\text{prop}} = 0$). Aunque en el espacio exterior la fricción atmosférica es nula, la nave no está aislada de interacciones: sobre ella actúa la fuerza gravitatoria terrestre.

Al existir una fuerza neta no nula, el movimiento no es rectilíneo uniforme. La gravedad ejerce una fuerza de atracción central dirigida hacia la Tierra, en sentido opuesto al vector velocidad con el que Orion se aleja hacia la Luna. Esta fuerza genera una desaceleración inicial de aproximadamente $9{,}25 \ \text{m/s}^2$, frenando progresivamente a la nave a medida que asciende. Conforme Orion se distancia, dicha fuerza disminuye con el cuadrado de la distancia ($1/r^2$), haciendo que la tasa de frenado sea cada vez menor durante el trayecto.
