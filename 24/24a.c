#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 24a - the input is a maze with obstacles; we are to find a path
//       from the start to the end. however, the obstacles are moving
//       in a fixed pattern: across the maze or up and down, wrapping
//       around the edges.
//
// idea: we BFS through the maze, but treat the maze as 3D with the 
//       third dimension being time. given the way the obstacles move,
//       we can compute the amount of time it takes for their pattern
//       to repeat, so time doesn't just infinitely repeat, there are 
//       a relatively small number of different mazes. in some cases,
//       all of our moves may be blocks, so staying in place is a required
//       valid move.
//
// https://adventofcode.com/2022/day/24
//

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

typedef struct node_t node_t;
struct node_t {
    int x;
    int y;
    int t;
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

typedef struct listnode_t listnode_t; 
struct listnode_t {
    listnode_t *next;
    node_t node;
};

#define HASHPRIME  19319
static listnode_t *hashfreel;
static listnode_t *expanded[HASHPRIME];
static queue_t work;

unsigned hashnode(node_t *node) 
{
    unsigned n = node->t;
    n *= 105;
    n += node->x;
    n *= 40;
    n += node->y;
    return (unsigned)(n ^ (n >> 16));
}

listnode_t *hashalloc(void) 
{
    if (hashfreel) {
        listnode_t *p = hashfreel;
        hashfreel = p->next;
        return p;
    }

    return (listnode_t*)malloc(sizeof(listnode_t));
}

void hashfree(listnode_t *p)
{
    p->next = hashfreel;
    hashfreel = p;
}

bool hashfind(listnode_t **table, node_t *node) 
{
    int hash = hashnode(node);
    int index = hash % HASHPRIME;

    for (listnode_t *p = table[index]; p; p = p->next) {
        if (p->node.x == node->x && p->node.y == p->node.y && p->node.t == node->t) {
            return true;
        }
    }

    return false;
}

void hashremove(listnode_t **table, node_t *node)
{
    int hash = hashnode(node);
    int index = hash % HASHPRIME;

    listnode_t *prev = NULL;
    listnode_t *curr = table[index];

    while (curr && curr->node.x != node->x && curr->node.y != node->y && curr->node.t != node->t)
        prev = curr, curr = curr->next;

    if (!curr)
        return;

    if (prev)
        prev->next = curr->next;
    else
        table[index] = curr->next;

    hashfree(curr);
}

void hashinsert(listnode_t **table, node_t *node)
{
    int hash = hashnode(node);
    int index = hash % HASHPRIME;

    listnode_t *p = hashalloc();

    p->node = *node;
    p->next = table[index];
    table[index] = p;
}

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

    // we really want to start at 1,1 which avoids a bunch of special cases
    //
    for (int t = 1; t < cycle; t++) {
        if (empty[1][1][t]) {
            node_t node = { 1, 1, t };
            enqueue(&work, &node, t);
            break;
        }
    }
    
    node_t node;
    int time;
    while (dequeue(&work, &node, &time)) {
        if (node.x == cols - 2 && node.y == rows - 2) {
            printf("%u\n", time + 1);
            break;
        }

        if (seen[node.y][node.x][node.t])
            continue;
        seen[node.y][node.x][node.t] = 1;


        static int dxs[5] = { 0, -1,  1,  0,  0 };
        static int dys[5] = { 0,  0,  0, -1,  1 };

        int nt = (node.t + 1) % cycle;
        for (int i = 0; i < 5; i++) {
            int nx = node.x + dxs[i];
            int ny = node.y + dys[i];
            
            if (nx < 1 || ny < 1 || nx > cols-1 || ny > rows-1)
                continue;
            
            if (!empty[ny][nx][nt]) {
                continue;
            }
            
            node_t next = { nx, ny, nt };
            enqueue(&work, &next, time + 1);
        }
    }
}
