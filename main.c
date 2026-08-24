#include <unistd.h>
#include <stdio.h>

int main() {
    printf("%d\n", getpagesize());
    return 0;
}