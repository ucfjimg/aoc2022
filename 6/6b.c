#include <stdio.h>
#include <string.h>

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