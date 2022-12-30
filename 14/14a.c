#include <stdio.h>
#include <string.h>

// 14a - the input is a list of vectors for drawing a 2D map.
//       the map is vertical and sand is falling in from the top;
//       it piles up on the described walls. we are to count how
//       much sand has to pile up before it starts falling through
//       bottom of the grid into empty space.
//
//
// idea: simple simulation with checking against the map bounds.
//
// https://adventofcode.com/2022/day/14
//

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef struct point_t point_t;
struct point_t {
    int x, y;
};

static char grid[1000][1000];
static int minx = 1000;
static int miny = 1000;
static int maxx = 0;
static int maxy = 0;

int main()
{
    int i;
    int done;
    int t0;
    int t1;
    char line[1000];
    char *p;
    point_t prev;
    point_t curr;

    memset(grid, '.', 1000 * 1000);

    while (fgets(line, sizeof(line), stdin)) {
        p = line;
        sscanf(p, "%d,%d", &prev.x, &prev.y);
        prev.x += 2;
        prev.y += 2;
        minx = MIN(minx, prev.x);
        miny = MIN(miny, prev.y);
        maxx = MAX(maxx, prev.x);
        maxy = MAX(maxy, prev.y);

        while (1) {
            p = strchr(p, ' ');
            if (p == NULL) {
                break;
            }
            p += 4;
            sscanf(p, "%d,%d", &curr.x, &curr.y);
            curr.x += 2;
            curr.y += 2;
            if (prev.x == curr.x) {
                t0 = MIN(prev.y, curr.y);
                t1 = MAX(prev.y, curr.y);
                for (; t0 <= t1; t0++) {
                    grid[t0][prev.x] = '#';
                }
            } else {
                t0 = MIN(prev.x, curr.x);
                t1 = MAX(prev.x, curr.x);
                for (; t0 <= t1; t0++) {
                    grid[prev.y][t0] = '#';
                }
            }
            
            prev = curr;
            minx = MIN(minx, prev.x);
            miny = MIN(miny, prev.y);
            maxx = MAX(maxx, prev.x);
            maxy = MAX(maxy, prev.y);            
        }
    }

    for (i = 0, done = 0; !done ; i++) {
        curr.x = 500+2;
        curr.y = 0+2;

        while (1) {
            if (grid[curr.y+1][curr.x] == '.')
                curr.y++;
            else if (grid[curr.y+1][curr.x-1] == '.')
                curr.y++, curr.x--;
            else if (grid[curr.y+1][curr.x+1] == '.')
                curr.y++, curr.x++;
            else {
                grid[curr.y][curr.x] = 'o';
                break;
            }

            if (curr.y > maxy) {
                done = 1;
                break;
            }
        }

   }

   printf("%d\n", i-1);
}
