#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 8a - the input is a rectangular map with a grid of heights of
//      trees. the output is the number of trees visible from
//      outside the grid.
//
// idea: for each grid square just outside the rectangle, walk
//       across the grid finding visible trees. keep the visible
//       trees in a set (a 2D array of the same size as the grid)
//       and then count up the size of the set.
//
// https://adventofcode.com/2022/day/8
//
#define MAX(x,y) ((x) > (y) ? (x) : (y))

static int height[100][100];
static int visible[100][100];

int main()
{
    char line[102];
    int gridsize;
    int row, col;
    int total;
    int max;

    if (!fgets(line, sizeof(line), stdin)) {
        printf("premature eof");
        return 1;
    }

    gridsize = strlen(line);
    while (gridsize && isspace(line[gridsize-1]))
        gridsize--;

    row = 0;
    do {
        for (col = 0; col < gridsize; col++) {
            height[col][row] = line[col] - '0'; 
        }
        row++;
    } while (fgets(line, sizeof(line), stdin));

    for (row = 0; row < gridsize; row++) {
        visible[0][row] = 1;
        visible[gridsize-1][row] = 1;
        
        max = height[0][row];
        for (col = 1; col < gridsize; col++) {
            if (height[col][row] > max) {
                visible[col][row] = 1;
            }
            max = MAX(max, height[col][row]);
        }

        max = height[gridsize-1][row];
        for (col = gridsize-2; col > 0; col--) {
            if (height[col][row] > max) {
                visible[col][row] = 1;
            }
            max = MAX(max, height[col][row]);
        }
    }

    for (col = 0; col < gridsize; col++) {
        visible[col][0] = 1;
        visible[col][gridsize-1] = 1;
        
        max = height[col][0];
        for (row = 1; row < gridsize; row++) {
            if (height[col][row] > max) {
                visible[col][row] = 1;
            }
            max = MAX(max, height[col][row]);
        }

        max = height[col][gridsize-1];
        for (row = gridsize-2; row > 0; row--) {
            if (height[col][row] > max) {
                visible[col][row] = 1;
            }
            max = MAX(max, height[col][row]);
        }
    }

    total = 0;
    for (row = 0; row < gridsize; row++) {
        for (col = 0; col < gridsize; col++) {
            if (visible[col][row])
                total++;
        }
    }

    printf("%d\n", total);
}
