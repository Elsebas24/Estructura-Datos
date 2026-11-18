#include<stdio.h>
#include<stdlib.h>

typedef struct Nodo{
    int info;
    struct NodoNuevo *left;
    struct NodoNuevo *right;
} NodoNuevo;

struct NodoNuevo *raiz = NULL;

void insertar(int valor){
    NodoNuevo *nuevoNodo = (struct NodoNuevo*)malloc(sizeof(NodoNuevo));
    nuevoNodo->info = valor;
    nuevoNodo->left = NULL;
    nuevoNodo->right = NULL;

    if(raiz == NULL){
        raiz = nuevoNodo;
    } else {
        NodoNuevo *anterior = NULL;
        NodoNuevo *recorrido = raiz;

        while(recorrido != NULL){
            anterior = recorrido;
            if(valor < recorrido->info){
                recorrido = recorrido->left;
            } else {
                recorrido = recorrido->right;
            }
        }
        if(valor < anterior->info){
            anterior->left = nuevoNodo;
        } else {
            anterior->right = nuevoNodo;
        }
    }
}

imprimirPreOrder(NodoNuevo *nodo){
    if(nodo != NULL){
        printf("%d ", nodo->info);
        imprimirPreOrder(nodo->left);
        imprimirPreOrder(nodo->right);
    }
}

imprimirInOrder(NodoNuevo *nodo){
    if(nodo != NULL){
        imprimirInOrder(nodo->left);
        printf("%d ", nodo->info);
        imprimirInOrder(nodo->right);
    }
}

imprimirPostOrder(NodoNuevo *nodo){
    if(nodo != NULL){
        imprimirPostOrder(nodo->left);
        imprimirPostOrder(nodo->right);
        printf("%d ", nodo->info);
    }
}

int main(){
    insertar(100);
    insertar(50);
    insertar(25);
    insertar(75);
    insertar(150);
    
    printf("Recorrido Pre-Order: ");
    imprimirPreOrder(raiz);
    printf("\n");

    printf("Recorrido In-Order: ");
    imprimirInOrder(raiz);
    printf("\n");

    printf("Recorrido Post-Order: ");
    imprimirPostOrder(raiz);
    printf("\n");

    return 0;
}