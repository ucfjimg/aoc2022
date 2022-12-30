#include <stdio.h>
#include <stdlib.h>

// 18a - the inputs is a list of 3D coordinates representing the positions of
//       cubes in space. find the number of cube sides that are not face-to-face
//       with other sides.
//
// idea: keep the cubes in a 3D array so finding if any cube exists is a simple
//       lookup. for each cube, check to see if, in each axis, there is a matching
//       cube in the increasing direction for that axis. since we only check one
//       direction on each axis, we only find each pair of matching faces once.
//       each cube has 6 sides, so we can just subtract off the shared sides.
//
// https://adventofcode.com/2022/day/18
//

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
