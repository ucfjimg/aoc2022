#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static char grid[200][200];
static char seen[200][200];
static int dist[200][200];
static int rows = 0;
static int cols = 0;

static int drow[] = { 0, 0, 1, -1 };
static int dcol[] = { 1, -1, 0, 0 };

int shortest_path(int startrow, int startcol)
{
    int i;
    int u, v;
    int row, col;
    int currrow, currcol;
    int nextrow, nextcol;
    char curr, next;
    int alt;
    int best;
    int left = rows * cols;

    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            seen[row][col] = 0;
            dist[row][col] = 2 * rows * cols;
        }
    }

    dist[startrow][startcol] = 0;

    while (left) {
        currrow = 0;
        currcol = 0;
        best = 2 * rows * cols;

        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {
                if (seen[row][col] == 0 && dist[row][col] < best) {
                    best = dist[row][col];
                    currrow = row;
                    currcol = col;
                }
            }
        }

        if (best == 2 * rows * cols)
            break;

        left--;
        seen[currrow][currcol] = 1;


        assert(best != 2 * rows * cols);

        curr = grid[currrow][currcol];
        for (i = 0; i < 4; i++) {
            nextrow = currrow + drow[i];
            nextcol = currcol + dcol[i];

            if (nextrow < 0 || nextcol < 0 || nextrow >= rows || nextcol >= cols) 
                continue;

            next = grid[nextrow][nextcol];
            if (next == 'E')
                next = 'z';

            if (curr == 'S' || next <= curr+1) {
                alt = dist[currrow][currcol] + 1;
                if (alt < dist[nextrow][nextcol]) {
                    dist[nextrow][nextcol] = alt;
                }
            }
        }
    }

    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++) 
            if (grid[row][col] == 'E')
                return dist[row][col];
}

int main()
{
    int len;
    int row, col;
    int startrow = -1, startcol = -1;
    int curr;
    int best;

    while (fgets(grid[rows], 200, stdin)) {
        len = strlen(grid[rows]) - 1;
        while (len >= 0 && isspace(grid[rows][len]))
            len--;
        if (len == -1)
            break;
        grid[rows][++len] = '\0';
        rows++;
        cols = len;
    }

    best = 2 * rows * cols;
    
    for (row = 0; row < rows; row++)
        for (col = 0; col < cols; col++) 
            if (grid[row][col] == 'a') {
                curr = shortest_path(row, col);
                if (curr < best) 
                    best = curr;                
            }

    printf("%d\n", best);
}