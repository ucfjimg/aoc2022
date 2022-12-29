#include <stdio.h>
#include <stdlib.h>

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
static int nsensors;
static span_t spans[4000000];
static int nspans;

int spancomp(const void *left, const void *right)
{
    const span_t *sl = left;
    const span_t *sr = right;

    return sl->left - sr->left;
}

int main()
{
    int i;
    int j;
    sensor_t *s;
    point_t sens;
    point_t beac;
    int row;
    span_t *prev;
    span_t *curr;
    int src;
    int dst;

    nsensors =0;
    while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sens.x, &sens.y, &beac.x, &beac.y) == 4) {
        sensors[nsensors].loc = sens;
        sensors[nsensors].range = abs(sens.x - beac.x) + abs(sens.y - beac.y);
        nsensors++;
    }

    for (row = 0; row < 4000000; row++) {
        nspans = 0;
        for (i = 0; i < nsensors; i++) {
            s = &sensors[i];

            if (abs(s->loc.y - row) > s->range)
                continue;

            spans[nspans].left = s->loc.x - (s->range - abs(s->loc.y - row)); 
            spans[nspans].right = s->loc.x + (s->range - abs(s->loc.y - row)) + 1;
            
            nspans++;
        }

        qsort(spans, nspans, sizeof(span_t), spancomp);

        prev = &spans[0];

        for (i = 1; i < nspans; i++) {
            curr = &spans[i];

            if (curr->right <= prev->right)
                continue;
            
            if (curr->left > prev->right) {
                long freq = prev->right;
                freq *= 4000000;
                freq += row;
                printf("%ld\n", freq);
                return 0;
            }
            
            prev = curr;
        }
    }
}