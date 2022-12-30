#include <stdio.h>
#include <string.h>

// 5a - the input is an initial state of a list of stacks
//      of crates, followed by moves which take the top
//      `n` crates from one stack and move them to 
//      a new stack. The crates are moved one at a time,
//      so their order will be reversed on the top of the 
//      new stack. The answer is the letters of the crates
//      on the top of each stack after all the moving.
//
// idea: straight simulation; keep an array of stacks 
//       and move crates between them.
//
// https://adventofcode.com/2022/day/5
//

struct stack_t {
    int top;
    char crates[102];
};

int main()
{
    char line[200];
    struct stack_t stacks[10];

    for (int i = 0; i < 10; i++) {
        stacks[i].top = -1;
        memset(stacks[i].crates, '\0', 102);
    }

    while (1) {
        if (!fgets(line, sizeof(line), stdin)) {
            printf("premature eof");
            return 1;
        }

        if (line[1] == '1') {
            break;
        }

        int l = strlen(line);

        for (int i = 0; i < 10; i++) {
            int crate = 1 + 4 * i;
            if (crate >= l) {
                break;
            }

            if (line[crate] != ' ') {
                stacks[i].crates[++stacks[i].top] = line[crate];
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        char *p = stacks[i].crates;
        int low = 0;
        int high = stacks[i].top;

        for (; low < high; low++, high--) {
            char t = p[low];
            p[low] = p[high];
            p[high] = t;
        }
    }

    while (fgets(line, sizeof(line), stdin)) {
        int crates, from, to;
        if (sscanf(line, "move %d from %d to %d", &crates, &from, &to) == 3) {
            from--, to--;
            while (crates--) {
                char top = stacks[from].crates[stacks[from].top];
                stacks[from].crates[stacks[from].top--] = '\0';
                stacks[to].crates[++stacks[to].top] = top;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        int top = stacks[i].top;
        if (top >= 0) {
            printf("%c", stacks[i].crates[top]);
        }
    }
    printf("\n");
}
