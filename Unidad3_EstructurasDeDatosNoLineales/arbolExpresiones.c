/*
    Autor: Geisler Jimenez Torres
    Proposito: Arbol de expresiones con dos pilas dinamicas
               (una de arboles y otra de operadores).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Nodo{
    char op;
    int val;
    bool esOp;
    struct Nodo *izq;
    struct Nodo *der;
}Nodo;

/* Pila de arboles (operandos ya armados) */
typedef struct NodoPilaA{
    Nodo *arbol;
    struct NodoPilaA *sig;
}NodoPilaA;

typedef struct PilaArboles{
    NodoPilaA *tope;
}PilaArboles;

/* Pila de operadores pendientes */
typedef struct NodoPilaO{
    char op;
    struct NodoPilaO *sig;
}NodoPilaO;

typedef struct PilaOps{
    NodoPilaO *tope;
}PilaOps;


/* Unico metodo que reserva memoria */
void *reservarmemoria(int tamano){
    void *p = malloc(tamano);

    if(p == NULL){
        printf("No hay memoria disponible\n");
        exit(1);
    }
    return p;
}

/* Lee una linea de cualquier largo */
char *leerTexto(){
    char *texto = reservarmemoria(1);
    char *nuevo;
    int n = 0;
    int c;

    texto[0] = '\0';
    c = getchar();

    while(c != '\n' && c != EOF){
        nuevo = reservarmemoria(n + 2);
        strcpy(nuevo, texto);
        nuevo[n] = c;
        nuevo[n + 1] = '\0';
        free(texto);
        texto = nuevo;
        n++;
        c = getchar();
    }
    return texto;
}


/* ---------- Crear nodos del arbol ---------- */

Nodo *crearValorNodo(int val){
    Nodo *n = reservarmemoria(sizeof(Nodo));

    n->esOp = false;    
    n->val = val;
    n->op = ' ';
    n->izq = NULL;
    n->der = NULL;
    return n;
}

Nodo *crearOpNodo(char op, Nodo *izq, Nodo *der){
    Nodo *n = reservarmemoria(sizeof(Nodo));

    n->esOp = true;
    n->op = op;
    n->val = 0;
    n->izq = izq;
    n->der = der;
    return n;
}

void liberarArbol(Nodo *r){
    if(r == NULL){
        return;
    }
    liberarArbol(r->izq);
    liberarArbol(r->der);
    free(r);
}


/* ---------- Pila de arboles ---------- */

void pushArbol(PilaArboles *p, Nodo *a){
    NodoPilaA *nuevo = reservarmemoria(sizeof(NodoPilaA));

    nuevo->arbol = a;
    nuevo->sig = p->tope;
    p->tope = nuevo;
}

/* Regresa NULL si la pila esta vacia */
Nodo *popArbol(PilaArboles *p){
    NodoPilaA *aux = p->tope;
    Nodo *a;

    if(aux == NULL){
        return NULL;
    }
    a = aux->arbol;
    p->tope = aux->sig;
    free(aux);
    return a;
}


/* ---------- Pila de operadores ---------- */

void pushOp(PilaOps *p, char op){
    NodoPilaO *nuevo = reservarmemoria(sizeof(NodoPilaO));

    nuevo->op = op;
    nuevo->sig = p->tope;
    p->tope = nuevo;
}

/* Regresa '\0' si la pila esta vacia */
char popOp(PilaOps *p){
    NodoPilaO *aux = p->tope;
    char op;

    if(aux == NULL){
        return '\0';
    }
    op = aux->op;
    p->tope = aux->sig;
    free(aux);
    return op;
}

char topeOp(PilaOps *p){
    if(p->tope == NULL){
        return '\0';
    }
    return p->tope->op;
}


/* ---------- Construccion del arbol ---------- */

int precedente(char op){
    switch(op){
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default:  return 0;
    }
}

/* Saca un operador y dos arboles, arma el subarbol y lo regresa a la pila.
   El primero que sale es el hijo derecho. Regresa false si faltan operandos */
bool armarSubarbol(PilaArboles *pa, PilaOps *po){
    char op = popOp(po);
    Nodo *der = popArbol(pa);
    Nodo *izq = popArbol(pa);

    if(der == NULL || izq == NULL){
        liberarArbol(der);
        liberarArbol(izq);
        return false;
    }
    pushArbol(pa, crearOpNodo(op, izq, der));
    return true;
}

/* Regresa la raiz, o NULL si la expresion esta mal escrita */
Nodo *construirArbol(char *expr){
    PilaArboles pa;
    PilaOps po;
    Nodo *raiz = NULL;
    bool ok = true;
    int i = 0;
    int num;
    char c;

    pa.tope = NULL;
    po.tope = NULL;

    while(expr[i] != '\0' && ok){
        c = expr[i];

        if(c == ' '){
            i++;
        }else if(isdigit(c)){
            num = 0;
            while(isdigit(expr[i])){
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            pushArbol(&pa, crearValorNodo(num));
        }else if(c == '('){
            pushOp(&po, c);
            i++;
        }else if(c == ')'){
            while(topeOp(&po) != '(' && topeOp(&po) != '\0' && ok){
                ok = armarSubarbol(&pa, &po);
            }
            if(topeOp(&po) == '('){
                popOp(&po);
            }else{
                ok = false;
            }
            i++;
        }else if(c == '+' || c == '-' || c == '*' || c == '/'){
            while(topeOp(&po) != '(' && topeOp(&po) != '\0' && ok
                  && precedente(topeOp(&po)) >= precedente(c)){
                ok = armarSubarbol(&pa, &po);
            }
            pushOp(&po, c);
            i++;
        }else{
            ok = false;
        }
    }

    /* Se arma lo que quedo pendiente */
    while(po.tope != NULL && ok){
        if(topeOp(&po) == '('){
            ok = false;
        }else{
            ok = armarSubarbol(&pa, &po);
        }
    }

    if(ok){
        raiz = popArbol(&pa);
        if(raiz == NULL || pa.tope != NULL){
            ok = false;
        }
    }

    /* Si hubo error se libera todo lo que haya quedado en las pilas */
    if(!ok){
        liberarArbol(raiz);
        raiz = NULL;
        while(pa.tope != NULL){
            liberarArbol(popArbol(&pa));
        }
    }
    while(po.tope != NULL){
        popOp(&po);
    }
    return raiz;
}


/* ---------- Recorridos ---------- */

void imprimirNodo(Nodo *n){
    if(n->esOp){
        printf("%c ", n->op);
    }else{
        printf("%d ", n->val);
    }
}

void preorden(Nodo *r){
    if(r == NULL){
        return;
    }
    imprimirNodo(r);
    preorden(r->izq);
    preorden(r->der);
}

void inorden(Nodo *r){
    if(r == NULL){
        return;
    }
    if(r->esOp){
        printf("( ");
    }
    inorden(r->izq);
    imprimirNodo(r);
    inorden(r->der);
    if(r->esOp){
        printf(") ");
    }
}

void postorden(Nodo *r){
    if(r == NULL){
        return;
    }
    postorden(r->izq);
    postorden(r->der);
    imprimirNodo(r);
}

/* Se ve de lado: derecha arriba, izquierda abajo */
void imprimirArbol(Nodo *r, int nivel){
    int i;

    if(r == NULL){
        return;
    }
    imprimirArbol(r->der, nivel + 1);
    for(i = 0; i < nivel; i++){
        printf("    ");
    }
    imprimirNodo(r);
    printf("\n");
    imprimirArbol(r->izq, nivel + 1);
}


/* ---------- Operaciones ---------- */

int contarNodos(Nodo *r){
    if(r == NULL){
        return 0;
    }
    return 1 + contarNodos(r->izq) + contarNodos(r->der);
}

int contarHojas(Nodo *r){
    if(r == NULL){
        return 0;
    }
    if(r->izq == NULL && r->der == NULL){
        return 1;
    }
    return contarHojas(r->izq) + contarHojas(r->der);
}

int altura(Nodo *r){
    int hi;
    int hd;

    if(r == NULL){
        return -1;
    }
    hi = altura(r->izq);
    hd = altura(r->der);
    if(hi > hd){
        return 1 + hi;
    }
    return 1 + hd;
}

/* Evalua el arbol de abajo hacia arriba. ok pasa a false si divide entre 0 */
int evaluar(Nodo *r, bool *ok){
    int a;
    int b;

    if(r == NULL){
        return 0;
    }
    if(!r->esOp){
        return r->val;
    }
    a = evaluar(r->izq, ok);
    b = evaluar(r->der, ok);

    switch(r->op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if(b == 0){
                *ok = false;
                return 0;
            }
            return a / b;
    }
    return 0;
}


int main(){
    char *expr;
    Nodo *raiz;
    bool ok;
    int resultado;

    printf("Escribe una expresion (ej: (3+4)*2-10/5) o 'salir'\n");

    while(1){
        printf("\nExpresion: ");
        expr = leerTexto();

        if(strcmp(expr, "salir") == 0){
            free(expr);
            break;
        }

        raiz = construirArbol(expr);

        if(raiz == NULL){
            printf("Expresion invalida\n");
        }else{
            printf("\nArbol:\n");
            imprimirArbol(raiz, 0);
            printf("\nPreorden:  ");
            preorden(raiz);
            printf("\nInorden:   ");
            inorden(raiz);
            printf("\nPostorden: ");
            postorden(raiz);
            printf("\n\nNodos: %d\n", contarNodos(raiz));
            printf("Hojas: %d\n", contarHojas(raiz));
            printf("Altura: %d\n", altura(raiz));

            ok = true;
            resultado = evaluar(raiz, &ok);
            if(ok){
                printf("Resultado: %d\n", resultado);
            }else{
                printf("Resultado: division entre cero\n");
            }
            liberarArbol(raiz);
        }
        free(expr);
    }

    printf("Programa terminado\n");
    return 0;
}