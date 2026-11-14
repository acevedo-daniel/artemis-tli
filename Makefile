CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Werror
FUENTES = src/main.c src/simulador.c src/validaciones.c
CABECERAS = src/simulador.h src/validaciones.h
EJECUTABLE = artemis_tli

$(EJECUTABLE): $(FUENTES) $(CABECERAS)
	$(CC) $(CFLAGS) $(FUENTES) -o $(EJECUTABLE) -lm

PRUEBAS = pruebas/pruebas_simulador.c

ifeq ($(OS),Windows_NT)
    EJECUTAR_PRUEBAS = pruebas_simulador.exe
else
    EJECUTAR_PRUEBAS = ./pruebas_simulador
endif

.PHONY: pruebas
pruebas: $(PRUEBAS) src/simulador.c $(CABECERAS)
	$(CC) $(CFLAGS) -Isrc $(PRUEBAS) src/simulador.c -o pruebas_simulador -lm
	$(EJECUTAR_PRUEBAS)
