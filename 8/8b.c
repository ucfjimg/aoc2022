#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 8b - the input is a rectangular map with a grid of heights of
//      trees. now we want to find the tree with the best scenic
//      view, as scored by the product of the visible trees in the
//      four cardinal directions.
//
// idea: for each tree in the grid, walk outward to the edge in all
//       four directions counting visible trees. keep the best score.
//
// https://adventofcode.com/2022/day/8
//

#define MAX(x,y) ((x) > (y) ? (x) : (y))

static int height[100][100];

int main()
{
    char line[102];
    int gridsize;
    int row, col;
    int i, len;
    unsigned long long best;

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

    best = 0;
    for (col = 0; col < gridsize; col++) {
        for (row = 0; row < gridsize; row++) {
            unsigned long long score = 1;
            len = 0;
            for (i = row-1; i >= 0; i--) {
                len++;
                if (height[col][i] >= height[col][row]) {
                    break;
                }
            }
            score *= len;

            len = 0;
            for (i = row+1; i < gridsize; i++) {
                len++;
                if (height[col][i] >= height[col][row]) {
                    break;
                }
            }
            score *= len;

            len = 0;
            for (i = col-1; i >= 0; i--) {
                len++;
                if (height[i][row] >= height[col][row]) {
                    break;
                }
            }
            score *= len;

            len = 0;
            for (i = col+1; i < gridsize; i++) {
                len++;
                if (height[i][row] >= height[col][row]) {
                    break;
                }
            }
            score *= len;
        
            best = MAX(best, score);
        }
    }

    printf("%llu\n", best);
}
