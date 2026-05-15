#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    while (1) {
        FILE *archivo;
        char etiqueta[50];
        long valor;
        char unidad[10];

        long memoria_total = 0;
        long memoria_disponible = 0;
        long memoria_usada = 0;
        double porcentaje_usado = 0.0;

        archivo = fopen("/proc/meminfo", "r");

        if (archivo == NULL) {
            printf("Error: no se pudo abrir /proc/meminfo\n");
            return 1;
        }

        while (fscanf(archivo, "%s %ld %s", etiqueta, &valor, unidad) == 3) {
            if (strcmp(etiqueta, "MemTotal:") == 0) {
                memoria_total = valor;
            }

            if (strcmp(etiqueta, "MemAvailable:") == 0) {
                memoria_disponible = valor;
            }
        }

        fclose(archivo);

        memoria_usada = memoria_total - memoria_disponible;
        porcentaje_usado = ((double) memoria_usada / memoria_total) * 100;

        system("clear");

        printf("===== MONITOR DEL SISTEMA =====\n\n");
        printf("Memoria total:       %ld kB\n", memoria_total);
        printf("Memoria disponible:  %ld kB\n", memoria_disponible);
        printf("Memoria usada:       %ld kB\n", memoria_usada);
        printf("Uso de memoria:      %.2f %%\n", porcentaje_usado);

        printf("\nActualizando cada segundo...\n");
        printf("Presiona Ctrl + C para salir.\n");

        sleep(1);
    }

    return 0;
}