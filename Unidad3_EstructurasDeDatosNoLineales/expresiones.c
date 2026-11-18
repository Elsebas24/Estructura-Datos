#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//Creamos el nodo

typedef struct Nodo {
    char op;
    int val;
    bool esOp;
    struct Nodo *izq;
    struct Nodo *der;    
} Nodo;

//Funciones auxiliares para crear nodos

Nodo* crear_valor_nodo(int val){
    Nodo* n = (Nodo*)malloc(sizeof(Nodo));
    
    n->esOp = false;
    n->val = val;
    n->izq = NULL;
    n->der = NULL;

    return n;
}

Nodo* crear_op_nodo(char op, Nodo* izq, Nodo* der){
    Nodo* n = (Nodo*)malloc(sizeof(Nodo));
    n->esOp = true;
    n->op = op;
    n->izq = izq;
    n->der = der;
    
    return n;
}

int precedente(char op){
    switch(op){
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default:  return 0;
    }
}

//Construccion de un subarbol a partir de las pilas
void reduce_subexprecion(char op, Nodo** nodo_stack, int* nodo_top){
    Nodo* izq = nodo_stack[*nodo_top--];
    Nodo* der = nodo_stack[*nodo_top--];
    Nodo* op_nodo = crear_op_nodo(op, izq, der);
}