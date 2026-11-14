#include <stdio.h>
#include "simulador.h"

int nueva_mision(ReporteFinal *reporte)
{
    (void)reporte;
    printf("La nueva misión todavía no está disponible.\n");
    return 0;
}

void submenu_consultas(void)
{
    printf("Las consultas del Problema 3 todavía no están disponibles.\n");
}

void mostrar_datos_y_supuestos(void)
{
    printf("\n");
    printf("DATOS DEL ESCENARIO (Problema 3)\n");
    printf("  Masa de Orion por defecto ........... %.0f kg\n", MASA_DEFECTO);
    printf("  Velocidad de inyección por defecto .. %.0f m/s\n", VELOCIDAD_TLI_DEFECTO);
    printf("  Altitud de la órbita baja ........... %.0f m\n", ALTITUD_DEFECTO);
    printf("  Distancia media Tierra-Luna ......... %.0f m\n", DISTANCIA_TIERRA_LUNA);
    printf("  Constante de gravitación G .......... %.3e N·m²/kg²\n", CONSTANTE_G);
    printf("  Masa de la Tierra ................... %.3e kg\n", MASA_TIERRA);
    printf("  Radio medio de la Tierra ............ %.0f m\n", RADIO_TIERRA);
    printf("  Masa de la Luna ..................... %.3e kg\n", MASA_LUNA);
    printf("  Radio medio de la Luna .............. %.0f m\n", RADIO_LUNA);
    printf("\n");
    printf("SUPUESTOS DEL MODELO\n");
    printf("  - Una dimensión: la nave se mueve sobre la recta Tierra-Luna.\n");
    printf("  - Luna fija: se usa la distancia media Tierra-Luna.\n");
    printf("  - Sin propulsión: después de la TLI solo actúan las gravedades de la Tierra\n");
    printf("    y de la Luna, salvo las correcciones que aplica el jugador.\n");
    printf("  - Conservación de la energía: la velocidad en cada punto se obtiene de Em = K + U.\n");
}

void mostrar_ultimo_reporte(const ReporteFinal *reporte)
{
    if (reporte == NULL) {
        printf("Todavía no hay misiones jugadas\n");
        return;
    }
    printf("El reporte detallado todavía no está disponible.\n");
}
