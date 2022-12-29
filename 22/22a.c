#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char maze[201][201];
static int lens[201];
static int lines;
static char dir[10000];


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
        fprintf(stderr, "failed to read input.\n");
        return 1;
    }
    
    int facing = 0;
    int x = 0;
    int y = 0;
    while (maze[y][x] != '.')
        x++;

    char *pdir = dir;

    static int dxs[4] = { 1, 0, -1,  0 };
    static int dys[4] = { 0, 1,  0, -1 };

    while (isdigit(*pdir)) {
        int delta = strtol(pdir, &pdir, 10);
        int dx = dxs[facing];
        int dy = dys[facing];

        while (delta--) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx == -1)
                nx = lens[y] - 1;
            else if (nx == lens[y])
                nx = 0;
            else if (ny == -1) 
                ny = lines - 1;
            else if (ny == lines)
                ny = 0;

            char ch = maze[ny][nx];

            while (isspace(ch) || ch == '\0') {
                if (dx) {
                    nx += dx;
                    if (nx == -1)
                        nx = lens[y] - 1;
                    else if (nx == lens[y])
                        nx = 0;
                } else {
                    ny += dy;
                    if (ny == -1) 
                        ny = lines - 1;
                    else if (ny == lines)
                        ny = 0;
                }

                ch = maze[ny][nx];
            }

            if (ch == '#') {
                break;
            }

            x = nx;
            y = ny;
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
