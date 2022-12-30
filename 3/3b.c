#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 3b - now we're looking for a common item that appears 
//      across each group of 3 lines, and adding up the priorities
//      of those.
//
// idea: represent each line by one bit; in an array indexed by
//       all possible letters, set the bits for all letters found
//       in the corresponding line. then, find the one letter
//       with all 3 bits set.
//
// https://adventofcode.com/2022/day/3
//
#define MAX_PRI 52

int to_pri(char ch)
{
    if (!isalpha(ch)) {
        printf("'%c' is not alpha\n", ch);
        assert(0);
    }

    if (isupper(ch)) {
        return 27 + ch - 'A';
    } else {
        return 1 + ch - 'a';
    }
}

int main()
{
    int elf = 0;
    int len;
    char line[100];
    int pris[MAX_PRI + 1];
    int pri;
    int sum = 0;

    // now each entry of the `pri` array is a bitmask
    // indicating if each item has been seen in row 0, 1, 
    // or 2 in the current group.
    //
    memset(pris, 0, sizeof(pris));
    while (fgets(line, sizeof(line), stdin)) {
        len = strlen(line);
        while (len && isspace(line[len-1]))
            len--;

        if (len == 0) {
            break;
        }

        line[len] = '\0';

        // each item has a bitmask, one bit per elf
        for (int i = 0; i < len; i++) {
            pris[to_pri(line[i])] |= (1 << elf);
        }

        elf++;
        if (elf == 3) {
            for (int i = 1; i < MAX_PRI+1; i++) {
                if (pris[i] == 7) { // 7 = all 3 elf bits
                    sum += i;
                    break;
                }
            }

            elf = 0;
            memset(pris, 0, sizeof(pris));
        }
    }

    printf("%d\n", sum);
}
