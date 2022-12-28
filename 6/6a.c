#include <stdio.h>
#include <string.h>

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