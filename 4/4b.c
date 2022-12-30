#include <stdio.h>

// 4b - we're given a list of pairs of inclusive ranges,
//      and want to compute how many pairs contain 
//      overlapping ranges.
//
// idea: for each pair, compare the endpoints for overlap.
//
// https://adventofcode.com/2022/day/4
//

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
