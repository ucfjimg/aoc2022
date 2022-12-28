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
    pt_t rope[10];
    pt_t min = { 0, 0 };
    pt_t max = { 0, 0 };
    char dir;
    int i;
    int step;
    int dx, dy;
    int tailcount = 0;
    int vx, vy;

    for (i = 0; i < 10; i++) {
        rope[i].x = 0;
        rope[i].y = 0;
    }

    while (scanf("%c %d\n", &dir, &step) == 2) {
        dx = dy = 0;
        switch (dir) {
            case 'R': dx = 1; break;
            case 'L': dx = -1; break;
            case 'U': dy = 1; break;
            case 'D': dy = -1; break;
        }

        while (step--) {
            rope[0].x += dx;
            rope[0].y += dy;
            for (i = 1; i < 10; i++) 
                move_tail(rope[i-1], &rope[i]);
            
            vx = rope[9].x + 150;
            vy = rope[9].y + 150;

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