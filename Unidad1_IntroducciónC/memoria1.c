#include<stdio.h>

int main(){
    
    int *ptr;
    ptr = calloc(4, sizeof(*ptr));

    *ptr =2;
    ptr[1] = 4;
    ptr[2] = 6;

    printf("%d\n", *ptr);
    printf("%d\n", ptr[1], ptr[2], ptr[3]);
    
    return 0;
}