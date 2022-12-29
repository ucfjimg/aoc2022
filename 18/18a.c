#include <stdio.h>
#include <stdlib.h>

typedef int pt_t[3];

#define SIZE 24

static pt_t cubes[10000];
static int cubemap[SIZE][SIZE][SIZE];
static int ncubes;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

void readcubes(void)
{
    pt_t pt;

    while (scanf("%d,%d,%d\n", &pt[0], &pt[1], &pt[2]) == 3) {
        cubemap[pt[0]][pt[1]][pt[2]] = 1;
        cubes[ncubes][0] = pt[0];
        cubes[ncubes][1] = pt[1];
        cubes[ncubes][2] = pt[2];
        ncubes++;
    }
}

int main()
{
    int axis;
    int cuidx;
    pt_t search;

    readcubes();

    int shared = 0;
    for (axis = 0; axis < 3; axis++) {
        for (cuidx = 0; cuidx < ncubes; cuidx++) {
            search[0] = cubes[cuidx][0] + (axis == 0);
            search[1] = cubes[cuidx][1] + (axis == 1);
            search[2] = cubes[cuidx][2] + (axis == 2);

            if (cubemap[search[0]][search[1]][search[2]]) {
                shared++;
            }
        }
    }

    printf("%d\n", ncubes * 6 - 2 * shared);
}