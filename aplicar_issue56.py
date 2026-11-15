#!/usr/bin/env python3
"""Aplica la implementacion del estado de la nave correspondiente al ISSUE #56.

Uso: ejecutar desde la raiz del repositorio artemis-tli y estando en la rama
feat/estado-nave. El script modifica unicamente src/simulador.c y crea antes
una copia de seguridad en respaldo-issue56/.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime
from pathlib import Path

RAMA_ESPERADA = "feat/estado-nave"

BLOQUE_PENDIENTE = r'''/* Estado de la nave */

double calcular_distancia_punto(int punto)
{
    (void)punto;
    /* Pendiente: se implementa en esta etapa. */
    return 0.0;
}

void inicializar_nave(EstadoNave *nave, double velocidad_inyeccion, double reserva)
{
    (void)nave;
    (void)velocidad_inyeccion;
    (void)reserva;
    /* Pendiente: se implementa en esta etapa. */
}

int aplicar_correccion(EstadoNave *nave, double trabajo)
{
    (void)nave;
    (void)trabajo;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
}

int avanzar_nave(EstadoNave *nave)
{
    (void)nave;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
}

int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[])
{
    (void)nave;
    (void)velocidades;
    /* Pendiente: se implementa en esta etapa. */
    return 0;
}
'''

BLOQUE_IMPLEMENTADO = r'''/* Estado de la nave */

/* Calcula la distancia correspondiente a un punto de la trayectoria. */
double calcular_distancia_punto(int punto)
{
    double distancia_inicial = RADIO_TIERRA + ALTITUD_DEFECTO;

    if (punto < 0 || punto > CANTIDAD_TRAMOS) {
        return 0.0;
    }

    return distancia_inicial
           + punto * (calcular_punto_equilibrio() - distancia_inicial) / CANTIDAD_TRAMOS;
}

/* Inicializa todos los datos de la nave en la órbita baja. */
void inicializar_nave(EstadoNave *nave, double velocidad_inyeccion, double reserva)
{
    if (nave == NULL) {
        return;
    }

    nave->punto = 0;
    nave->distancia = calcular_distancia_punto(0);
    nave->velocidad = velocidad_inyeccion;
    nave->reserva = reserva;
    nave->trabajo_aplicado = 0.0;
    nave->correcciones = 0;
}

/* Aplica trabajo adicional a la nave y descuenta ese trabajo de la reserva. */
int aplicar_correccion(EstadoNave *nave, double trabajo)
{
    double nueva_velocidad;

    if (nave == NULL || !isfinite(trabajo) || trabajo <= 0.0 ||
        trabajo > nave->reserva) {
        return 0;
    }

    nueva_velocidad = sqrt(nave->velocidad * nave->velocidad + 2.0 * trabajo);
    if (!isfinite(nueva_velocidad)) {
        return 0;
    }

    nave->velocidad = nueva_velocidad;
    nave->reserva -= trabajo;
    nave->trabajo_aplicado += trabajo;
    nave->correcciones++;
    return 1;
}

/* Intenta avanzar un tramo; solo actualiza el estado si puede llegar. */
int avanzar_nave(EstadoNave *nave)
{
    double velocidad_siguiente;
    int siguiente_punto;
    double siguiente_distancia;

    if (nave == NULL || nave->punto < 0 || nave->punto >= CANTIDAD_TRAMOS) {
        return 0;
    }

    siguiente_punto = nave->punto + 1;
    siguiente_distancia = calcular_distancia_punto(siguiente_punto);

    if (!calcular_velocidad_en(nave->distancia, nave->velocidad,
                               siguiente_distancia, &velocidad_siguiente)) {
        return 0;
    }

    nave->punto = siguiente_punto;
    nave->distancia = siguiente_distancia;
    nave->velocidad = velocidad_siguiente;
    return 1;
}

/* Recorre todos los puntos alcanzables sin correcciones y registra las velocidades. */
int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[])
{
    if (nave == NULL || velocidades == NULL || nave->punto < 0 ||
        nave->punto > CANTIDAD_TRAMOS) {
        return 0;
    }

    velocidades[nave->punto] = nave->velocidad;

    while (avanzar_nave(nave)) {
        velocidades[nave->punto] = nave->velocidad;
    }

    return nave->punto;
}
'''


def abortar(mensaje: str) -> "NoReturn":
    print(f"ERROR: {mensaje}", file=sys.stderr)
    raise SystemExit(1)


def obtener_raiz_y_validar() -> Path:
    raiz = Path.cwd()
    if not (raiz / ".git").exists():
        abortar("No se encontro .git. Ejecuta este script desde la raiz del repositorio artemis-tli.")

    requeridos = (
        raiz / "src" / "simulador.c",
        raiz / "src" / "simulador.h",
        raiz / "pruebas" / "pruebas_simulador.c",
    )
    faltantes = [str(ruta.relative_to(raiz)) for ruta in requeridos if not ruta.is_file()]
    if faltantes:
        abortar("Faltan archivos del proyecto: " + ", ".join(faltantes))

    try:
        rama = subprocess.run(
            ["git", "branch", "--show-current"],
            cwd=raiz,
            check=True,
            capture_output=True,
            text=True,
            encoding="utf-8",
        ).stdout.strip()
    except (OSError, subprocess.CalledProcessError) as error:
        abortar(f"No se pudo comprobar la rama de Git: {error}")

    if rama != RAMA_ESPERADA:
        abortar(
            f"La rama actual es '{rama or '(sin rama)'}'; se requiere '{RAMA_ESPERADA}'. "
            "No se modifico ningun archivo."
        )

    return raiz


def crear_respaldo(archivo: Path, directorio: Path) -> Path:
    directorio.mkdir(parents=True, exist_ok=True)
    marca_tiempo = datetime.now().strftime("%Y%m%d-%H%M%S")
    destino = directorio / f"simulador.c.antes-issue56-{marca_tiempo}.bak"
    contador = 1
    while destino.exists():
        destino = directorio / f"simulador.c.antes-issue56-{marca_tiempo}-{contador}.bak"
        contador += 1
    shutil.copy2(archivo, destino)
    return destino


def escribir_atomico(archivo: Path, contenido: bytes) -> None:
    temporal = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="wb", dir=archivo.parent, prefix=".simulador.c-issue56-", suffix=".tmp", delete=False
        ) as salida:
            temporal = Path(salida.name)
            salida.write(contenido)
            salida.flush()
            os.fsync(salida.fileno())
        os.replace(temporal, archivo)
    except OSError:
        if temporal is not None:
            try:
                temporal.unlink(missing_ok=True)
            except OSError:
                pass
        raise


def main() -> int:
    raiz = obtener_raiz_y_validar()
    archivo = raiz / "src" / "simulador.c"

    try:
        bytes_originales = archivo.read_bytes()
        tiene_bom = bytes_originales.startswith(b"\xef\xbb\xbf")
        texto = bytes_originales.decode("utf-8-sig" if tiene_bom else "utf-8")
    except (OSError, UnicodeDecodeError) as error:
        abortar(f"No se pudo leer src/simulador.c como UTF-8: {error}")

    # Se compara con saltos de linea normalizados, pero al guardar se conserva
    # el estilo original (LF o CRLF) y, si existia, el BOM UTF-8.
    salto = "\r\n" if "\r\n" in texto else "\n"
    texto_normalizado = texto.replace("\r\n", "\n")
    pendiente = BLOQUE_PENDIENTE
    implementado = BLOQUE_IMPLEMENTADO

    cantidad_pendiente = texto_normalizado.count(pendiente)
    cantidad_implementado = texto_normalizado.count(implementado)

    if cantidad_implementado == 1 and cantidad_pendiente == 0:
        print("El ISSUE #56 ya parece estar implementado. No se modifico ningun archivo.")
        return 0

    if cantidad_pendiente != 1 or cantidad_implementado != 0:
        abortar(
            "No se encontro exactamente una version pendiente del bloque 'Estado de la nave'. "
            "El archivo no fue modificado. Revisa src/simulador.c antes de continuar."
        )

    nuevo_texto = texto_normalizado.replace(pendiente, implementado, 1)
    if salto == "\r\n":
        nuevo_texto = nuevo_texto.replace("\n", "\r\n")
    bytes_nuevos = ("\ufeff" if tiene_bom else "")
    bytes_nuevos = (bytes_nuevos + nuevo_texto).encode("utf-8")

    directorio_respaldo = raiz / "respaldo-issue56"
    try:
        respaldo = crear_respaldo(archivo, directorio_respaldo)
        escribir_atomico(archivo, bytes_nuevos)
    except OSError as error:
        abortar(f"No se pudo aplicar el cambio: {error}")

    print("Implementacion del ISSUE #56 aplicada correctamente.")
    print("Archivo modificado: src/simulador.c")
    print(f"Copia de seguridad: {respaldo.relative_to(raiz)}")
    print("No se modificaron las pruebas ni el README.")
    print("Ahora ejecuta: mingw32-make && mingw32-make pruebas")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
