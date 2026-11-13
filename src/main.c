#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "simulador.h"
#include "validaciones.h"

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    printf("Simulador Artemis TLI: inyección translunar de Orion\n");
    return 0;
}
