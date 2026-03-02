#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESOS 200   // por si quieren simular mas de 100
#define MAX_SEGMENTS 20000 // para la linea de tiempo (Gantt)

typedef struct {
    int id;                 // ID del proceso
    int arrival;            // Tiempo de llegada
    int burst;              // Ráfaga de CPU (tiempo total CPU)
    int priority;           // Prioridad (no usada en FCFS/RR, pero viene en el archivo)

    int remaining;          // CPU restante (RR)
    int start_time;         // Primer instante en que se ejecuta
    int finish_time;        // Momento en que termina

    int waiting_time;       // Tiempo total de espera
    int turnaround;         // finish_time - arrival
    int response_time;      // start_time - arrival
} Process;

typedef struct {
    int pid;        // id del proceso (o -1 si IDLE)
    int start;      // inicio del segmento
    int end;        // fin del segmento (exclusivo)
} Segment;

// Cola circular de indices para RR
typedef struct {
    int data[MAX_PROCESOS];
    int front, back, size;
} Queue;

static void q_init(Queue *q){ q->front = q->back = q->size = 0; }
static int  q_empty(Queue *q){ return q->size == 0; }
static void q_push(Queue *q, int x){
    q->data[q->back] = x;
    q->back = (q->back + 1) % MAX_PROCESOS;
    q->size++;
}
static int q_pop(Queue *q){
    int x = q->data[q->front];
    q->front = (q->front + 1) % MAX_PROCESOS;
    q->size--;
    return x;
}

static int cmp_proc(const void *A, const void *B){
    const Process *a = (const Process*)A;
    const Process *b = (const Process*)B;
    if (a->arrival != b->arrival) return a->arrival - b->arrival;
    return a->id - b->id;
}

static void reset_runtime(Process p[], int n){
    for(int i=0;i<n;i++){
        p[i].remaining = p[i].burst;
        p[i].start_time = -1;
        p[i].finish_time = -1;
        p[i].waiting_time = 0;
        p[i].turnaround = 0;
        p[i].response_time = 0;
    }
}

static void finalize_metrics(Process p[], int n){
    for(int i=0;i<n;i++){
        p[i].turnaround = p[i].finish_time - p[i].arrival;
        p[i].waiting_time = p[i].turnaround - p[i].burst;
        p[i].response_time = p[i].start_time - p[i].arrival;
    }
}

static void print_timeline(Segment segs[], int segCount){
    printf("\nOrden de ejecucion (linea de tiempo):\n");
    printf("  Segmento: [inicio, fin)\n");
    for(int i=0;i<segCount;i++){
        if (segs[i].pid == -1) printf("  IDLE: [%d, %d)\n", segs[i].start, segs[i].end);
        else                  printf("  P%d:   [%d, %d)\n", segs[i].pid, segs[i].start, segs[i].end);
    }
}

static void print_metrics(Process p[], int n){
    double sumW=0, sumT=0, sumR=0;

    // para imprimir en orden por ID, hacemos una copia y ordenamos
    Process tmp[MAX_PROCESOS];
    for(int i=0;i<n;i++) tmp[i] = p[i];
    // ordenar por id
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-1-i;j++){
            if (tmp[j].id > tmp[j+1].id){
                Process t = tmp[j]; tmp[j]=tmp[j+1]; tmp[j+1]=t;
            }
        }
    }

    printf("\nMétricas por proceso:\n");
    printf("ID\tarr\tburst\tstart\tfinish\twait\tturnaround\tresponse\n");
    for(int i=0;i<n;i++){
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n",
               tmp[i].id, tmp[i].arrival, tmp[i].burst,
               tmp[i].start_time, tmp[i].finish_time,
               tmp[i].waiting_time, tmp[i].turnaround, tmp[i].response_time);
        sumW += tmp[i].waiting_time;
        sumT += tmp[i].turnaround;
        sumR += tmp[i].response_time;
    }

    printf("\nPromedios:\n");
    printf("  Waiting (espera)     : %.2f\n", sumW/n);
    printf("  Turnaround (retorno) : %.2f\n", sumT/n);
    printf("  Response (respuesta) : %.2f\n", sumR/n);
}

// --- FCFS ---
static void simulate_fcfs(const Process in[], int n){
    Process p[MAX_PROCESOS];
    for(int i=0;i<n;i++) p[i] = in[i];

    qsort(p, n, sizeof(Process), cmp_proc);
    reset_runtime(p, n);

    Segment segs[MAX_SEGMENTS];
    int segCount = 0;

    int t = 0;
    for(int i=0;i<n;i++){
        if (t < p[i].arrival){
            // CPU ocioso
            segs[segCount++] = (Segment){ .pid = -1, .start = t, .end = p[i].arrival };
            t = p[i].arrival;
        }

        p[i].start_time = t;
        segs[segCount++] = (Segment){ .pid = p[i].id, .start = t, .end = t + p[i].burst };
        t += p[i].burst;
        p[i].finish_time = t;
        p[i].remaining = 0;

        if (segCount >= MAX_SEGMENTS){
            fprintf(stderr, "Demasiados segmentos en Gantt (MAX_SEGMENTS=%d)\n", MAX_SEGMENTS);
            break;
        }
    }

    finalize_metrics(p, n);

    printf("\n===========================================\n");
    printf("Algoritmo: FCFS (First-Come, First-Served)\n");
    printf("===========================================\n");

    print_timeline(segs, segCount);
    print_metrics(p, n);
}

// Encola todos los procesos con arrival <= currentTime a la cola RR.
static void enqueue_arrivals_rr(Process p[], int n, int *next, int currentTime, Queue *q){
    while (*next < n && p[*next].arrival <= currentTime){
        q_push(q, *next);
        (*next)++;
    }
}

// --- Round Robin ---
static void simulate_rr(const Process in[], int n, int quantum){
    Process p[MAX_PROCESOS];
    for(int i=0;i<n;i++) p[i] = in[i];

    qsort(p, n, sizeof(Process), cmp_proc);
    reset_runtime(p, n);

    Segment segs[MAX_SEGMENTS];
    int segCount = 0;

    Queue q; q_init(&q);

    int t = 0;
    int next = 0; // siguiente proceso por llegar (en el arreglo ya ordenado)

    if (n > 0){
        if (t < p[0].arrival) t = p[0].arrival;
        enqueue_arrivals_rr(p, n, &next, t, &q);
    }

    while (!q_empty(&q) || next < n){
        if (q_empty(&q)){
            // CPU ocioso hasta el siguiente arrival
            int nextArrival = p[next].arrival;
            if (t < nextArrival){
                segs[segCount++] = (Segment){ .pid = -1, .start = t, .end = nextArrival };
                t = nextArrival;
            }
            enqueue_arrivals_rr(p, n, &next, t, &q);
        }

        int idx = q_pop(&q);
        if (p[idx].start_time == -1) p[idx].start_time = t;

        int run = (p[idx].remaining < quantum) ? p[idx].remaining : quantum;
        int t2 = t + run;

        segs[segCount++] = (Segment){ .pid = p[idx].id, .start = t, .end = t2 };

        p[idx].remaining -= run;
        t = t2;

        // agregar los que llegaron durante el tramo
        enqueue_arrivals_rr(p, n, &next, t, &q);

        if (p[idx].remaining > 0){
            q_push(&q, idx); // re-encolar
        } else {
            p[idx].finish_time = t;
        }

        if (segCount >= MAX_SEGMENTS){
            fprintf(stderr, "Demasiados segmentos en Gantt (MAX_SEGMENTS=%d)\n", MAX_SEGMENTS);
            break;
        }
    }

    finalize_metrics(p, n);

    printf("\n========================================\n");
    printf("Algoritmo: RR (Round Robin), quantum=%d\n", quantum);
    printf("========================================\n");

    print_timeline(segs, segCount);
    print_metrics(p, n);
}

int main(int argc, char *argv[]) {
    // Quantum por defecto: 4 (común en muchos ejercicios)
    int quantum = 4;
    if (argc >= 2) {
        quantum = atoi(argv[1]);
        if (quantum <= 0) {
            fprintf(stderr, "Quantum invalido. Uso: %s [quantum] < Procesos.txt\n", argv[0]);
            return 1;
        }
    }

    Process procesos[MAX_PROCESOS];
    int n = 0;

    // Lee desde stdin hasta EOF: cada línea debe tener id arrival burst priority
    while (scanf("%d %d %d %d",
                 &procesos[n].id,
                 &procesos[n].arrival,
                 &procesos[n].burst,
                 &procesos[n].priority) == 4) {

        // Validaciones simples
        if (procesos[n].arrival < 0 || procesos[n].burst <= 0) {
            fprintf(stderr, "Linea invalida (arrival>=0, burst>0). Proceso P%d\n", procesos[n].id);
            return 1;
        }

        procesos[n].remaining    = procesos[n].burst;
        procesos[n].start_time   = -1;
        procesos[n].finish_time  = -1;
        procesos[n].waiting_time = 0;
        procesos[n].turnaround   = 0;
        procesos[n].response_time= 0;

        n++;
        if (n >= MAX_PROCESOS) {
            fprintf(stderr, "Se alcanzó el límite MAX_PROCESOS=%d\n", MAX_PROCESOS);
            break;
        }
    }

    if (n == 0) {
        fprintf(stderr, "No se leyeron procesos. Asegura ejecutar: %s [quantum] < Procesos.txt\n", argv[0]);
        return 1;
    }

    // Ejecutar algoritmos requeridos
    simulate_fcfs(procesos, n);
    simulate_rr(procesos, n, quantum);

    return 0;
}
