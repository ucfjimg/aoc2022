#include <stdio.h>
#include <stdlib.h>

// 15a - the input is a list of 2D sensor positions; the sensors can detect
//       beacons. each sensor gives the location of the nearest (taxicab distance)
//       beacon it can see. we are to find, for a particular Y coordinate,
//       how many positions in that row cannot contain a beacon.
//
// idea: 1. sort and remove duplicate beacons
//       2. for each sensor, compute the span in the given Y that it covers.
//       3. sort the spans and walk them left to right counting total
//          coverage and accounting for overlap.
//       4. count how many beacons we know about that are in the spans.
//
//       step 3 minus step 4 is the answer.
//
// https://adventofcode.com/2022/day/15
//

typedef struct point_t point_t;
struct point_t {
    int x, y;
};

typedef struct sensor_t sensor_t;
struct sensor_t {
    point_t loc;
    int range;
};

typedef struct span_t span_t;
struct span_t {
    span_t *next;
    int left;
    int right;
};

static sensor_t sensors[100];
static point_t beacons[100];
static int nsensors;
static int nbeacons;
static span_t spans[100];
static int nspans;

int spancomp(const void *left, const void *right)
{
    const span_t *sl = left;
    const span_t *sr = right;

    return sl->left - sr->left;
}

int pointcomp(const void *left, const void *right) 
{
    const point_t *pl = left;
    const point_t *pr = right;

    if (pl->x == pr->x)
        return pl->y - pr->y;
    return pl->x - pr->x;
}

int main()
{
    int i;
    int j;
    sensor_t *s;
    point_t sens;
    point_t beac;
    int row = 10;
    long total;
    span_t *prev;
    span_t *curr;
    int src;
    int dst;

    row = 2000000;

    // read sensors
    //
    nsensors =0;
    while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sens.x, &sens.y, &beac.x, &beac.y) == 4) {
        sensors[nsensors].loc = sens;
        sensors[nsensors].range = abs(sens.x - beac.x) + abs(sens.y - beac.y);
        nsensors++;
        beacons[nbeacons++] = beac;
    }

    // sort the beacons first by x, then by y
    //
    qsort(beacons, nbeacons, sizeof(point_t), pointcomp);

    // remove duplicate beacons
    // 
    dst = 1;
    for (src = 1; src < nbeacons; src++) {
        if (beacons[src].x == beacons[dst-1].x && beacons[src].y == beacons[dst-1].y)
            continue;
        beacons[dst++] = beacons[src];
    }
    nbeacons = dst;

    // for each sensor, compute the beacon-free span it contributes
    // to the desired row.
    //
    for (i = 0; i < nsensors; i++) {
        s = &sensors[i];

        if (abs(s->loc.y - row) > s->range)
            continue;

        spans[nspans].left = s->loc.x - (s->range - abs(s->loc.y - row)); 
        spans[nspans].right = s->loc.x + (s->range - abs(s->loc.y - row)) + 1;
        
        nspans++;
    }

    // sort and merge the spans, computing the total coverage
    // along the way.
    //
    qsort(spans, nspans, sizeof(span_t), spancomp);

    total = spans[0].right - spans[0].left;
    prev = &spans[0];

    for (i = 1; i < nspans; i++) {
        curr = &spans[i];

        if (curr->right <= prev->right)
            continue;
        
        total += curr->right - curr->left;

        if (curr->left <= prev->right) {
            total -= prev->right - curr->left;
        }
        
        prev = curr;
    }

    // remove any known beacons from the row.
    //
    for (i = 0; i < nbeacons; i++) {
        if (beacons[i].y == row) {
            for (j = 0; j < nspans; j++) {
                if (beacons[i].x >= spans[j].left && beacons[i].x < spans[j].right) {
                    total--;
                    break;
                }
            }
        }
    }

    printf("%ld\n", total);
}
