#include <stdio.h>
#include <stdbool.h>

#define MAX 5

typedef struct {
    int top;
    int datos[MAX];
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

void resetStack(Stack *s) {
    initStack(s);
}

bool isFull(Stack *s) {
    return s->top == MAX - 1;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

bool push(Stack *s, int value) {
    if (isFull(s)) {
        return false; // Stack is full
    }
    s->datos[++(s->top)] = value;
    return true;
}

int pop(Stack *s) {
    if (isEmpty(s)) {
        return -1; // Stack is empty
    }
    return s->datos[(s->top)--];
}

void displayStack(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack elements: ");
    for (int i = s->top; i >= 0; i--) {
        printf("%d ", s->datos[i]);
    }
    printf("\n");
}

int returnTop(Stack *s) {
    if (isEmpty(s)) {
        return -1; // Stack is empty
    }
    return s->datos[s->top];
}

int main(){
    Stack s;
    initStack(&s);
    int option = 0;
    do{
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Display Stack\n");
        printf("4. Return Top Element\n");
        printf("5. Reset Stack\n");
        printf("6. Exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);

        switch(option) {
            case 1: {
                int value;
                printf("Enter value to push: ");
                scanf("%d", &value);
                if (push(&s, value)) {
                    printf("Pushed %d onto the stack.\n", value);
                } else {
                    printf("Stack is full. Cannot push %d.\n", value);
                }
                break;
            }
            case 2: {
                int poppedValue = pop(&s);
                if (poppedValue != -1) {
                    printf("Popped %d from the stack.\n", poppedValue);
                } else {
                    printf("Stack is empty. Cannot pop.\n");
                }
                break;
            }
            case 3:
                displayStack(&s);
                break;
            case 4: {
                int topValue = returnTop(&s);
                if (topValue != -1) {
                    printf("Top element is %d.\n", topValue);
                } else {
                    printf("Stack is empty. No top element.\n");
                }
                break;
            }
            case 5:
                initStack(&s);
                printf("Stack has been reset.\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }


    }while(option != 6);
}