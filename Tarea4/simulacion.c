#include <stdio.h>
#include <string.h>

#define NODES 4

// Índices de nodos
// 0 = T1, 1 = T2, 2 = R1, 3 = R2

const char *names[NODES] = {"T1", "T2", "R1", "R2"};

int graph[NODES][NODES] = {0};

void add_edge(int from, int to) {
    graph[from][to] = 1;
}

void print_graph() {
    printf("Grafo de asignacion de recursos:\n");
    for (int i = 0; i < NODES; i++) {
        for (int j = 0; j < NODES; j++) {
            if (graph[i][j]) {
                printf("  %s -> %s\n", names[i], names[j]);
            }
        }
    }
}

int visited[NODES];
int recStack[NODES];

int has_cycle_util(int node) {
    visited[node] = 1;
    recStack[node] = 1;

    for (int neighbor = 0; neighbor < NODES; neighbor++) {
        if (graph[node][neighbor]) {
            if (!visited[neighbor]) {
                if (has_cycle_util(neighbor)) {
                    return 1;
                }
            } else if (recStack[neighbor]) {
                return 1;
            }
        }
    }

    recStack[node] = 0;
    return 0;
}

int has_cycle() {
    memset(visited, 0, sizeof(visited));
    memset(recStack, 0, sizeof(recStack));

    for (int i = 0; i < NODES; i++) {
        if (!visited[i]) {
            if (has_cycle_util(i)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    // Asignaciones:
    // R1 -> P1
    // R2 -> P2
    add_edge(2, 0);
    add_edge(3, 1);

    // Solicitudes:
    // P1 -> R2
    // P2 -> R1
    add_edge(0, 3);
    add_edge(1, 2);

    print_graph();

    if (has_cycle()) {
        printf("\nSe detecto un ciclo en el grafo.\n\n");
        printf("Conclusion: existe DEADLOCK.\n");
    } else {
        printf("\nNo se detecto ningun ciclo.\n");
        printf("Conclusion: no hay deadlock.\n");
    }

    return 0;
}