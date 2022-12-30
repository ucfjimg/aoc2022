#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1a - sum continguous numbers, one per line, separated
//      by blank lines, and return the largest sum.
//
// idea: read in the numbers, sum each section, keep track 
//       of the max.
//
// https://adventofcode.com/2022/day/1
//
int main()
{
    int best = 0;
    int sum = 0;
    char line[100];
    
    while (fgets(line, sizeof(line), stdin)) {
        int l = strlen(line);
        if (l && line[l-1] == '\n') {
            line[--l] = '\0';
        }

        if (l == 0) {
            if (sum > best) {
                best = sum;
            }
            sum = 0;
            continue;
        }

        sum += strtol(line, NULL, 10);
    }

    if (sum > best) {
        best = sum;
    }

    printf("%d\n", best);
}

