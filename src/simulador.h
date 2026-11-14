#ifndef SIMULADOR_H
#define SIMULADOR_H

/* Constantes físicas: consigna, Problemas 2 y 3; datos de la Luna: NASA Moon Fact Sheet */
#define CONSTANTE_G 6.674e-11              /* N·m²/kg² */
#define MASA_TIERRA 5.972e24               /* kg */
#define RADIO_TIERRA 6371000.0             /* m */
#define MASA_LUNA 7.346e22                 /* kg */
#define RADIO_LUNA 1737400.0               /* m */
#define DISTANCIA_TIERRA_LUNA 384000000.0  /* m */

/* Valores por defecto del escenario */
#define MASA_DEFECTO 26000.0               /* kg */
#define VELOCIDAD_TLI_DEFECTO 10900.0      /* m/s */
#define ALTITUD_DEFECTO 192000.0           /* m */

/* Rangos válidos de las entradas */
#define MASA_MINIMA 1000.0
#define MASA_MAXIMA 100000.0
#define VELOCIDAD_TLI_MINIMA 10500.0
#define VELOCIDAD_TLI_MAXIMA 11200.0
#define ALTITUD_MINIMA 160000.0
#define ALTITUD_MAXIMA 2000000.0

/* Reglas del juego */
#define CANTIDAD_TRAMOS 10
#define RESERVA_CORRECCION_DEFECTO 150000.0  /* J/kg */
#define PUNTAJE_MAXIMO 100.0
#define DESCUENTO_POR_KJ_EXCESO 0.05
#define DESCUENTO_POR_CORRECCION 5.0
#define FUGA_MINIMA 10                     /* % de reserva que se pierde */
#define FUGA_MAXIMA 25

/* Menú principal */
#define OPCION_SALIR 0
#define OPCION_MENU_MAXIMA 4

typedef enum {
    RESULTADO_EN_CURSO,
    RESULTADO_EXITO,
    RESULTADO_FRACASO,
    RESULTADO_ABORTADA
} ResultadoMision;

typedef enum {
    EVENTO_NINGUNO,
    EVENTO_FUGA_PROPULSION,
    EVENTO_PERDIDA_SENAL,
    EVENTO_FALLA_SENSOR
} TipoEvento;

/* Estado de la nave durante el vuelo */
typedef struct {
    int punto;                 /* 0 (órbita baja) a CANTIDAD_TRAMOS (equilibrio) */
    double distancia;          /* m, desde el centro de la Tierra */
    double velocidad;          /* m/s */
    double reserva;            /* J/kg de corrección disponible */
    double trabajo_aplicado;   /* J/kg sumado en correcciones */
    int correcciones;          /* correcciones aplicadas */
} EstadoNave;

/* Datos de la misión elegidos en la inyección */
typedef struct {
    double masa;                 /* kg */
    double velocidad_inyeccion;  /* m/s */
    int indice_nivel;
    ResultadoMision resultado;
} ParametrosMision;

/* Datos que se muestran en cada turno */
typedef struct {
    double distancia;            /* m */
    double velocidad;            /* m/s */
    double atraccion_tierra;     /* m/s² */
    double atraccion_luna;       /* m/s² */
    double desaceleracion_neta;  /* m/s² */
    double energia_cinetica;     /* J/kg */
} Telemetria;

typedef struct {
    TipoEvento tipo;
    double porcentaje_reserva;  /* reserva que se pierde, de 0 a 100 */
} Evento;

/* Resultado de una misión terminada */
typedef struct {
    double masa;                      /* kg */
    double velocidad_inyeccion;       /* m/s */
    double energia_cinetica_inicial;  /* J */
    ResultadoMision resultado;
    double distancia_maxima;          /* m */
    double velocidad_final;           /* m/s */
    double trabajo_aplicado;          /* J/kg */
    double trabajo_minimo;            /* J/kg */
    double pozo_tierra;               /* J/kg */
    double pozo_luna;                 /* J/kg */
    int correcciones_usadas;
    double puntaje;                   /* 0 a 100 */
} ReporteFinal;

/* Opciones del menú principal */
int nueva_mision(ReporteFinal *reporte);
void submenu_consultas(void);
void mostrar_datos_y_supuestos(void);
void mostrar_ultimo_reporte(const ReporteFinal *reporte);

/* Cálculos físicos. Una distancia fuera de (0, DISTANCIA_TIERRA_LUNA) devuelve 0. */
double calcular_atraccion_tierra(double distancia);
double calcular_atraccion_luna(double distancia);
double calcular_energia_cinetica(double masa, double velocidad);
double calcular_energia_potencial_por_kg(double distancia);
double calcular_punto_equilibrio(void);
double calcular_trabajo_minimo(double velocidad, double altitud);
double calcular_pozo_tierra(double altitud);
double calcular_pozo_luna(void);
int calcular_velocidad_en(double distancia_inicial, double velocidad_inicial,
                          double distancia, double *velocidad);
double calcular_distancia_maxima(double distancia_inicial, double velocidad_inicial);

/* Estado de la nave */
double calcular_distancia_punto(int punto);
void inicializar_nave(EstadoNave *nave, double velocidad_inyeccion, double reserva);
void calcular_telemetria(const EstadoNave *nave, Telemetria *telemetria);
int aplicar_correccion(EstadoNave *nave, double trabajo);
int avanzar_nave(EstadoNave *nave);
int recorrer_sin_correcciones(EstadoNave *nave, double velocidades[]);

/* Eventos y puntaje */
void generar_evento(Evento *evento, int probabilidad, int nave_alcanza_siguiente);
void aplicar_evento(EstadoNave *nave, const Evento *evento);
double calcular_puntaje(ResultadoMision resultado, double velocidad_inyeccion,
                        double trabajo_aplicado, int correcciones);

#endif
