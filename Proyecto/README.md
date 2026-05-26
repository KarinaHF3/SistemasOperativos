# Monitor Básico del Sistema en C

## Datos

**Karina Hernández Flores**  

Número de cuenta: 317346404  

Semestre 2026-2

---

## Descripción

Este proyecto consiste en el desarrollo de un monitor básico del sistema operativo en lenguaje C para Linux. El programa permite monitorear en tiempo real distintos recursos del sistema utilizando información obtenida directamente desde el sistema de archivos virtual `/proc`.

La aplicación actualiza automáticamente la información cada segundo y muestra métricas dinámicas relacionadas con el procesador y la memoria, implementando además una interfaz visual básica en terminal mediante barras ASCII y colores ANSI.

---

## Objetivo

Implementar una herramienta de monitoreo del sistema que permita aplicar conceptos vistos en la materia de Sistemas Operativos, como:

- administración de memoria,
- monitoreo de recursos,
- lectura de archivos virtuales del sistema (`/proc`),
- procesamiento dinámico de información,
- uso de llamadas y funciones del sistema,
- monitoreo del procesador y memoria en tiempo real.

---

## Estructura del proyecto

```text
Proyecto/
├── src/
│   └── main.c
├── README.md
├── Makefile
├── Proyecto_SO_HFK.pdf
└── .gitignore
```

---

## Compilación

Desde la carpeta raíz del proyecto ejecutar:

```bash
make
```

También es posible compilar manualmente con:

```bash
gcc src/main.c -o monitor
```

---

## Ejecución

```bash
./monitor
```

Para salir del programa:

```text
Ctrl + C
```

Eliminar ejecutable:

```bash
make clean
```

---

## Funcionalidades implementadas

- Monitoreo dinámico de uso de CPU
- Monitoreo dinámico de memoria RAM
- Monitoreo de memoria SWAP
- Visualización de uptime del sistema
- Visualización de load average
- Cálculo de porcentajes de uso
- Actualización automática cada segundo
- Barras visuales ASCII
- Colores ANSI dinámicos según nivel de uso
- Fecha y hora de actualización

---

## Archivos del sistema utilizados

| Archivo | Función |
|---|---|
| `/proc/stat` | Obtención de estadísticas del CPU |
| `/proc/meminfo` | Información de memoria RAM y SWAP |
| `/proc/uptime` | Tiempo activo del sistema |
| `/proc/loadavg` | Carga promedio del sistema |

---

## Funciones y llamadas utilizadas

Durante el desarrollo se utilizaron diversas funciones relacionadas con acceso a recursos del sistema:

| Función | Propósito |
|---|---|
| `fopen()` | Apertura de archivos virtuales |
| `fscanf()` | Lectura de información del sistema |
| `fclose()` | Cierre de archivos |
| `sleep()` | Actualización periódica |
| `system("clear")` | Limpieza de terminal |
| `strftime()` | Formato de fecha y hora |

---

## Interfaz del monitor

El sistema implementa una interfaz básica en terminal inspirada en herramientas de monitoreo como `htop`, mostrando barras visuales para CPU, RAM y SWAP.

### Ejemplo de salida

```text
        MONITOR DEL SISTEMA EN C        
========================================
Última actualización: 26/05/2026 13:54:52

CPU    [░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░] 2.34 %
RAM    [██████████░░░░░░░░░░░░░░░░░░░░] 34.52 %
SWAP   [░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░] 0.88 %

----------------------------------------
Memoria total:       16185608 kB
Memoria disponible:  10599012 kB
Memoria usada:       5586596 kB

Swap total:          7813116 kB
Swap libre:          7744236 kB
Swap usada:          68880 kB

Uptime:              02:30:18
Load average:        0.62 0.70 0.76

Actualizando cada segundo...
Presiona Ctrl + C para salir.
^C
```

---
