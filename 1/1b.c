#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1b - as 1a, but now we want the product of the 3 largest
//      sums.
//
// idea: keep an array of the section sums, sort it, and
//       multiply the top 3 sums.
//
// https://adventofcode.com/2022/day/1
//
int comp(const void *l, const void *r)
{
    int li = *(const int *)l;
    int ri = *(const int *)r;

    return ri - li; 
}

int main()
{
    char line[200];
    int sums[1000];
    int n = 0;
    
    sums[0] = 0;
    while (fgets(line, sizeof(line), stdin)) {
        int l = strlen(line);
        if (l && line[l-1] == '\n') {
            line[--l] = '\0';
        }

        if (l == 0) {
            sums[++n] = 0;
            continue;
        }

        sums[n] += strtol(line, NULL, 10);
    }

    if (sums[n])
        n++;
    
    qsort(sums, n, sizeof(sums[0]), comp);
    printf("%d\n", sums[0] + sums[1] + sums[2]);
}



