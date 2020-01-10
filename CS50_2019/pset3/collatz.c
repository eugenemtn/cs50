#include <stdio.h>

int collatz(int n);

int main(void) {
    printf("Collatz of 7: %i\n", collatz(7));
}

int collatz(int n)
{
    if (n == 1)
        return 0;
    else if ((n % 2) == 0)
        return 1 + collatz(n / 2);
    else
        return 1 + collatz(n * 3 + 1);
}