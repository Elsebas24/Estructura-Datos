#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

typedef struct Nodo {
    int apunta;                 // posición del vecino en el arreglo
    struct Nodo* siguiente;
} Nodo;

// Cada vértice guarda su número (etiqueta) y su lista
typedef struct {
    int etiqueta;               // número con el que lo conoces: 1, 2, 8...
    Nodo* lista;                // lista de adyacencia
} Vertice;

typedef struct {
    Vertice* vertices;          // arreglo dinámico de vértices
    int numVertices;
} GrafoLista;


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

// El grafo empieza vacío; los vértices se agregan uno por uno
GrafoLista* crearGrafo(void) {
    GrafoLista* g = (GrafoLista*)malloc(sizeof(GrafoLista));
    if (!g) {
        printf("[ERROR] No se pudo asignar memoria para el grafo.\n");
        exit(EXIT_FAILURE);
    }
    g->vertices = NULL;
    g->numVertices = 0;
    return g;
}

// Busca la posición de un vértice según su etiqueta
// Regresa -1 si no existe
int buscarVertice(const GrafoLista* g, int etiqueta) {
    for (int i = 0; i < g->numVertices; i++) {
        if (g->vertices[i].etiqueta == etiqueta) {
            return i;
        }
    }
    return -1;
}

// Agrega un vértice nuevo (no se permiten repetidos)
int agregarVertice(GrafoLista* g, int etiqueta) {
    if (buscarVertice(g, etiqueta) != -1) {
        printf("[WARN] El vértice %d ya existe.\n", etiqueta);
        return -1;
    }

    int nuevoTam = (g->numVertices + 1) * sizeof(Vertice);
    Vertice* nuevo = (Vertice*)realloc(g->vertices, nuevoTam);
    if (!nuevo) {
        printf("[ERROR] No se pudo agregar el vértice.\n");
        return -1;
    }

    g->vertices = nuevo;
    g->vertices[g->numVertices].etiqueta = etiqueta;
    g->vertices[g->numVertices].lista = NULL;
    g->numVertices++;

    return g->numVertices - 1;
}

// Verifica si existe arista (recibe posiciones, no etiquetas)
bool existeArista(const GrafoLista* g, int u, int v) {
    Nodo* actual = g->vertices[u].lista;
    while (actual != NULL) {
        if (actual->apunta == v) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

// Agrega una arista no dirigida usando las etiquetas
void agregarArista(GrafoLista* g, int a, int b) {
    int u = buscarVertice(g, a);
    int v = buscarVertice(g, b);

    // Solo se aceptan vértices que fueron declarados
    if (u == -1 || v == -1) {
        printf("[WARN] Arista (%d, %d): vértice no declarado.\n", a, b);
        return;
    }

    // No se permiten aristas repetidas
    if (existeArista(g, u, v)) {
        printf("[WARN] La arista (%d, %d) ya existe.\n", a, b);
        return;
    }

    // Agrega v a la lista de u
    Nodo* nuevoV = crearNodo(v);
    nuevoV->siguiente = g->vertices[u].lista;
    g->vertices[u].lista = nuevoV;

    // Agrega u a la lista de v (grafo no dirigido)
    if (u != v) {
        Nodo* nuevoU = crearNodo(u);
        nuevoU->siguiente = g->vertices[v].lista;
        g->vertices[v].lista = nuevoU;
    }
}

// Calcula el grado de un vértice (por posición)
int calcularGrado(const GrafoLista* g, int i) {
    int grado = 0;
    Nodo* actual = g->vertices[i].lista;
    while (actual != NULL) {
        grado++;
        actual = actual->siguiente;
    }
    return grado;
}

// Imprime la lista de adyacencia mostrando las etiquetas
void imprimirGrafo(const GrafoLista* g) {
    printf("\n====== LISTA DE ADYACENCIA (%d Vértices) ======\n",
           g->numVertices);

    for (int i = 0; i < g->numVertices; i++) {
        printf("[%d] -> ", g->vertices[i].etiqueta);

        Nodo* actual = g->vertices[i].lista;
        while (actual != NULL) {
            printf("%d -> ", g->vertices[actual->apunta].etiqueta);
            actual = actual->siguiente;
        }

        printf("NULL | Grado: %d\n", calcularGrado(g, i));
    }
    printf("================================================\n");
}


/* ============================================================
   CAMINOS ENTRE DOS VÉRTICES
   ============================================================
   Internamente se trabaja con POSICIONES (0 a numVertices-1)
   para que visitado[] y camino[] tengan el tamaño exacto.
   Al imprimir se muestran las ETIQUETAS (1, 2, 3, 8...).

   Se recorre el grafo dos veces con DFS + vuelta atrás:
     1) buscarMinimo: encuentra cuántas aristas tiene
        el camino más corto.
     2) imprimirCaminos: muestra todos los caminos y marca
        los que tienen ese largo mínimo.
   ============================================================ */

// Recorrido 1: encuentra el número mínimo de aristas
void buscarMinimo(const GrafoLista* g, int actual, int fin,
                  bool* visitado, int largo, int* minimo) {

    visitado[actual] = true;

    if (actual == fin) {
        if (*minimo == -1 || largo < *minimo) {
            *minimo = largo;
        }
    } else {
        Nodo* vecino = g->vertices[actual].lista;
        while (vecino != NULL) {
            if (!visitado[vecino->apunta]) {
                buscarMinimo(g, vecino->apunta, fin,
                             visitado, largo + 1, minimo);
            }
            vecino = vecino->siguiente;
        }
    }

    // Vuelta atrás: se libera el vértice para otros caminos
    visitado[actual] = false;
}

// Recorrido 2: imprime todos los caminos y destaca el más corto
void imprimirCaminos(const GrafoLista* g, int actual, int fin,
                     bool* visitado, int* camino, int largo,
                     int minimo, int* total) {

    visitado[actual] = true;
    camino[largo] = actual;

    if (actual == fin) {
        (*total)++;

        if (largo == minimo) {
            printf("  *** Camino %d: ", *total);
        } else {
            printf("      Camino %d: ", *total);
        }

        for (int i = 0; i <= largo; i++) {
            printf("%d", g->vertices[camino[i]].etiqueta);
            if (i < largo) {
                printf(" -> ");
            }
        }

        printf("  (%d aristas)", largo);
        if (largo == minimo) {
            printf("  <== MÁS CORTO");
        }
        printf("\n");

    } else {
        Nodo* vecino = g->vertices[actual].lista;
        while (vecino != NULL) {
            if (!visitado[vecino->apunta]) {
                imprimirCaminos(g, vecino->apunta, fin, visitado,
                                camino, largo + 1, minimo, total);
            }
            vecino = vecino->siguiente;
        }
    }

    // Vuelta atrás
    visitado[actual] = false;
}

// Recibe ETIQUETAS, las convierte a posiciones y muestra todo
void mostrarCaminos(const GrafoLista* g, int a, int b) {

    int inicio = buscarVertice(g, a);
    int fin = buscarVertice(g, b);

    if (inicio == -1 || fin == -1) {
        printf("[WARN] El vértice no existe en el grafo.\n");
        return;
    }

    // Arreglos del tamaño exacto del grafo (calloc los deja en 0)
    bool* visitado = (bool*)calloc(g->numVertices, sizeof(bool));
    int* camino = (int*)calloc(g->numVertices, sizeof(int));

    if (!visitado || !camino) {
        printf("[ERROR] No se pudo asignar memoria.\n");
        free(visitado);
        free(camino);
        return;
    }

    int minimo = -1;   // -1 significa "no hay camino"
    int total = 0;

    buscarMinimo(g, inicio, fin, visitado, 0, &minimo);

    printf("\n====== CAMINOS DE %d A %d ======\n", a, b);

    if (minimo == -1) {
        printf("  No existe ningún camino.\n");
    } else {
        imprimirCaminos(g, inicio, fin, visitado, camino,
                        0, minimo, &total);
        printf("--------------------------------\n");
        printf("  Total de caminos: %d\n", total);
        printf("  Camino más corto: %d aristas\n", minimo);
    }
    printf("================================\n");

    free(visitado);
    free(camino);
}


// Libera la memoria del grafo
void liberarGrafo(GrafoLista* g) {
    for (int i = 0; i < g->numVertices; i++) {
        Nodo* actual = g->vertices[i].lista;
        while (actual != NULL) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(g->vertices);
    free(g);
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);   // la consola muestra acentos bien

    printf(">>> Creando Grafo con Lista Dinámica de Adyacencia...\n");
    GrafoLista* grafo = crearGrafo();

    // Solo se declaran los vértices que realmente se usan
    int etiquetas[] = {1, 2, 3, 4, 5, 6, 8, 9, 10};
    int n = sizeof(etiquetas) / sizeof(etiquetas[0]);

    for (int i = 0; i < n; i++) {
        agregarVertice(grafo, etiquetas[i]);
    }

    // Conectar aristas (con las etiquetas)
    agregarArista(grafo, 1, 3);
    agregarArista(grafo, 1, 5);
    agregarArista(grafo, 2, 3);
    agregarArista(grafo, 2, 5);
    agregarArista(grafo, 3, 4);
    agregarArista(grafo, 4, 6);
    agregarArista(grafo, 4, 10);
    agregarArista(grafo, 5, 6);
    agregarArista(grafo, 5, 9);
    agregarArista(grafo, 6, 8);
    agregarArista(grafo, 6, 9);
    agregarArista(grafo, 6, 10);
    agregarArista(grafo, 8, 9);

    imprimirGrafo(grafo);

    // Pedir vértice de inicio y final (por su número)
    int inicio, fin;
    printf("\nVértice de inicio: ");
    scanf("%d", &inicio);
    printf("Vértice final: ");
    scanf("%d", &fin);

    mostrarCaminos(grafo, inicio, fin);

    liberarGrafo(grafo);
    return 0;
}