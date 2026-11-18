#include <stdio.h>
#include <stdbool.h>

#define MAX_VERTICES 10

// Estructura para el Grafo con Matriz Estática
typedef struct {
    int matriz[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} GrafoMatriz;

// Inicializa la matriz estableciendo todos los enlaces en 0
void inicializarGrafo(GrafoMatriz* g, int vertices) {
    if (vertices > MAX_VERTICES) {
        printf("[ERROR] El número máximo de vértices permitido es %d.\n", MAX_VERTICES);
        vertices = MAX_VERTICES;
    }
    g->numVertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->matriz[i][j] = 0;
        }
    }
}

// Agrega una arista no dirigida entre origen y destino
void agregarArista(GrafoMatriz* g, int u, int v) {
    if (u >= 0 && u < g->numVertices && v >= 0 && v < g->numVertices) {
        g->matriz[u][v] = 1;
        g->matriz[v][u] = 1; // Grafo no dirigido (simétrico)
    } else {
        printf("[WARN] Vértices fuera de rango: (%d, %d)\n", u, v);
    }
}

// Verifica si existe arista en tiempo O(1)
bool existeArista(const GrafoMatriz* g, int u, int v) {
    if (u >= 0 && u < g->numVertices && v >= 0 && v < g->numVertices) {
        return g->matriz[u][v] == 1;
    }
    return false;
}

// Calcula el grado de un vértice en tiempo O(V)
int calcularGrado(const GrafoMatriz* g, int v) {
    if (v < 0 || v >= g->numVertices) return -1;
    int grado = 0;
    for (int j = 0; j < g->numVertices; j++) {
        if (g->matriz[v][j] == 1) grado++;
    }
    return grado;
}

// Imprime la matriz de adyacencia formateada
void imprimirMatriz(const GrafoMatriz* g) {
    printf("\n====== MATRIZ DE ADYACENCIA (%d Vértices) ======\n   ", g->numVertices);
    for (int j = 0; j < g->numVertices; j++) {
        printf("[%d] ", j);
    }
    printf("\n");

    for (int i = 0; i < g->numVertices; i++) {
        printf("[%d] ", i);
        for (int j = 0; j < g->numVertices; j++) {
            printf(" %d  ", g->matriz[i][j]);
        }
        printf(" | Grado: %d\n", calcularGrado(g, i));
    }
    printf("================================================\n");
}

int main(void) {
    GrafoMatriz grafo;
    int vertices;
    int numPares;

    printf("Ingrese el número de vértices (máx %d): ", MAX_VERTICES);
    scanf("%d", &vertices);
    printf("Ingrese el número de pares de vértices a conectar: ");
    scanf("%d", &numPares);

    int x[numPares], y[numPares];

    for (int i = 0; i < numPares; i++) {

        //Que el usuario ingrese los pares de vértices a conectar uno por uno

        printf("Ingrese el primer vértice del par %d: ", i + 1);
        scanf("%d", &x[i]);
        printf("Ingrese el segundo vértice del par %d: ", i + 1);
        scanf("%d", &y[i]);
        printf("Par ingresado: (%d, %d)\n", x[i], y[i]);
    }

    printf(">>> Inicializando Grafo con Matriz Estática...\n");
    inicializarGrafo(&grafo, vertices);

    // Agregar aristas al grafo
    for (int i = 0; i < numPares; i++) {
        agregarArista(&grafo, x[i], y[i]);
    }

    imprimirMatriz(&grafo);

    // Pruebas de consulta
    printf("\nConsultas de adyacencia O(1):\n");
    printf("¿Existe arista (2, 2)? %s\n", existeArista(&grafo, 2, 2) ? "SÍ" : "NO");
    printf("¿Existe arista (0, 3)? %s\n", existeArista(&grafo, 0, 3) ? "SÍ" : "NO");

    return 0;
}
