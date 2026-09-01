#include <unistd.h>
#include <stdio.h>
#include "malloc.h"

int main() {
    char *ptr1 = ft_malloc(200); // First SMALL allocation
    char *ptr2 = ft_malloc(300); // Second SMALL allocation

    printf("ptr1 address: %p\n", (void *)ptr1);
    printf("ptr2 address: %p\n", (void *)ptr2);
    return 0;
}