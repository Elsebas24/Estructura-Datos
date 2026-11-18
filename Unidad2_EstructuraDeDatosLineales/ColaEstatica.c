//Bibliotecas
#include<stdio.h>
#include<stdbool.h>

#define MAX 6

typedef struct Cola{
    int cola[MAX];
    int ultimo;
    int primero;
}Cola;

void iniCola(Cola *cola){
    cola->primero=-1;
    cola->ultimo= -1;
}

bool estaVacia(Cola *cola){
    return (cola->primero==-1 && cola->ultimo==-1);
}

bool encolar(Cola *c, int dato){
    if(c->ultimo==MAX-1){
        return false;
    }else{
        if(estaVacia(c)){
            c->primero=0;
            c->ultimo=0;
        }else{
            c->ultimo++;
        }
        c->cola[c->ultimo]=dato;
        return true;
    }
}

int desencolar(Cola *c){
    if(estaVacia(c)){
        return -1;
    }else{
        int dato=c->cola[c->primero];
        if(c->primero==c->ultimo){
            c->primero=-1;
            c->ultimo=-1;
        }else{
            c->primero++;
        }
        return dato;
    }
}

void mostrarCola(Cola *c){
    if(estaVacia(c)){
        printf("La cola esta vacia\n");
    }else{
        printf("Elementos de la cola: ");
        for(int i=c->primero;i<=c->ultimo;i++){
            printf("%d ",c->cola[i]);
        }
        printf("\n");
    }
}

int main(){

    Cola c;
    iniCola(&c);
    encolar(&c, 1);
    encolar(&c, 2);
    encolar(&c, 3);
    encolar(&c, 4);
    mostrarCola(&c);
    desencolar(&c);
    desencolar(&c);
    mostrarCola(&c);
    encolar(&c, 5);
    encolar(&c, 6);
    encolar(&c, 7);
    mostrarCola(&c);

    return 0;

}