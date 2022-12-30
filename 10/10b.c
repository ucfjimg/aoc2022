#include <stdio.h>
#include <string.h>

// 10b - the input is a set of nop and add instructions with
//       strict timing. nop takes one cycle; add takes two
//       cycles and updates a register (X). in this part,
//       X is a sprite register a raster is moving across
//       a CRT. if the value of X is near the raster position,
//       a pixel is drawn. the output is a series of characters
//       drawn on the screen.
//
// idea: simulate the instructions and draw the raster as we go.
//
// https://adventofcode.com/2022/day/10
//

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
