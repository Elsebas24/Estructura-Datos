#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
    int data;
    struct Nodo *left;
    struct Nodo *right;
} Nodo;

typedef struct Tree {
    Nodo *root;
} TreeRoot;

void inicializarTree(TreeRoot *tree)
{
    tree->root = NULL;
}

Nodo *crearNodo(int dato)
{
    Nodo *nuevoNodo = malloc(sizeof(Nodo));

    if (nuevoNodo == NULL) {
        fprintf(stderr, "No se pudo reservar memoria para el nodo.\n");
        exit(EXIT_FAILURE);
    }

    nuevoNodo->data = dato;
    nuevoNodo->left = NULL;
    nuevoNodo->right = NULL;

    return nuevoNodo;
}

void construirArbol(TreeRoot *tree)
{
    /*
              3
            /   \
           8     9
          / \     \
         7   1     2
        /
       6
    */

    tree->root = crearNodo(3);

    tree->root->left = crearNodo(8);
    tree->root->right = crearNodo(9);

    tree->root->left->left = crearNodo(7);
    tree->root->left->right = crearNodo(1);
    tree->root->right->right = crearNodo(2);

    tree->root->left->left->left = crearNodo(6);
}

void liberarArbol(Nodo *nodo)
{
    if (nodo == NULL) {
        return;
    }

    liberarArbol(nodo->left);
    liberarArbol(nodo->right);
    free(nodo);
}

int main(void)
{
    TreeRoot tree;

    inicializarTree(&tree);
    construirArbol(&tree);
    liberarArbol(tree.root);

    return 0;
}
