#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef struct pt_t pt_t;
struct pt_t {
    int x;
    int y;
};

typedef struct elf_t elf_t;
struct elf_t {
    pt_t pos;
    pt_t proposed;
    int moving;
};

typedef struct hashnode_t hashnode_t;
struct hashnode_t {
    hashnode_t *next;
    pt_t pt;
    elf_t *elf;
};

static pt_t moves[4][3] = {
    {
        { -1, -1 },
        {  0, -1 },
        {  1, -1 }
    },
    {
        { -1,  1 },
        {  0,  1 },
        {  1,  1 }
    },
    {
        { -1, -1 },
        { -1,  0 },
        { -1,  1 }
    },
    {
        {  1, -1 },
        {  1,  0 },
        {  1,  1 }
    },
};

static elf_t elves[3000];
static int nelves;

static int first_move = 0;
#define HASHPRIME  19319

static hashnode_t *hashfreel = NULL;
static hashnode_t *poshash[HASHPRIME];
static hashnode_t *prophash[HASHPRIME];

static char map[1024][1024];

int pointhash(pt_t pt) 
{
    int x = abs(pt.x);
    int y = abs(pt.y);

    return x ^ (x >> 16) ^ y ^ (y >> 16);
}

hashnode_t *hashalloc(void) 
{
    if (hashfreel) {
        hashnode_t *p = hashfreel;
        hashfreel = p->next;
        return p;
    }

    return (hashnode_t*)malloc(sizeof(hashnode_t));
}

void hashfree(hashnode_t *p)
{
    p->next = hashfreel;
    hashfreel = p;
}

elf_t *hashfind(hashnode_t **table, pt_t loc) 
{
    int hash = pointhash(loc);
    int index = hash % HASHPRIME;

    for (hashnode_t *p = table[index]; p; p = p->next) {
        if (p->pt.x == loc.x && p->pt.y == loc.y) {
            return p->elf;
        }
    }

    return NULL;
}

void hashinsert(hashnode_t **table, pt_t loc, elf_t *elf)
{
    int hash = pointhash(loc);
    int index = hash % HASHPRIME;

    for (hashnode_t *p = table[index]; p; p = p->next)
        assert(!( p->pt.x == loc.x && p->pt.y == loc.y ));

    hashnode_t *p = hashalloc();

    p->pt = loc;
    p->elf = elf;
    p->next = table[index];
    table[index] = p;
}

void hashclear(hashnode_t **table)
{
    for (int i = 0; i < HASHPRIME; i++) {
        hashnode_t *head = table[i];
        if (head == NULL)
            continue;

        hashnode_t *tail = head;
        while (tail->next)
            tail = tail->next;

        tail->next = hashfreel;
        hashfreel = head;

        table[i] = NULL;
    }
}

void readelves(void)
{
    char line[1000];
    int y = 0;

    while (fgets(line, sizeof line, stdin)) {
        int l = strlen(line);
        if (line[l-1] == '\n')
            --l;
        for (int x = 0; x < l; x++) {
            if (line[x] == '#') {
                elf_t *elf = &elves[nelves++];
                elf->pos.x = x;
                elf->pos.y = y;

                hashinsert(poshash, elf->pos, elf);
            }
        }
        y++;
    }
}

int main()
{
    readelves();

    elf_t *end = &elves[nelves];

    for (int round = 1; round <= 10; round++) {
        // first half
        //
        for (elf_t *p = elves; p < end; p++) {
            p->moving = 0;

            int wantsmove = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {
                        continue;
                    }

                    pt_t check;
                    check.x = p->pos.x + i;
                    check.y = p->pos.y + j;
                    
                    if (hashfind(poshash, check)) {
                        wantsmove = 1;
                        break;
                    }
                }
            }

            if (!wantsmove) {
                continue;
            }

            int mvi = first_move;
            for (int i = 0; i < 4; i++, mvi = (mvi + 1) % 4) {
                int j;
                for (j = 0; j < 3; j++) {
                    pt_t check;
                    check.x = p->pos.x + moves[mvi][j].x;
                    check.y = p->pos.y + moves[mvi][j].y;

                    if (hashfind(poshash, check)) {
                        break;
                    }
                }

                if (j == 3) {
                    p->moving = 1;
                    p->proposed.x = p->pos.x + moves[mvi][1].x;
                    p->proposed.y = p->pos.y + moves[mvi][1].y;
                    break;
                }

                if (p->moving)
                    break;
            }
        }

        // second half: everyone moves that's allowed to
        //
        hashclear(prophash);
        for (elf_t *p = elves; p < end; p++) {
            if (!p->moving) {
                continue;
            }

            elf_t *other = hashfind(prophash, p->proposed);
            if (other == NULL) {
                hashinsert(prophash, p->proposed, p);
                continue;
            } 

            p->moving = 0;
            other->moving = 0;
        }

        // now move everyone
        //
        hashclear(poshash);
        for (elf_t *p = elves; p < end; p++) {
            if (p->moving) {
                p->pos = p->proposed;
            }
            hashinsert(poshash, p->pos, p);
        }

        first_move = (first_move + 1) % 4;
    }

    // figure out empty space
    //
    pt_t    min = elves[0].pos;
    pt_t    max = elves[0].pos;
    int     x, y;
    int     dx, dy;
    elf_t   *p;

    for (p = elves; p < end; p++) {
        min.x = MIN(min.x, p->pos.x);
        min.y = MIN(min.y, p->pos.y);
        max.x = MAX(max.x, p->pos.x);
        max.y = MAX(max.y, p->pos.y);
    }

    int total = (max.x - min.x + 1) * (max.y - min.y + 1);
    printf("%d\n", total - nelves);
}