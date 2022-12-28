#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#define MAX(a,b)  ((a) > (b) ? (a) : (b))

typedef struct pt_t pt_t;
struct pt_t {
    int x;
    int y;
};

static char visited[300][300];

void minmax(pt_t pt, pt_t *min, pt_t *max)
{
    min->x = MIN(min->x, pt.x);
    min->y = MIN(min->y, pt.y);
    max->x = MAX(max->x, pt.x);
    max->y = MAX(max->y, pt.y);
}

void print(pt_t head, pt_t tail, pt_t min, pt_t max)
{
    int x, y;

    for (int y = max.y; y >= min.y; y--) {
        for (int x = min.x; x <= max.x; x++) {
            if (x == head.x && y == head.y) {
                printf("H");
            } else if (x == tail.x && y == tail.y) {
                printf("T");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void move_tail(pt_t head, pt_t *tail)
{
    // check if we don't need to move
    //
    if (abs(head.x - tail->x) < 2 && abs(head.y - tail->y) < 2)
        return;

    // same row/col cases
    //
    if (head.x == tail->x) {
        if (head.y < tail->y)
            tail->y--;
        else
            tail->y++;
        return;
    } 

    if (head.y == tail->y) {
        if (head.x < tail->x)
            tail->x--;
        else
            tail->x++;
        return;
    } 

    // diagonals
    //
    if (head.x > tail->x)
        tail->x++;
    else
        tail->x--;

    if (head.y > tail->y)
        tail->y++;
    else
        tail->y--;
}

int main()
{
    pt_t head = { 0, 0 };
    pt_t tail = { 0, 0 };
    pt_t min = { 0, 0 };
    pt_t max = { 0, 0 };
    char dir;
    int step;
    int dx, dy;
    int tailcount = 0;
    int vx, vy;

    while (scanf("%c %d\n", &dir, &step) == 2) {
        dx = dy = 0;
        switch (dir) {
            case 'R': dx = 1; break;
            case 'L': dx = -1; break;
            case 'U': dy = 1; break;
            case 'D': dy = -1; break;
        }

        while (step--) {
            head.x += dx;
            head.y += dy;
            move_tail(head, &tail);
            minmax(head, &min, &max);
            minmax(tail, &min, &max);

            vx = tail.x + 150;
            vy = tail.y + 150;

            assert(vx >= 0 && vx < 300); 
            assert(vy >= 0 && vy < 300); 

            if (!visited[vx][vy]) {
                tailcount++;
                visited[vx][vy]++;
            }
        }
    }
    printf("%d\n", tailcount);
}