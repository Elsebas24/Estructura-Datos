#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Nodo {
    int apunta;
    struct Nodo* siguiente;
} Nodo;

Nodo* crearNodo(int apunta) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        printf("[ERROR] No se pudo asignar memoria para el nodo.\n");
        exit(EXIT_FAILURE);
    }
    nuevo->apunta = apunta;
    nuevo->siguiente = NULL;
    return nuevo;
}

typedef struct {
    Nodo** Grafo;// Puntero a un arreglo de punteros a Nodo
    int numVertices;
} GrafoLista;

//Constructor para inicializar el grafo con lista de adyacencia
GrafoLista* crearGrafo(int vertices) {
    GrafoLista* g = (GrafoLista*)malloc(sizeof(GrafoLista));
    if (!g) {
        printf("[ERROR] No se pudo asignar memoria para el grafo.\n");
        exit(EXIT_FAILURE);
    }
    g->numVertices = vertices;
    g->Grafo = (Nodo**)malloc(vertices * sizeof(Nodo*));
    if (!g->Grafo && vertices > 0) {
        printf("[ERROR] No se pudo asignar memoria para la lista de adyacencia.\n");
        free(g);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < vertices; i++) {
        g->Grafo[i] = NULL; // Inicializa cada lista como vacía
    }
    return g;
}

// Verifica si existe arista en tiempo O(V)
bool existeArista(const GrafoLista* g, int u, int v) {
    if (u >= 0 && u < g->numVertices && v >= 0 && v < g->numVertices) {
        Nodo* actual = g->Grafo[u];
        while (actual != NULL) {
            if (actual->apunta == v) {
                return true;
            }
            actual = actual->siguiente;
        }
    }
    return false;
}

// Agrega una arista no dirigida entre origen y destino
void agregarArista(GrafoLista* g, int u, int v) {
    if (u >= 0 && u < g->numVertices && v >= 0 && v < g->numVertices) {
        // CAMBIO 3: no se permiten aristas repetidas
        if (existeArista(g, u, v)) {
            printf("[WARN] La arista (%d, %d) ya existe.\n", u, v);
            return;
        }
        // Agrega v a la lista de adyacencia de u
        Nodo* nuevoNodoV = crearNodo(v);
        nuevoNodoV->siguiente = g->Grafo[u];
        g->Grafo[u] = nuevoNodoV;

        // Agrega u a la lista de adyacencia de v (para grafo no dirigido)
        if (u != v) { // Evita agregar la misma arista en caso de bucle
            Nodo* nuevoNodoU = crearNodo(u);
            nuevoNodoU->siguiente = g->Grafo[v];
            g->Grafo[v] = nuevoNodoU;
        }
    } else {
        printf("[WARN] Vértices fuera de rango: (%d, %d)\n", u, v);
    }
}

int agregarVertice(GrafoLista* g) {
    Nodo** nuevo = (Nodo**)realloc(g->Grafo, (g->numVertices + 1) * sizeof(Nodo*));
    if (!nuevo) {
        printf("[ERROR] No se pudo agregar el vértice.\n");
        return -1;
    }
    g->Grafo = nuevo;
    g->Grafo[g->numVertices] = NULL;
    return g->numVertices++;
}

// Calcula el grado de un vértice en tiempo O(V)
int calcularGrado(const GrafoLista* g, int v) {
    if (v < 0 || v >= g->numVertices) return -1;
    int grado = 0;
    Nodo* actual = g->Grafo[v];
    while (actual != NULL) {
        grado++;
        actual = actual->siguiente;
    }
    return grado;
}

// Imprime la lista de adyacencia formateada
void imprimirGrafo(const GrafoLista* g) {
    printf("\n====== LISTA DE ADYACENCIA (%d Vértices) ======\n", g->numVertices);
    for (int i = 0; i < g->numVertices; i++) {
        printf("[%d] -> ", i);
        Nodo* actual = g->Grafo[i];
        while (actual != NULL) {
            printf("%d -> ", actual->apunta);
            actual = actual->siguiente;
        }
        printf("NULL | Grado: %d\n", calcularGrado(g, i));
    }
    printf("================================================\n");
}

// Libera la memoria del grafo
void liberarGrafo(GrafoLista* g) {
    for (int i = 0; i < g->numVertices; i++) {
        Nodo* actual = g->Grafo[i];
        while (actual != NULL) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(g->Grafo);
    free(g);
}
int main(void) {
    int vertices = 4;
    printf(">>> Creando Grafo con Lista Dinámica de Adyacencia...\n");
    GrafoLista* grafo = crearGrafo(vertices);

    // Conectar aristas: (0,1), (0,2), (1,2), (2,3)
    agregarArista(grafo, 0, 1);
    agregarArista(grafo, 0, 2);
    agregarArista(grafo, 1, 2);
    agregarArista(grafo, 2, 3); 

    imprimirGrafo(grafo);

    // Consultas
    printf("\nConsultas de adyacencia O(grado(u)):\n");
    printf("¿Existe arista (0, 2)? %s\n", existeArista(grafo, 0, 2) ? "SÍ" : "NO");
    printf("¿Existe arista (0, 3)? %s\n", existeArista(grafo, 0, 3) ? "SÍ" : "NO");

    // Liberación
    liberarGrafo(grafo);
    return 0;
}