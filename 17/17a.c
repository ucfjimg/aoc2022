#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
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
    int ch;
    int height = 0;
    int i;
    int move = 0;
    int inptr = 0;
    int pieceno = 0;
    const piece_t *piece = &pieces[pieceno];
    pt_t piece_origin = {2, 3};
    pt_t placed;
    bool resting;
    static char line[11000];

    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "failed to read input.\n");
        return 1;
    }

    i = strlen(line);
    while (isspace(line[i-1]))
        i--;
    line[i] = '\0';

    while (move < 2022) {
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
            if (placed.y + 1 > height) {
                height = placed.y + 1;
            }
        }

        pieceno = (pieceno + 1) % 5;
        piece = &pieces[pieceno];
        piece_origin.x = 2;
        piece_origin.y = height + 3;
    }

    printf("%d\n", height);
}