#include <stdio.h>      // Funciones de entrada y salida (printf, fopen, fscanf)
#include <stdlib.h>     // Funciones generales del sistema (system)
#include <string.h>     // Manejo de cadenas (strcmp)
#include <unistd.h>     // Funciones POSIX (sleep)

// Estructura para almacenar los tiempos del CPU obtenidos desde /proc/stat
typedef struct {
    long user;      // Tiempo en modo usuario
    long nice;      // Tiempo de procesos con prioridad modificada
    long system;    // Tiempo en modo kernel
    long idle;      // Tiempo inactivo
    long iowait;    // Tiempo esperando operaciones de I/O
    long irq;       // Tiempo atendiendo interrupciones hardware
    long softirq;   // Tiempo atendiendo interrupciones software
    long steal;     // Tiempo "robado" por otras máquinas virtuales
} CPUStats;


// Función que lee los datos actuales del CPU desde /proc/stat
CPUStats leer_cpu() {

    FILE *archivo;

    // Inicializa todos los campos en 0
    CPUStats cpu = {0};

    // Abre el archivo del sistema en modo lectura
    archivo = fopen("/proc/stat", "r");

    // Verifica si hubo error al abrir el archivo
    if (archivo == NULL) {
        printf("Error: no se pudo abrir /proc/stat\n");
        return cpu;
    }

    // Lee la primera línea del archivo:
    // cpu user nice system idle iowait irq softirq steal
    fscanf(archivo, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
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

    // Retorna la estructura con los valores leídos
    return cpu;
}


// Función que calcula el porcentaje de uso de CPU
double calcular_uso_cpu(CPUStats anterior, CPUStats actual) {

    // Tiempo idle total anterior y actual
    long idle_anterior = anterior.idle + anterior.iowait;
    long idle_actual = actual.idle + actual.iowait;

    // Tiempo total anterior
    long total_anterior = anterior.user + anterior.nice + anterior.system +
                          anterior.idle + anterior.iowait + anterior.irq +
                          anterior.softirq + anterior.steal;

    // Tiempo total actual
    long total_actual = actual.user + actual.nice + actual.system +
                        actual.idle + actual.iowait + actual.irq +
                        actual.softirq + actual.steal;

    // Diferencia de tiempo total entre ambas lecturas
    long diferencia_total = total_actual - total_anterior;

    // Diferencia de tiempo inactivo entre ambas lecturas
    long diferencia_idle = idle_actual - idle_anterior;

    // Evita división entre cero
    if (diferencia_total == 0) {
        return 0.0;
    }

    // Fórmula para calcular porcentaje de uso de CPU
    return ((double)(diferencia_total - diferencia_idle) / diferencia_total) * 100;
}


// Función que lee información de memoria RAM desde /proc/meminfo
void leer_memoria(long *memoria_total, long *memoria_disponible) {

    FILE *archivo;

    char etiqueta[50];
    long valor;
    char unidad[10];

    // Abre el archivo del sistema
    archivo = fopen("/proc/meminfo", "r");

    // Verifica si pudo abrirse correctamente
    if (archivo == NULL) {
        printf("Error: no se pudo abrir /proc/meminfo\n");
        return;
    }

    // Lee línea por línea:
    // Ejemplo:
    // MemTotal: 16343456 kB
    while (fscanf(archivo, "%s %ld %s", etiqueta, &valor, unidad) == 3) {

        // Si encuentra MemTotal guarda el valor
        if (strcmp(etiqueta, "MemTotal:") == 0) {
            *memoria_total = valor;
        }

        // Si encuentra MemAvailable guarda el valor
        if (strcmp(etiqueta, "MemAvailable:") == 0) {
            *memoria_disponible = valor;
        }
    }

    // Cierra el archivo
    fclose(archivo);
}


// Función principal
int main() {

    // Primera lectura de CPU
    // Se usa como referencia inicial
    CPUStats cpu_anterior = leer_cpu();

    // Ciclo infinito para monitoreo en tiempo real
    while (1) {

        // Variables para memoria RAM
        long memoria_total = 0;
        long memoria_disponible = 0;
        long memoria_usada = 0;

        // Variables para porcentajes
        double porcentaje_memoria = 0.0;

        // Espera 1 segundo antes de la siguiente lectura
        sleep(1);

        // Segunda lectura de CPU
        CPUStats cpu_actual = leer_cpu();

        // Calcula el porcentaje de uso de CPU
        double porcentaje_cpu = calcular_uso_cpu(cpu_anterior, cpu_actual);

        // Actualiza referencia anterior
        cpu_anterior = cpu_actual;

        // Lee información de memoria RAM
        leer_memoria(&memoria_total, &memoria_disponible);

        // Calcula memoria usada
        memoria_usada = memoria_total - memoria_disponible;

        // Calcula porcentaje de uso de memoria
        if (memoria_total != 0) {
            porcentaje_memoria =
                ((double) memoria_usada / memoria_total) * 100;
        }

        // Limpia la terminal para refrescar pantalla
        system("clear");

        // Encabezado
        printf("===== MONITOR DEL SISTEMA =====\n\n");

        // Muestra porcentajes
        printf("Uso de CPU:          %.2f %%\n", porcentaje_cpu);
        printf("Uso de memoria RAM:  %.2f %%\n\n", porcentaje_memoria);

        // Muestra información detallada de memoria
        printf("Memoria total:       %ld kB\n", memoria_total);
        printf("Memoria disponible:  %ld kB\n", memoria_disponible);
        printf("Memoria usada:       %ld kB\n", memoria_usada);

        // Mensajes informativos
        printf("\nActualizando cada segundo...\n");
        printf("Presiona Ctrl + C para salir.\n");
    }

    return 0;
}