# Monitor Básico del Sistema en C

## Datos

Karina Hernández Flores

Número de cuenta: 317346404

Semestre 2026-2


## Descripción

Este proyecto consiste en el desarrollo de un monitor básico del sistema operativo en lenguaje C para Linux. Actualmente, el programa permite monitorear el uso de memoria RAM en tiempo real utilizando información obtenida directamente desde el sistema de archivos virtual '/proc'.

La aplicación actualiza automáticamente la información cada segundo y muestra en terminal:

* Memoria total
* Memoria disponible
* Memoria usada
* Porcentaje de uso de memoria RAM

---

## Objetivo

Implementar una herramienta sencilla de monitoreo del sistema que permita aplicar conceptos vistos en la materia de Sistemas Operativos, como:

* administración de memoria,
* acceso a recursos del sistema,
* monitoreo de recursos,
* lectura de archivos del sistema ('/proc'),

---

## Estructura del proyecto

```text
Proyecto/
├── src/
│   └── main.c
└── README.md
```

---

## Compilación

Desde la carpeta raíz del proyecto ejecutar:

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

---

## Funcionalidades implementadas

* Lectura de memoria RAM desde '/proc/meminfo'
* Cálculo de memoria usada
* Cálculo del porcentaje de uso de memoria
* Actualización automática cada segundo
* Limpieza dinámica de terminal

---