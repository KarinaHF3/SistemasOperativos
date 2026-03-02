# Simulador de Planificación de Procesos  
Sistemas Operativos – Tarea 3  
Equipo 10
Karina Hernández FLores
317346404
---

## Descripción General

Este proyecto consiste en la implementación de un simulador de planificación de procesos en lenguaje C.

Se implementaron los siguientes algoritmos:

- First-Come, First-Served (FCFS)
- Round Robin (RR)

El simulador:

- Lee una lista de procesos desde la entrada estándar.
- Genera la línea de tiempo de ejecución.
- Calcula métricas individuales y promedios.
- Permite comparar el desempeño entre ambos algoritmos.

---


## Compilación y Ejecución

### Compilar

```bash
gcc -O2 -Wall simuladorPlanProcesos.c -o simuladorPlanProcesos
```

### Ejecutar con quantum por defecto (4)

```bash
./simuladorPlanProcesos < Procesos.txt
```

### Ejecutar indicando quantum

```bash
./simuladorPlanProcesos 4 < Procesos.txt
```

---

## Algoritmos Implementados

### 1. First-Come, First-Served (FCFS)

Algoritmo no apropiativo que ejecuta los procesos en el orden en que llegan al sistema.

Una vez que un proceso obtiene la CPU, la conserva hasta terminar su ráfaga.

#### Ventajas
- Implementación sencilla.
- Fácil de entender.

#### Desventajas
- Procesos cortos pueden esperar demasiado si llegan después de uno largo.

---

### 2. Round Robin (RR)

Algoritmo apropiativo que asigna a cada proceso un intervalo fijo llamado quantum.

Si el proceso no termina en ese intervalo:
- Se interrumpe.
- Se coloca al final de la cola.

#### Ventajas
- Distribución equitativa del CPU.
- Mejor tiempo de respuesta.
- Ideal para sistemas interactivos.

#### Desventajas
- Quantum pequeño → muchos cambios de contexto.
- Quantum grande → comportamiento similar a FCFS.

---

## Métricas Calculadas

Para cada proceso se calculan:

### Turnaround Time
```
finish_time - arrival
```

Tiempo total que el proceso permanece en el sistema.

### Waiting Time
```
turnaround - burst
```

Tiempo total que el proceso espera en la cola.

### Response Time
```
start_time - arrival
```

Tiempo que tarda el proceso en recibir CPU por primera vez.

También se calculan los promedios para cada algoritmo.

---

## Casos de Prueba Utilizados

### Archivo con 100 procesos

Se utilizó el archivo `Procesos.txt` proporcionado en la actividad para evaluar el desempeño y se obtuvieron los siguientes resultados.
---

#### FCFS (First-Come, First-Served)
Promedios:
  Waiting (espera)     : 194.00
  Turnaround (retorno) : 200.00
  Response (respuesta) : 194.00

---
#### RR (Round Robin), quantum=4
Promedios:
  Waiting (espera)     : 240.64
  Turnaround (retorno) : 246.64
  Response (respuesta) : 122.82
 
---
#### RR (Round Robin), quantum=3
Promedios:
  Waiting (espera)     : 263.60
  Turnaround (retorno) : 269.60
  Response (respuesta) : 98.68

---
#### RR (Round Robin), quantum=5
Promedios:
  Waiting (espera)     : 246.65
  Turnaround (retorno) : 252.65
  Response (respuesta) : 141.31
---

## Resultados Comparativos

### Promedios Obtenidos

| Algoritmo  | Quantum  | Waiting  | Turnaround  | Response  |
|------------|----------|----------|-------------|-----------|
| FCFS       | —        | 194.00   | 200.00      | 194.00    |
| RR         | 3        | 263.60   | 269.60      | 98.68     |
| RR         | 4        | 240.64   | 246.64      | 122.82    |
| RR         | 5        | 246.65   | 252.65      | 141.31    |

---

## Análisis de Resultados

### Tiempo de Respuesta

Round Robin mejora significativamente el tiempo de respuesta en comparación con FCFS:

- FCFS: **194.00**
- RR (q=3): **98.68**
- RR (q=4): **122.82**
- RR (q=5): **141.31**

El menor tiempo de respuesta se obtuvo con **quantum = 3**, lo que confirma que un quantum pequeño permite que los procesos reciban CPU más rápidamente por primera vez.

---

### Tiempo de Espera

En este conjunto de datos, FCFS presentó el menor tiempo de espera promedio:

- FCFS: **194.00**
- RR (q=4): 240.64
- RR (q=5): 246.65
- RR (q=3): 263.60

Entonces, para este caso específico, FCFS resultó más eficiente en términos de espera total acumulada.

Sin embargo, aunque FCFS tiene menor waiting promedio, no distribuye el CPU de forma equitativa.

---

### Turnaround

El comportamiento es similar al waiting:

- FCFS: **200.00** (menor valor)
- RR (q=4): 246.64
- RR (q=5): 252.65
- RR (q=3): 269.60

FCFS mostró mejor turnaround promedio en esta simulación.

---

## Impacto del Quantum en Round Robin

Se observa que:

- Quantum más pequeño (3):
  - Mejor tiempo de respuesta
  - Peor waiting y turnaround
  - Mayor número de cambios de contexto

- Quantum más grande (5):
  - Aumenta el tiempo de respuesta
  - Mejora ligeramente waiting respecto a q=3
  - Se aproxima al comportamiento de FCFS

Entonces, un quantum pequeño mejora interactividad pero incrementa overhead y tiempos globales.

---
## Análisis

### ¿Qué algoritmo favorece procesos cortos?

Round Robin, ya que permite que todos los procesos reciban CPU rápidamente.

En FCFS, un proceso corto puede quedar esperando detrás de uno largo.

### ¿Cuál es más justo?

- FCFS es justo en orden de llegada.
- RR es justo en distribución de CPU.

En sistemas actuales, se puede considerar que Round Robin más equitativo.
---
## Conclusión

Aunque FCFS mostró mejores reultados promedio de espera y retorno en este conjunto de datos, Round Robin ofrece una distribución más equitativa del CPU y reduce significativamente el tiempo de respuesta.

Por lo tanto:

- Para sistemas batch, FCFS puede ser suficiente.
- Para sistemas interactivos, Round Robin es más adecuado.

El quantum más óptimo depende del equilibrio deseado entre eficiencia y rapidez de respuesta.

---

