#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct storm_t storm_t;
struct storm_t 
{
    int x;
    int y;
    int dx;
    int dy;
};

static storm_t storms[4000];
static int nstorms;

static int rows;
static int cols;

static char map[40][110];
static char empty[40][110][4000];
static char seen[40][110][4000];

static int dxs[5] = { 0, -1,  1,  0,  0 };
static int dys[5] = { 0,  0,  0, -1,  1 };


typedef struct node_t node_t;
struct node_t {
    int x;
    int y;
};

typedef struct queuenode_t queuenode_t;
struct queuenode_t {
    queuenode_t *next;
    node_t node;
    int time;
};

typedef struct queue_t queue_t;
struct queue_t {
    queuenode_t *head;
    queuenode_t *tail;
    queuenode_t *free;
};

static queue_t work;


void enqueue(queue_t *q, node_t *node, int time)
{
    queuenode_t *qn;

    if (q->free) {
        qn = q->free;
        q->free = qn->next;
    } else {
        qn = malloc(sizeof(queuenode_t));
    }

    if (q->tail) {
        q->tail->next = qn;
    } else {
        q->head = qn;
    }
    q->tail = qn;
    qn->next = NULL;
    qn->node = *node;
    qn->time = time;
}

bool dequeue(queue_t *q, node_t *node, int *time)
{
    queuenode_t *qn = q->head;

    if (qn == NULL)
        return false;

    *node = qn->node;
    *time = qn->time;

    q->head = qn->next;
    if (q->head == NULL)
        q->tail = NULL;

    qn->next = q->free;
    q->free = qn;
    return true;
}

void clearqueue(queue_t *q)
{
    if (q->tail) {
        q->tail->next = q->free;
        q->free = q->head;
    }
    q->head = NULL;
    q->tail = NULL;
}

void build_map(void)
{
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < cols - 1; x++) {
            map[y][x] = '.';
        }
    }

    for (int i = 0; i < nstorms; i++) {
        storm_t *s = &storms[i];
        if (map[s->y][s->x] == '.') {
            if (s->dx == -1) {
                map[s->y][s->x] = '<';
            } else if (s->dx == 1) {
                map[s->y][s->x] = '>';
            } else if (s->dy == -1) {
                map[s->y][s->x] = '^';
            } else if (s->dy == 1) {
                map[s->y][s->x] = 'v';
            } else {
                map[s->y][s->x] = '?';
            }
        } else {
            map[s->y][s->x] = '+';
        }
    }
}

void print_map(void)
{
    for (int y = 0; y < rows; y++) {
        printf("%s\n", map[y]);
    }
}

int solve(int sx, int sy, int ex, int ey, int time)
{
    node_t node;

    clearqueue(&work);
    memset(seen, 0, sizeof(seen));


    int xmod = cols - 2;
    int ymod = rows - 2;
    int cycle = xmod * ymod;



    node.x = sx;
    node.y = sy;
    enqueue(&work, &node, time);

    while (dequeue(&work, &node, &time)) {
        if (node.x == ex && node.y == ey) {
            return time;
        }

        int nt = (time + 1) % cycle;

        if (seen[node.y][node.x][nt])
            continue;
        seen[node.y][node.x][nt] = 1;

        for (int i = 0; i < 5; i++) {
            int nx = node.x + dxs[i];
            int ny = node.y + dys[i];
            
            bool inmaze = !(nx < 0 || ny < 0 || nx > cols-1 || ny > rows-1); 
            if (!inmaze)
                continue;
            
            if (!empty[ny][nx][nt]) {
                continue;
            }
            
            node_t next = { nx, ny };
            enqueue(&work, &next, time + 1);
        }
    }
    printf("-1\n");
    return -1;
}

int main()
{
    char line[200];

    int y = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        for (int x = 0; line[x] && line[x] != '\n'; x++) {
            map[y][x] = line[x];
            switch (line[x]) {
                case '<':                 
                    storms[nstorms].x = x;
                    storms[nstorms].y = y;
                    storms[nstorms].dx = -1;
                    storms[nstorms].dy = 0;
                    nstorms++;
                    break;
                case '>':
                    storms[nstorms].x = x;
                    storms[nstorms].y = y;
                    storms[nstorms].dx = 1;
                    storms[nstorms].dy = 0;
                    nstorms++;
                    break;
                case 'v':
                    storms[nstorms].x = x;
                    storms[nstorms].y = y;
                    storms[nstorms].dx = 0;
                    storms[nstorms].dy = 1;
                    nstorms++;
                    break;
                case '^':
                    storms[nstorms].x = x;
                    storms[nstorms].y = y;
                    storms[nstorms].dx = 0;
                    storms[nstorms].dy = -1;
                    nstorms++;
                    break;
            }

            if (x > cols)
                cols = x;
        }
        y++;
    }
    cols++;
    rows = y;

    int xmod = cols - 2;
    int ymod = rows - 2;
    int cycle = xmod * ymod;
    
    for (int m = 0; m <= xmod * ymod; m++) {
        build_map();

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                empty[y][x][m] = map[y][x] == '.';
            }
        }

        for (int i = 0; i < nstorms; i++) {
            storm_t *s = &storms[i];
            s->x = 1 + (s->x - 1 + s->dx + 3 * xmod) % xmod; 
            s->y = 1 + (s->y - 1 + s->dy + 3 * ymod) % ymod; 
        }
    }

    int time = 0;
    time = solve(1, 0, cols-2, rows-1, time);
    time = solve(cols-2, rows-1, 1, 0, time);
    time = solve(1, 0, cols-2, rows-1, time);
    printf("%d\n", time);
}
