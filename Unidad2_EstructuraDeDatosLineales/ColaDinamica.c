//Bibliotecas
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Nodo {
    int dato;
    struct Nodo *siguiente;
} NuevoNodo;

typedef struct Cola {
    NuevoNodo *primero;
    NuevoNodo *ultimo;
    int tamano; // Variable para almacenar el tamaño de la cola
} Cola;

void iniCola(Cola *cola) {
    cola->primero = NULL;
    cola->ultimo = NULL;
    cola->tamano = 0; // Inicializar el tamaño de la cola a 0
}

bool empty(Cola *cola) {
    return (cola->primero == NULL || cola->tamano == 0);
}

bool encolar(Cola *cola, int valor) {
    NuevoNodo *nuevoNodo = (NuevoNodo *)malloc(sizeof(NuevoNodo));
    if (nuevoNodo == NULL) {
        return false; // Error al asignar memoria
    }
    nuevoNodo->dato = valor;
    nuevoNodo->siguiente = NULL;

    if (empty(cola)) {
        cola->primero = nuevoNodo;
        cola->ultimo = nuevoNodo;
    } else {
        cola->ultimo->siguiente = nuevoNodo;
        cola->ultimo = nuevoNodo;
    }
    cola->tamano++; // Incrementar el tamaño de la cola
    return true;
}

int desencolar(Cola *cola) {
    if (empty(cola)) {
        return -1; // Cola vacía
    }
    NuevoNodo *nodoAEliminar = cola->primero;
    int valor = nodoAEliminar->dato;
    cola->primero = nodoAEliminar->siguiente;
    free(nodoAEliminar);
    cola->tamano--; // Decrementar el tamaño de la cola
    if (cola->primero == NULL) {
        cola->ultimo = NULL; // La cola está vacía después de desencolar
    }
    return valor;
}

void liberarCola(Cola *cola) {
    while (!empty(cola)) {
        desencolar(cola);
    }
}

void imprimirCola(Cola *cola) {
    NuevoNodo *actual = cola->primero;
    while (actual != NULL) {
        printf("%d ", actual->dato);
        actual = actual->siguiente;
    }
    printf("\n");
}

int obtenerTamano(Cola *cola) {
    return cola->tamano; // Retornar el tamaño actual de la cola
}

int main() {
    Cola cola;
    iniCola(&cola);

    encolar(&cola, 10);
    encolar(&cola, 20);
    encolar(&cola, 30);

    printf("Contenido de la cola: ");
    imprimirCola(&cola);
    printf("Tamaño de la cola: %d\n", obtenerTamano(&cola));

    int valorDesencolado = desencolar(&cola);
    printf("Valor desencolado: %d\n", valorDesencolado);
    printf("Contenido de la cola después de desencolar: ");
    imprimirCola(&cola);
    printf("Tamaño de la cola después de desencolar: %d\n", obtenerTamano(&cola));

    liberarCola(&cola);
    return 0;
}