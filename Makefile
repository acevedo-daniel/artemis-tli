CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Werror
FUENTES = src/main.c src/simulador.c src/validaciones.c
CABECERAS = src/simulador.h src/validaciones.h
EJECUTABLE = artemis_tli

$(EJECUTABLE): $(FUENTES) $(CABECERAS)
	$(CC) $(CFLAGS) $(FUENTES) -o $(EJECUTABLE) -lm
