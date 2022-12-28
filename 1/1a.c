#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

