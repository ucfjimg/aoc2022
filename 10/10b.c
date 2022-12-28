#include <stdio.h>
#include <string.h>

int crt(int crtx, int pixx)
{
    if (pixx >= crtx-1 && pixx <= crtx+1) {
        putchar('#');
    } else {
        putchar(' ');
    }

    crtx++;
    if (crtx == 40) {
        putchar('\n');
        crtx = 0;
    }

    return crtx;
}

int main()
{
    int crtx = 0;
    int x = 1;
    int dx;
    char opcode[10];

    while (scanf("%s", opcode) == 1) {
        if (strcmp(opcode, "noop") == 0) {
            crtx = crt(crtx, x);
            continue;
        } else if (strcmp(opcode, "addx") == 0) {
            if (scanf("%d", &dx) != 1) {
                printf("couldn't read dx");
                return 1;
            }
            crtx = crt(crtx, x);
            crtx = crt(crtx, x);
            x += dx;
        }
    }
}
