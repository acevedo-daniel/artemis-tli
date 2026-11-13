#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "simulador.h"
#include "validaciones.h"

static void mostrar_bienvenida(void)
{
    printf("========================================\n");
    printf("  SIMULADOR ARTEMIS TLI\n");
    printf("  Inyección translunar de Orion\n");
    printf("========================================\n");
}

static void mostrar_menu_principal(void)
{
    printf("\n");
    printf("MENÚ PRINCIPAL\n");
    printf("  1. Nueva misión\n");
    printf("  2. Consultas del Problema 3\n");
    printf("  3. Datos y supuestos del modelo\n");
    printf("  4. Último reporte\n");
    printf("  0. Salir\n");
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    ReporteFinal ultimo_reporte;
    int hay_reporte = 0;
    int opcion;

    mostrar_bienvenida();

    do {
        mostrar_menu_principal();
        opcion = leer_entero("Elija una opción: ", OPCION_SALIR, OPCION_MENU_MAXIMA, "Opción inexistente. Ingrese un número entre 0 y 4:");

        switch (opcion) {
            case 1:
                if (nueva_mision(&ultimo_reporte)) {
                    hay_reporte = 1;
                }
                break;
            case 2:
                submenu_consultas();
                break;
            case 3:
                mostrar_datos_y_supuestos();
                break;
            case 4:
                mostrar_ultimo_reporte(hay_reporte ? &ultimo_reporte : NULL);
                break;
            case OPCION_SALIR:
                break;
            default:
                break;
        }
    } while (opcion != OPCION_SALIR);

    printf("Saliendo del simulador. ¡Hasta la próxima!\n");
    return 0;
}