#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pt_t pt_t;
struct pt_t  
{
    int x, y;
};

typedef struct piece_t piece_t;
struct piece_t {
    pt_t offsets[5];
    int width;
    int size;
};

static piece_t pieces[5] = 
{
    { { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, 4, 4 },
    { { {1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2} }, 3, 5 },
    { { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} }, 3, 5 },
    { { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, 1, 4 },
    { { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, 2, 4 },
};

static char column[100000];

#define HASH_PRIME  181081

typedef struct hashent_t hashent_t;
struct hashent_t {
    hashent_t *next;
    uint64_t state;
    int height;
    int move;
};

static hashent_t *states[HASH_PRIME];

uint64_t build_state(int height, int piece, int inptr)
{
    int col; 
    int row;
    uint8_t mask;
    uint64_t st = 0;

    int bottom = height - 8;
    if (bottom < 0) {
        bottom = -1;
    }

    for (col = 0; col < 7; col++) {
        mask = 1 << col;

        for (row = height; row > bottom; row--) {
            if (column[row] & mask) {
                break;
            }
        }

        int hole = height - row;
        st = (st << 4) | hole;
    }

    st |= (piece << 28);
    st |= ((uint64_t)inptr) << 31;
    return st;
}

hashent_t *hash_get(int height, int piece, int inptr)
{
    uint64_t state = build_state(height, piece, inptr);
    uint64_t hash;
    hashent_t *ent;

    hash = state ^ (state >> 16) ^ (state >> 32);
    hash %= HASH_PRIME;

    for (ent = states[hash]; ent; ent = ent->next) {
        if (ent->state == state) {
            return ent;
        }
    }

    return NULL;
}

void hash_add(int height, int piece, int inptr, int move)
{
    uint64_t state = build_state(height, piece, inptr);
    uint64_t hash;
    hashent_t *ent;

    hash = state ^ (state >> 16) ^ (hash >> 32);
    hash %= HASH_PRIME;

    ent = malloc(sizeof(hashent_t));
    ent->next = states[hash];
    states[hash] = ent;
    ent->state = state;
    ent->height = height;
    ent->move = move;
}


void print(const piece_t *piece, pt_t piece_origin)
{
    int x;
    int i;
    bool printed;

    for (int y = piece_origin.y + 4; y >= 0; y--) {
        printf("%06d |", y);
        for (int x = 0; x < 7; x++) {
            printed = false;
            if (y >= piece_origin.y) {
                for (i = 0; i < piece->size; i++) {
                    if (x == piece_origin.x + piece->offsets[i].x && y == piece_origin.y + piece->offsets[i].y) {
                        putchar('@');
                        printed = true;
                        break;
                    }
                }
            }

            if (column[y] & (1 << x)) {
                putchar('#');
                printed = true;
            }

            if (!printed) {
                putchar('.');
            }
        }
        printf("|\n");
    }
    printf("       +-------+\n");
}

bool canmove(const piece_t *piece, int x, int y)
{
    int i;
    pt_t placed;

    if (y < 0 || x < 0 || x + piece->width > 7) {
        return false;
    }

    for (i = 0; i < piece->size; i++) {
        placed.x = x + piece->offsets[i].x;
        placed.y = y + piece->offsets[i].y;

        if (column[placed.y] & (1 << placed.x)) {
            return false;
        }
    }

    return true;
}

int main()
{
    static char line[11000];
    int i;
    int height = 0;
    int ch;
    int inptr = 0;
    int pieceno = 0;
    const piece_t *piece = &pieces[pieceno];
    pt_t piece_origin = {2, 3};
    int move = 0;
    pt_t placed;
    hashent_t *ent;

    if (!fgets(line, sizeof(line), stdin))
        return 1;
    i = strlen(line);
    while (isspace(line[i-1]))
        i--;
    line[i] = '\0';

    while (height < sizeof(column) - 10) {
        ch = line[inptr++];
        if (!line[inptr])
            inptr = 0;

        if (ch == '<' && canmove(piece, piece_origin.x - 1, piece_origin.y)) {
            piece_origin.x--;
        } else if (ch == '>' && canmove(piece, piece_origin.x + 1, piece_origin.y)) {
            piece_origin.x++;
        }

        if (canmove(piece, piece_origin.x, piece_origin.y - 1)) {
            piece_origin.y--;
            continue;
        }

        move++;
        for (i = 0; i < piece->size; i++) {
            placed.x = piece_origin.x + piece->offsets[i].x;
            placed.y = piece_origin.y + piece->offsets[i].y;

            column[placed.y] |= (1 << placed.x);
            if (placed.y > height) {
                height = placed.y;
            }
        }
        
        pieceno = (pieceno + 1) % 5;
        piece = &pieces[pieceno];
        piece_origin.x = 2;
        piece_origin.y = height + 4;

        ent = hash_get(height, pieceno, inptr);
        if (ent) {
            break;
        }
        hash_add(height, pieceno, inptr, move);
    }

    uint64_t moves_left = 1000000000000;
    uint64_t total_height;

    // non-repeating prefix
    //
    moves_left -= ent->move;
    total_height = ent->height;

    int cycle_moves = move - ent->move;
    int cycle_height = height - ent->height;

    uint64_t cycles = moves_left / cycle_moves;
    total_height += cycles * cycle_height;
    height = 9;
    memset(column, 0, sizeof(column));
    moves_left -= cycles * cycle_moves;
    move = moves_left;

    uint8_t mask = 1 << 6;
    uint64_t state = ent->state;
    while (mask) {
        column[height - (state & 0x0f)] |= mask;
        mask >>= 1;
        state >>= 4;
    }

    piece_origin.x = 2;
    piece_origin.y = 13;
    pieceno = state & 7;
    piece = &pieces[pieceno];
    state >>= 3;
    inptr = state;

    while (move > 0) {
        ch = line[inptr++];
        if (!line[inptr])
            inptr = 0;

        if (ch == '<' && canmove(piece, piece_origin.x - 1, piece_origin.y)) {
            piece_origin.x--;
        } else if (ch == '>' && canmove(piece, piece_origin.x + 1, piece_origin.y)) {
            piece_origin.x++;
        }

        if (canmove(piece, piece_origin.x, piece_origin.y - 1)) {
            piece_origin.y--;
            continue;
        }

        move--;
        for (i = 0; i < piece->size; i++) {
            placed.x = piece_origin.x + piece->offsets[i].x;
            placed.y = piece_origin.y + piece->offsets[i].y;

            column[placed.y] |= (1 << placed.x);
            if (placed.y > height) {
                height = placed.y;
            }
        }
        
        pieceno = (pieceno + 1) % 5;
        piece = &pieces[pieceno];
        piece_origin.x = 2;
        piece_origin.y = height + 4;
    }

    total_height += height - 9 + 1;
    printf("%lu\n", total_height);
}