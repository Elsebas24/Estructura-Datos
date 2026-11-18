#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Nodo {
    int dato;
    struct Nodo *next;
} Nodo;

typedef struct {
    Nodo *top;
    int size;
} StackDinamic;

void initStack(StackDinamic *s) {
    s->top = NULL;
    s->size = 0;
}

bool isEmpty(StackDinamic *s) {
    return s->top == NULL;
}

bool push(StackDinamic *s, int value) {
    Nodo *newNode = (Nodo *)malloc(sizeof(Nodo));
    if (newNode == NULL) {
        return false; // Memory allocation failed
    }
    newNode->dato = value;
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
    return true;
}

int pop(StackDinamic *s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Stack is empty. Cannot pop.\n");
        exit(EXIT_FAILURE); // Exit the program if stack is empty
    }

    Nodo *temp = s->top;
    int value = temp->dato;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return value;
}

int returnTop(StackDinamic *s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Stack is empty. Cannot return top.\n");
        exit(EXIT_FAILURE); // Exit the program if stack is empty
    }
    return s->top->dato;
}

void resetStack(StackDinamic *s) {
    while (!isEmpty(s)) {
        printf("Delete: %d\n", pop(s));
    }
}

void displayStack(StackDinamic *s) {
    Nodo *current = s->top;
    printf("Content of the stack(Top to down):\n");
    while (current != NULL) {
        printf("%d\n", current->dato);
        current = current->next;
    }
    printf("\n");
}

int main() {
    StackDinamic s;
    initStack(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    displayStack(&s);

    printf("Top element: %d\n", returnTop(&s));

    printf("Popped element: %d\n", pop(&s));
    displayStack(&s);
    
    resetStack(&s);

    return 0;
}