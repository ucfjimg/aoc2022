#include <stdio.h>

// 4a - we're given a list of pairs of inclusive ranges,
//      and want to compute how many pairs contain 
//      ranges where one range contains the other range.
//
// idea: for each pair, compare the endpoints for containment.
//
// https://adventofcode.com/2022/day/4
//

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
