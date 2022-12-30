#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 3a - we're given the content of rucksacks; each line
//      contains two compartments of stuff. find the 
//      item that's in both compartments and add the 
//      priority (score based on item letter)
//
// idea: for the first half of a line, built a set of
//       seen characters; for the second half, find the 
//       character that's in the set. since it's a small
//       number of letters, the set is just an array.
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
    int len;
    char line[100];
    int pris[MAX_PRI + 1];
    int pri;
    int sum = 0;

    while (fgets(line, sizeof(line), stdin)) {
        len = strlen(line);
        while (len && isspace(line[len-1]))
            len--;

        if (len == 0) {
            continue;
        }

        line[len] = '\0';

        memset(pris, 0, sizeof(pris));

        for (int i = 0; i < len / 2; i++) {
            pris[to_pri(line[i])] = 1;
        }

        for (int i = len / 2; i < len; i++) {
            pri = to_pri(line[i]);
            if (pris[pri]) {
                sum += pri;
                break;
            }
        }
    }

    printf("%d\n", sum);
}
