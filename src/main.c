#include <unistd.h>
#include <stdio.h>
#include "malloc.h"

int main() {
    printf("%d\n", getpagesize());
    char *i = NULL;
    i = ft_malloc(1024);
    (void)i;
    return 0;
}