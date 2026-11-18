#include<stdio.h>
#include<stdio.h>
#include<stdbool.h>

typedef struct Nodo{
    int dato;
    struct Nodo *siguiente;
}Nodo;

typedef struct Lista{
    
    Nodo *ultimo;
    int tamanio;
}MiLista;

void iniLista(MiLista *l){

    l->ultimo= NULL;
    l->tamanio=0;
}

bool empty(MiLista *l){
    if(l->ultimo== NULL)
        return true;
    else
        return false;
}

void insertar(MiLista *l, int x){
    Nodo* nuevo= (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato= x;
    nuevo->siguiente= NULL;

    if(empty(l)){
        
        l->ultimo= nuevo;
    }else{
        l->ultimo->siguiente= nuevo;
        l->ultimo= nuevo;
    }
    l->tamanio++;
}

bool buscar(MiLista *l, int x){
    Nodo* actual = l->ultimo;
    while (actual != NULL) {
        if(x== actual->dato){
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

int buscarYEliminar(MiLista *l, int x){
    Nodo* actual = l->ultimo;
    Nodo* anterior = NULL;

    while (actual != NULL) {
        if(x== actual->dato){
            if(anterior == NULL){
                l->ultimo = actual->siguiente;
            }else{
                anterior->siguiente = actual->siguiente;
            }
            free(actual);
            l->tamanio--;
            return 1; // Elemento eliminado
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return -1; // Elemento no encontrado
}

void liberar(MiLista *l){
    Nodo* actual = l->ultimo;
    while (actual != NULL) {
        Nodo* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    l->ultimo = NULL;
    l->tamanio = 0;
}

void imprimirLista(MiLista *l){
    Nodo* actual = l->ultimo;
    while (actual != NULL) {
        printf("%d -> ", actual->dato);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}


int main() {
    MiLista lista;
    iniLista(&lista);

    insertar(&lista, 3);
    insertar(&lista, 8);
    insertar(&lista, 9);
    insertar(&lista, 2);
    imprimirLista(&lista);
    if(buscar(&lista, 9)){
        printf("El elemento si esta en la lista \n");
    }

    buscarYEliminar(&lista, 9);
    if(buscar(&lista, 9)){
        printf("El elemento si esta en la lista \n");
    }else {printf("El elemento no esta en la lista \n");}

    imprimirLista(&lista);

}