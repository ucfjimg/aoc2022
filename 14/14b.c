#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 14b - the input is a list of vectors for drawing a 2D map.
//       the map is vertical and sand is falling in from the top;
//       it piles up on the described walls. we are to count how
//       much sand has to pile up before it starts falling through
//       bottom of the grid into empty space.
//
//       in part b, there is a floor at the bottom, and we have to
//       keep simulating until the top of the sand pile blocks the
//       hole the sand is falling through.
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

typedef struct arrlist_t arrlist_t;
struct arrlist_t {
    int cap;
    int size;
    int *array;
};

arrlist_t rows[1000];
static int maxy = 0;

int arr_init(arrlist_t *arr)
{
    arr->cap = 16;
    arr->size = 0;
    arr->array = malloc(arr->cap * sizeof(int));
}

int arr_push(arrlist_t *arr, int val)
{
    if (arr->size == arr->cap) {
        arr->cap *= 2;
        arr->array = realloc(arr->array, arr->cap * sizeof(int));
    }
    arr->array[arr->size++] = val;
}

int arr_has(arrlist_t *arr, int val) 
{
    int i;

    for (i = 0; i < arr->size; i++)
        if (arr->array[i] == val)
            return 1;
    return 0;
}

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

    for (i = 0; i < 1000; i++) {
        arr_init(&rows[i]);
    }

    while (fgets(line, sizeof(line), stdin)) {
        p = line;
        sscanf(p, "%d,%d", &prev.x, &prev.y);
        maxy = MAX(maxy, prev.y);

        while (1) {
            p = strchr(p, ' ');
            if (p == NULL) {
                break;
            }
            p += 4;
            sscanf(p, "%d,%d", &curr.x, &curr.y);
            if (prev.x == curr.x) {
                t0 = MIN(prev.y, curr.y);
                t1 = MAX(prev.y, curr.y);
                for (; t0 <= t1; t0++) {
                    arr_push(&rows[t0], prev.x);
                }
            } else {
                t0 = MIN(prev.x, curr.x);
                t1 = MAX(prev.x, curr.x);
                for (; t0 <= t1; t0++) {
                    arr_push(&rows[prev.y], t0);
                }
            }
            
            prev = curr;
            maxy = MAX(maxy, prev.y);            
        }
    }

    for (i = 0, done = 0; !done ; i++) {
        curr.x = 500;
        curr.y = 0;

        while (1) {
            if (curr.y + 1 < maxy+2) {                
                if (!arr_has(&rows[curr.y+1], curr.x)) {
                    curr.y++;
                    continue;
                }

                if (!arr_has(&rows[curr.y+1], curr.x-1)) {
                    curr.y++, curr.x--;
                    continue;
                }

                if (!arr_has(&rows[curr.y+1], curr.x+1)) {
                    curr.y++, curr.x++;
                    continue;
                }
            }

            arr_push(&rows[curr.y], curr.x);
            
            if (curr.x == 500 && curr.y == 0) {
                done = 1;
            }
            break;
        }
   }

   printf("%d\n", i);
}
