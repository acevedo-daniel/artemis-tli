#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Muestra el mensaje y lee un entero entre minimo y maximo; reintenta hasta que sea válido. */
int leer_entero(const char *mensaje, int minimo, int maximo, const char *aviso_rango);

/* Muestra el mensaje y lee un número real entre minimo y maximo; reintenta hasta que sea válido. */
double leer_real(const char *mensaje, double minimo, double maximo, const char *aviso_rango);

/* Muestra el mensaje y lee un carácter de opciones_validas; devuelve la letra en minúscula. */
char leer_caracter(const char *mensaje, const char *opciones_validas, const char *aviso_invalido);

#endif
