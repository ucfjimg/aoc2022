#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char maze[201][201];
static int lens[201];
static int lines;
static char dir[10000];

enum op_t {
    MIN,
    MAX,
    X,
    Y,
    INVX,
    INVY,
};

// facing
enum facing_t {
    RIGHT = 0,
    DOWN = 1,
    LEFT = 2,
    UP = 3,
};

struct move_t {
    int side;
    enum op_t xop;
    enum op_t yop;
    enum facing_t facing;
};

struct side_t {
    struct move_t moves[4];    // by facing
};

// by facing
//
static int dxs[4] = { 1, 0, -1,  0 };
static int dys[4] = { 0, 1,  0, -1 };

#ifdef TEST
const int SIZE = 4;

               //   0    1    2    3    4    5
int xbase[6] = {    8,   0,   4,   8,   8,  12 };
int ybase[6] = {    0,   4,   4,   4,   8,   8 };

static struct side_t sides[6] = {
    {{ // 0
        /*RIGHT*/ { 5, MAX,  INVY,   LEFT },
        /*DOWN*/  { 3, X,    MIN,    DOWN },
        /*LEFT*/  { 2, Y,    MAX,    DOWN },
        /*UP*/    { 1, INVX, MIN,    DOWN },
    }},
    {{ // 1
        /*RIGHT*/ { 2, MIN,  Y,     RIGHT },
        /*DOWN*/  { 4, INVX, MAX,   UP },
        /*LEFT*/  { 5, INVY, MAX,   UP },
        /*UP*/    { 0, INVX, MIN,   DOWN },
    }},
    {{ // 2
        /*RIGHT*/ { 3, MIN,  Y,      RIGHT },
        /*DOWN*/  { 4, MIN,  INVX,   RIGHT },
        /*LEFT*/  { 1, MAX,  Y,      LEFT },
        /*UP*/    { 0, MIN,  X,      RIGHT },
    }},
    {{ // 3
        /*RIGHT*/ { 5, INVY, MIN,    DOWN },
        /*DOWN*/  { 4, X,    MIN,    DOWN },
        /*LEFT*/  { 2, MAX,  Y,      LEFT },
        /*UP*/    { 0, MIN,  X,      RIGHT },
    }},
    {{ // 4
        /*RIGHT*/ { 5, MIN,  Y,      RIGHT },
        /*DOWN*/  { 1, INVX, MAX,    UP },
        /*LEFT*/  { 2, INVY, MAX,    UP },
        /*UP*/    { 3, X,    MAX,    UP },
    }},
    {{ // 5
        /*RIGHT*/ { 0, MAX, INVY,    LEFT },
        /*DOWN*/  { 1, MIN, INVX,    RIGHT },
        /*LEFT*/  { 4, MAX, Y,       LEFT },
        /*UP*/    { 3, MAX, INVX,    LEFT },
    }},
};
#else
const int SIZE = 50;
               //   0    1    2    3    4    5
int xbase[6] = {   50, 100,  50,   0,  50,   0 };
int ybase[6] = {    0,   0,  50, 100, 100, 150 };

static struct side_t sides[6] = {
    {{ // 0
        /*RIGHT*/ { 1, MIN,  Y,     RIGHT },
        /*DOWN*/  { 2, X,    MIN,   DOWN },
        /*LEFT*/  { 3, MIN,  INVY,  RIGHT },
        /*UP*/    { 5, MIN,  X,     RIGHT },
    }},
    {{ // 1
        /*RIGHT*/ { 4, MAX,  INVY,  LEFT },
        /*DOWN*/  { 2, MAX,  X,     LEFT },
        /*LEFT*/  { 0, MAX,  Y,     LEFT },
        /*UP*/    { 5, X,    MAX,   UP },
    }},
    {{ // 2
        /*RIGHT*/ { 1, Y,   MAX,    UP },
        /*DOWN*/  { 4, X,   MIN,    DOWN },
        /*LEFT*/  { 3, Y,   MIN,    DOWN },
        /*UP*/    { 0, X,   MAX,    UP },
    }},
    {{ // 3
        /*RIGHT*/ { 4, MIN, Y,      RIGHT },
        /*DOWN*/  { 5, X,   MIN,    DOWN },
        /*LEFT*/  { 0, MIN, INVY,   RIGHT },
        /*UP*/    { 2, MIN, X,      RIGHT },
    }},
    {{ // 4
        /*RIGHT*/ { 1, MAX, INVY,   LEFT },
        /*DOWN*/  { 5, MAX, X,      LEFT },
        /*LEFT*/  { 3, MAX, Y,      LEFT },
        /*UP*/    { 2, X,   MAX,    UP },
    }},
    {{ // 5
        /*RIGHT*/ { 4, Y,   MAX,    UP },
        /*DOWN*/  { 1, X,   MIN,    DOWN },
        /*LEFT*/  { 0, Y,   MIN,    DOWN },
        /*UP*/    { 3, X,   MAX,    UP },
    }},
};
#endif

int sideno(int x, int y)
{
    for (int i = 0; i < 6; i++) {
        if (x >= xbase[i] && x < xbase[i] + SIZE && y >= ybase[i] && y < ybase[i] + SIZE)
            return i;
    }

    return -1;
}

int main()
{
    while (fgets(maze[lines], 201, stdin)) {
        int len = strlen(maze[lines]) - 1;
        while (len >= 0 && maze[lines][len] == '\n')
            len--;
        
        maze[lines][++len] = '\0';
        if (len == 0)
            break;
        
        lens[lines] = len;
        
        lines++;
    }

    if (scanf("%s\n", dir) != 1) {
        fprintf(stderr, "failed to read input\n");
        return 1;
    }

    int facing = RIGHT;
    int x = 0;
    int y = 0;
    while (maze[y][x] != '.')
        x++;

    char *pdir = dir;

    while (isdigit(*pdir)) {
        int delta = strtol(pdir, &pdir, 10);

        while (delta--) {
            int side = sideno(x, y);
            int dx = dxs[facing];
            int dy = dys[facing];

            int nx = x + dx;
            int ny = y + dy;
            int nfacing = facing;

            int stop = 0;

            if (sideno(nx, ny) != side) {
                nx = x - xbase[side];
                ny = y - ybase[side];

                const struct move_t *mv = &sides[side].moves[facing];

                switch (mv->xop) {
                    case MIN: nx = 0; break;
                    case MAX: nx = SIZE - 1; break;
                    case X: nx = x - xbase[side]; break;
                    case Y: nx = y - ybase[side]; break;
                    case INVX: nx = (SIZE - 1) - (x - xbase[side]); break;
                    case INVY: nx = (SIZE - 1) - (y - ybase[side]); break;
                    default:
                        stop++;
                }

                switch (mv->yop) {
                    case MIN: ny = 0; break;
                    case MAX: ny = SIZE - 1; break;
                    case X: ny = x - xbase[side]; break;
                    case Y: ny = y - ybase[side]; break;
                    case INVX: ny = (SIZE - 1) - (x - xbase[side]); break;
                    case INVY: ny = (SIZE - 1) - (y - ybase[side]); break;
                    default:
                        stop++;
                }

                side = mv->side;
                nfacing = mv->facing;

                nx += xbase[side];
                ny += ybase[side];
            }
            
            if (stop) {
                printf("exit side x %d y %d dx %d dy %d\n", x, y, dx, dy);
                return 1;
            }

            char ch = maze[ny][nx];
            if (ch == '#') {
                break;
            }
            if (ch != '.') {
                printf("oops! ch=%02x %d %d facing %d\n", ch, nx, ny, facing);
                return 1;
            }
            assert(ch == '.');
            x = nx;
            y = ny;
            facing = nfacing;
        }

        if (*pdir == 'R') {
            facing = (facing + 1) % 4;
        } else if (*pdir == 'L') {
            facing = (facing + 4 - 1) % 4;
        } else {
            break;
        }
        pdir++;
    }

    x++;
    y++;

    printf("%d\n", 1000 * y + 4 * x + facing);
}

