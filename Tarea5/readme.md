# Integer Overflow Attack Simulation

## Descripción

Este proyecto simula un **Integer Overflow**, mostrando cómo un error en operaciones aritméticas puede provocar cálculos incorrectos de memoria y errores en la ejecución del programa.

---

## ¿Qué es un Integer Overflow?

Un **integer overflow** ocurre cuando el resultado de una operación (como suma o multiplicación) supera el valor máximo que puede almacenar un tipo de dato.

Cuando esto pasa:
- el valor resultante es incorrecto,
- el programa no lanza error, y continúa ejecutándose como si nada hubiera pasado.

---

## Implementación

El código incluye dos versiones: una vulnerable y una segura.

---

### Versión vulnerable

En esta versión, el tamaño de memoria se calcula directamente:

```
bytes_needed = width * height;
```

Si los valores son grandes, puede ocurrir overflow y el resultado será incorrecto.

Después se usa:

```c
malloc(bytes_needed);
```

`malloc` reserva exactamente la cantidad de memoria que recibe, aunque sea incorrecta.

---

### Versión segura

La versión segura valida la operación antes de usar el resultado.

#### Con GCC o Clang

```
__builtin_mul_overflow(width, height, &bytes_needed)
```

Esta función:
- realiza la multiplicación,
- guarda el resultado en `bytes_needed`,
- detecta si hubo overflow.

Si hay overflow, el programa se detiene.

---

#### Validación manual

```
if (height != 0 && width > SIZE_MAX / height)
```

Esta condición revisa si la multiplicación excedería el límite antes de realizarla.

---

#### Manejo de memoria

```
char *buffer = malloc(bytes_needed);
```

Reserva memoria dinámica del tamaño indicado.

```
if (buffer == NULL)
```

Verifica que la memoria fue asignada correctamente.

---

## Contramedidas

### A nivel de código / API

- **size_t**  
  Tipo de dato usado para tamaños de memoria. No permite valores negativos.

- **calloc()**  
  Función que reserva memoria y además la inicializa en cero. A diferencia de `malloc`, ayuda a evitar errores en cálculos de tamaño.

- **reallocarray()**  
  Similar a `malloc`, pero calcula el tamaño de forma segura evitando overflow en multiplicaciones.

- Validar operaciones antes de usarlas  
  Evita utilizar resultados incorrectos directamente.

---

### A nivel de compilador

- **__builtin_mul_overflow**  
  Permite detectar overflow al momento de realizar operaciones.

- **-fsanitize=undefined**  
  Detecta errores como overflow durante la ejecución del programa.

- **-ftrapv**  
  Hace que el programa se detenga si ocurre overflow con enteros con signo.

---

### A nivel de sistema operativo

- **ASLR (Address Space Layout Randomization)**  
  Cambia la ubicación de la memoria para dificultar la explotación de vulnerabilidades.

- **Protección de memoria**  
  Evita accesos indebidos a regiones de memoria.

---
## Resultados

Después de ejecutar el programa, en terminal se observa lo siguiente.

```
=== Version vulnerable ===

width = 500, height = 500
Tamano calculado (uint16_t) = 53392 bytes
Tamano real esperado        = 250000 bytes

malloc reservo 53392 bytes
Pero el programa realmente necesitaba 250000 bytes
Si ahora se escribieran 250000 bytes, habria riesgo de buffer overflow.

=== Version segura ===

width = 500, height = 500
Tamano calculado correctamente = 250000 bytes
malloc reservo el tamano correcto

```

## Conclusión

El integer overflow es un error que puede no detectarse, pero afecta directamente cálculos importantes del programa.

Si no se controla, puede provocar errores en memoria y comportamientos inesperados.  
Por eso es importante validar operaciones, usar tipos adecuados y aprovechar herramientas del compilador y del sistema.
