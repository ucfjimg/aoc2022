#include <stdio.h>

int main()
{
    unsigned l0, l1, r0, r1;
    int total = 0;

    while (scanf("%u-%u,%u-%u", &l0, &l1, &r0, &r1) == 4) {
        if ((l0 >= r0 && l1 <= r1) || (r0 >= l0 && r1 <= l1)) {
            total++;
        }
    }

    printf("%d\n", total);
}