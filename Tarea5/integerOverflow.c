#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

void vulnerable(void) {
    uint16_t width = 500;
    uint16_t height = 500;

    /* Cálculo vulnerable: 500 * 500 = 250000, pero uint16_t
       solo puede representar hasta 65535. */
    uint16_t bytes_needed = width * height;

    /* Cálculo correcto, solo para comparar */
    uint32_t real_bytes = (uint32_t)width * (uint32_t)height;

    printf("=== Version vulnerable ===\n\n");
    printf("width = %u, height = %u\n", width, height);
    printf("Tamano calculado (uint16_t) = %u bytes\n", bytes_needed);
    printf("Tamano real esperado        = %u bytes\n\n", real_bytes);

    char *buffer = (char *)malloc(bytes_needed);
    if (buffer == NULL) {
        printf("malloc fallo\n");
        return;
    }

    printf("malloc reservo %u bytes\n", bytes_needed);
    printf("Pero el programa realmente necesitaba %u bytes\n", real_bytes);
    printf("Si ahora se escribieran %u bytes, habria riesgo de buffer overflow.\n\n", real_bytes);

    free(buffer);
}

void safe(void) {
    size_t width = 500;
    size_t height = 500;
    size_t bytes_needed;

    printf("\n=== Version segura ===\n\n");
    printf("width = %zu, height = %zu\n", width, height);

#if defined(__GNUC__) || defined(__clang__)
    if (__builtin_mul_overflow(width, height, &bytes_needed)) {
        printf("Error: overflow detectado al calcular width * height\n");
        return;
    }
#else
    if (height != 0 && width > SIZE_MAX / height) {
        printf("Error: overflow detectado al calcular width * height\n");
        return;
    }
    bytes_needed = width * height;
#endif

    char *buffer = (char *)malloc(bytes_needed);
    if (buffer == NULL) {
        printf("malloc fallo\n");
        return;
    }

    printf("Tamano calculado correctamente = %zu bytes\n", bytes_needed);
    printf("malloc reservo el tamano correcto\n\n");

    free(buffer);
}

int main(void) {
    vulnerable();
    safe();
    return 0;
}