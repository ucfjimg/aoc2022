#include <stdio.h>
#include <string.h>

// 6b - the one line of input is a packet full of letters;
//      we have to find the index of first set of 14 letters
//      that are all different.
//
// idea: for each position, build a bitmask set representing
//       the set of letters in the next 14 characters. if the
//       size of that set is 14, then all the letters are unique.
//
// https://adventofcode.com/2022/day/6

// counts the number of set bits in `n`.
//
int setbits(int n)
{
    int i = 0;

    while (n) {
        i++;
        n = n & (n-1);
    }

    return i;
}

int alldiff(char *p)
{
    // make a bitmask where each bit represents one seen
    // letter value. if there are 14 bits set then all
    // the letters are unique.
    //
    int flags = 0;

    for (int i = 0; i < 14; i++) {
        flags |= 1 << (*p++ - 'a');
    }

    return setbits(flags) == 14;
}

int main()
{
    char buffer[4097];

    int n = fread(buffer, 1, 4096, stdin);

    for (int i = 0; i < n - 13; i++) {
        if (alldiff(&buffer[i])) {
            printf("%d\n", i+14);
            break;
        }
    }
}
