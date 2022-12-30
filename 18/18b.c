#include <stdio.h>
#include <stdlib.h>

// 18b - the inputs is a list of 3D coordinates representing the positions of
//       cubes in space. find the number of cube sides that are not face-to-face
//       with other sides.
//
//       in part b, we want to find just the exterior boundary, discounting any
//       space totally enclosed inside the cubes.
//
// idea: mark each point in space as initially empty or contains a cube. then
//       do a 3D floodfill from a point known to be exterior to all cubes and mark
//       that space as exterior. we can now find boundaries at any point where
//       an exerior location meets a cube.
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
        pt[0] += 2;
        pt[1] += 2;
        pt[2] += 2;
        cubemap[pt[0]][pt[1]][pt[2]] = 1;
        cubes[ncubes][0] = pt[0];
        cubes[ncubes][1] = pt[1];
        cubes[ncubes][2] = pt[2];
        ncubes++;
    }
}

void floodfill(pt_t pt)
{
    int axis;
    pt_t next;

    if (pt[0] < 0 || pt[1] < 0 || pt[2] < 0) {
        return;
    }

    if (pt[0] > SIZE-1 || pt[1] > SIZE-1 || pt[2] > SIZE-1) {
        return;
    }

    if (cubemap[pt[0]][pt[1]][pt[2]] & 1) {
        return;
    }

    cubemap[pt[0]][pt[1]][pt[2]] |= 2;

    for (axis = 0; axis < 3; axis++) {
        next[0] = pt[0] + (axis == 0);
        next[1] = pt[1] + (axis == 1);
        next[2] = pt[2] + (axis == 2);
    
        if (cubemap[next[0]][next[1]][next[2]] == 0) {
            floodfill(next);
        }

        next[0] = pt[0] - (axis == 0);
        next[1] = pt[1] - (axis == 1);
        next[2] = pt[2] - (axis == 2);
    
        if (cubemap[next[0]][next[1]][next[2]] == 0) {
            floodfill(next);
        }
    }
}

int main()
{
    int axis;
    int cuidx;
    pt_t search;
    int boundry;
    int x, y, z;

    readcubes();

    int shared = 0;
    for (axis = 0; axis < 3; axis++) {
        for (cuidx = 0; cuidx < ncubes; cuidx++) {
            search[0] = cubes[cuidx][0] + (axis == 0);
            search[1] = cubes[cuidx][1] + (axis == 1);
            search[2] = cubes[cuidx][2] + (axis == 2);

            if (cubemap[search[0]][search[1]][search[2]] & 1) {
                shared++;
            }
        }
    }

    search[0] = 0;
    search[1] = 0;
    search[2] = 0;
    floodfill(search);

    boundry = 0;
    for (search[0] = 1; search[0] < SIZE-1; search[0]++) {
        for (search[1] = 1; search[1] < SIZE-1; search[1]++) {
            for (search[2] = 1; search[2] < SIZE-1; search[2]++) {
                if ((cubemap[search[0]][search[1]][search[2]] & 2) == 0) {
                    continue;
                }

                for (axis = 0; axis < 3; axis++) {
                    x = search[0] + (axis == 0);
                    y = search[1] + (axis == 1);
                    z = search[2] + (axis == 2);

                    if (cubemap[x][y][z] & 1) {
                        boundry++;
                    }
                
                    x = search[0] - (axis == 0);
                    y = search[1] - (axis == 1);
                    z = search[2] - (axis == 2);

                    if (cubemap[x][y][z] & 1) {
                        boundry++;
                    }
                }
            }   
        }
    }

    printf("%d\n", boundry);    
}
