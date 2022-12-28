#include <stdio.h>

int main()
{
    unsigned l0, l1, r0, r1;
    int total = 0;

    while (scanf("%u-%u,%u-%u", &l0, &l1, &r0, &r1) == 4) {
        if (r1 < l0) continue;
        if (l1 < r0) continue;
        total++;
    }

    printf("%d\n", total);
}