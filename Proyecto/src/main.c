#include <stdio.h>      // Entrada y salida estándar
#include <stdlib.h>     // Funciones generales del sistema
#include <string.h>     // Manejo de cadenas
#include <unistd.h>     // Funciones POSIX (sleep)
#include <time.h>       // Manejo de fecha y hora


// Códigos ANSI para colores en terminal
#define RESET "\033[0m"
#define VERDE "\033[32m"
#define AMARILLO "\033[33m"
#define ROJO "\033[31m"


// Estructura para almacenar estadísticas del CPU
typedef struct {

    long user;      // Tiempo en modo usuario
    long nice;      // Tiempo de procesos con prioridad modificada
    long system;    // Tiempo en modo kernel
    long idle;      // Tiempo inactivo
    long iowait;    // Tiempo esperando operaciones I/O
    long irq;       // Tiempo en interrupciones hardware
    long softirq;   // Tiempo en interrupciones software
    long steal;     // Tiempo robado por máquinas virtuales

} CPUStats;



// Función que lee estadísticas del CPU desde /proc/stat
CPUStats leer_cpu() {

    FILE *archivo;

    // Inicializa todos los campos en cero
    CPUStats cpu = {0};

    // Abre archivo virtual del sistema
    archivo = fopen("/proc/stat", "r");

    // Verifica si hubo error al abrir
    if (archivo == NULL) {

        printf("Error: no se pudo abrir /proc/stat\n");

        return cpu;
    }

    // Lee la primera línea del archivo:
    // cpu user nice system idle iowait irq softirq steal
    fscanf(archivo,
           "cpu %ld %ld %ld %ld %ld %ld %ld %ld",

           &cpu.user,
           &cpu.nice,
           &cpu.system,
           &cpu.idle,
           &cpu.iowait,
           &cpu.irq,
           &cpu.softirq,
           &cpu.steal);

    // Cierra el archivo
    fclose(archivo);

    // Retorna estructura con datos leídos
    return cpu;
}



// Función que calcula el porcentaje de uso de CPU
double calcular_uso_cpu(CPUStats anterior, CPUStats actual) {

    // Tiempo idle anterior y actual
    long idle_anterior = anterior.idle + anterior.iowait;

    long idle_actual = actual.idle + actual.iowait;


    // Tiempo total anterior
    long total_anterior =
        anterior.user +
        anterior.nice +
        anterior.system +
        anterior.idle +
        anterior.iowait +
        anterior.irq +
        anterior.softirq +
        anterior.steal;


    // Tiempo total actual
    long total_actual =
        actual.user +
        actual.nice +
        actual.system +
        actual.idle +
        actual.iowait +
        actual.irq +
        actual.softirq +
        actual.steal;


    // Diferencias entre lecturas
    long diferencia_total = total_actual - total_anterior;

    long diferencia_idle = idle_actual - idle_anterior;


    // Evita división entre cero
    if (diferencia_total == 0) {

        return 0.0;
    }


    // Fórmula de porcentaje de uso de CPU
    return
        ((double)(diferencia_total - diferencia_idle) / diferencia_total) * 100;
}




// Función que lee memoria RAM y SWAP
void leer_memoria(long *memoria_total, long *memoria_disponible, long *swap_total, long *swap_libre) {

    FILE *archivo;

    char etiqueta[50];
    long valor;
    char unidad[10];


    // Abre archivo virtual de memoria
    archivo = fopen("/proc/meminfo", "r");


    // Verifica si hubo error
    if (archivo == NULL) {

        printf("Error: no se pudo abrir /proc/meminfo\n");

        return;
    }


    // Lee línea por línea
    while (fscanf(archivo,
                  "%s %ld %s",
                  etiqueta,
                  &valor,
                  unidad) == 3) {

        // Memoria RAM total
        if (strcmp(etiqueta, "MemTotal:") == 0) {

            *memoria_total = valor;
        }

        // Memoria RAM disponible
        else if (strcmp(etiqueta, "MemAvailable:") == 0) {

            *memoria_disponible = valor;
        }

        // SWAP total
        else if (strcmp(etiqueta, "SwapTotal:") == 0) {

            *swap_total = valor;
        }

        // SWAP libre
        else if (strcmp(etiqueta, "SwapFree:") == 0) {

            *swap_libre = valor;
        }
    }

    // Cierra archivo
    fclose(archivo);
}




// Función que lee uptime del sistema
void leer_uptime(int *horas, int *minutos, int *segundos) {

    FILE *archivo;

    double uptime_segundos = 0;


    // Abre archivo virtual uptime
    archivo = fopen("/proc/uptime", "r");


    // Verifica errores
    if (archivo == NULL) {

        printf("Error: no se pudo abrir /proc/uptime\n");

        return;
    }


    // Lee uptime en segundos
    fscanf(archivo, "%lf", &uptime_segundos);

    fclose(archivo);


    // Conversión a horas, minutos y segundos
    int total = (int) uptime_segundos;

    *horas = total / 3600;

    *minutos = (total % 3600) / 60;

    *segundos = total % 60;
}




// Función que lee load average
void leer_load_average(double *load1, double *load5, double *load15) {

    FILE *archivo;


    // Abre archivo loadavg
    archivo = fopen("/proc/loadavg", "r");


    // Verifica errores
    if (archivo == NULL) {

        printf("Error: no se pudo abrir /proc/loadavg\n");

        return;
    }


    // Lee cargas promedio
    fscanf(archivo,
           "%lf %lf %lf",

           load1,
           load5,
           load15);

    fclose(archivo);
}




// Función que obtiene fecha y hora actual
void obtener_fecha_hora(char *buffer, int tamano) {

    time_t tiempo_actual;

    struct tm *info_tiempo;


    // Obtiene tiempo actual
    time(&tiempo_actual);


    // Convierte a estructura local
    info_tiempo = localtime(&tiempo_actual);


    // Formatea fecha y hora
    strftime(buffer,
             tamano,
             "%d/%m/%Y %H:%M:%S",
             info_tiempo);
}




// Función que determina color según porcentaje
const char* obtener_color(double porcentaje) {

    // Verde: uso bajo
    if (porcentaje < 50) {

        return VERDE;
    }

    // Amarillo: uso medio
    else if (porcentaje < 80) {

        return AMARILLO;
    }

    // Rojo: uso alto
    else {

        return ROJO;
    }
}




// Función que imprime barra visual
void imprimir_barra(const char *nombre, double porcentaje) {

    // Tamaño de barra
    int ancho_barra = 30;


    // Cantidad de bloques llenos
    int bloques_llenos = (int)((porcentaje / 100) * ancho_barra);


    // Obtiene color correspondiente
    const char *color = obtener_color(porcentaje);


    // Imprime nombre y color
    printf("%s%-6s [" RESET, color, nombre);


    // Dibuja barra
    for (int i = 0; i < ancho_barra; i++) {

        // Parte llena
        if (i < bloques_llenos) {

            printf("█");
        }

        // Parte vacía
        else {

            printf("░");
        }
    }


    // Imprime porcentaje
    printf("] %.2f %%\n", porcentaje);
}




// Función principal
int main() {

    // Primera lectura de CPU
    CPUStats cpu_anterior = leer_cpu();


    // Ciclo infinito de monitoreo
    while (1) {

        // Variables RAM
        long memoria_total = 0;
        long memoria_disponible = 0;
        long memoria_usada = 0;


        // Variables SWAP
        long swap_total = 0;
        long swap_libre = 0;
        long swap_usada = 0;


        // Variables porcentajes
        double porcentaje_memoria = 0.0;
        double porcentaje_swap = 0.0;


        // Variables uptime
        int horas = 0;
        int minutos = 0;
        int segundos = 0;


        // Variables load average
        double load1 = 0.0;
        double load5 = 0.0;
        double load15 = 0.0;


        // Buffer fecha y hora
        char fecha_hora[50];


        // Espera 1 segundo
        sleep(1);


        // Nueva lectura CPU
        CPUStats cpu_actual = leer_cpu();


        // Calcula uso CPU
        double porcentaje_cpu = calcular_uso_cpu(cpu_anterior, cpu_actual);


        // Actualiza lectura anterior
        cpu_anterior = cpu_actual;


        // Lee memoria RAM y SWAP
        leer_memoria(&memoria_total,
                     &memoria_disponible,
                     &swap_total,
                     &swap_libre);


        // Lee uptime
        leer_uptime(&horas,
                    &minutos,
                    &segundos);


        // Lee load average
        leer_load_average(&load1,
                          &load5,
                          &load15);


        // Obtiene fecha y hora
        obtener_fecha_hora(fecha_hora, sizeof(fecha_hora));


        // Calcula RAM usada
        memoria_usada = memoria_total - memoria_disponible;


        // Calcula SWAP usada
        swap_usada = swap_total - swap_libre;


        // Calcula porcentaje RAM
        if (memoria_total != 0) {

            porcentaje_memoria = ((double) memoria_usada / memoria_total) * 100;
        }


        // Calcula porcentaje SWAP
        if (swap_total != 0) {

            porcentaje_swap = ((double) swap_usada / swap_total) * 100;
        }


        // Limpia pantalla
        system("clear");


        // Encabezado
        printf("========================================\n");

        printf("        MONITOR DEL SISTEMA EN C        \n");

        printf("========================================\n");


        // Fecha y hora
        printf("Última actualización: %s\n\n", fecha_hora);


        // Barras CPU, RAM y SWAP
        imprimir_barra("CPU", porcentaje_cpu);

        imprimir_barra("RAM", porcentaje_memoria);

        imprimir_barra("SWAP", porcentaje_swap);


        // Información detallada
        printf("\n----------------------------------------\n");


        // RAM
        printf("Memoria total:       %ld kB\n", memoria_total);

        printf("Memoria disponible:  %ld kB\n", memoria_disponible);

        printf("Memoria usada:       %ld kB\n", memoria_usada);


        // SWAP
        printf("\nSwap total:          %ld kB\n", swap_total);

        printf("Swap libre:          %ld kB\n", swap_libre);

        printf("Swap usada:          %ld kB\n", swap_usada);


        // Uptime
        printf("\nUptime:              %02d:%02d:%02d\n", horas, minutos, segundos);


        // Load average
        printf("Load average:        %.2f %.2f %.2f\n", load1, load5, load15);


        // Mensajes finales
        printf("\nActualizando cada segundo...\n");

        printf("Presiona Ctrl + C para salir.\n");
    }

    return 0;
}