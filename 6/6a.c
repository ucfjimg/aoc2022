#include <stdio.h>
#include <string.h>

// 6a - the one line of input is a packet full of letters;
//      we have to find the index of first set of 4 letters that
//      are all different.
//
// idea: since the input isn't very large, just have a loop
//       over all positions, and at that position, check 
//       if the next 4 letters are all different.
//
// https://adventofcode.com/2022/day/6

int alldiff(char *p)
{
    for (int i = 0; i < 3; i++) {
        for (int j = i+1; j < 4; j++) {
            if (p[i] == p[j]) {
                return 0;
            }
        }
    }
    return 1;
}

int main()
{
    char buffer[4097];

    int n = fread(buffer, 1, 4096, stdin);

    for (int i = 0; i < n - 3; i++) {
        if (alldiff(&buffer[i])) {
            printf("%d\n", i+4);
            break;
        }
    }
}
