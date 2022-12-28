#include <stdio.h>
#include <string.h>

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
            
            for (int i = 0; i < crates; i++) {
                char top = stacks[from].crates[stacks[from].top - crates + i + 1];
                stacks[to].crates[stacks[to].top + i + 1] = top;
            }

            stacks[from].top -= crates;
            stacks[to].top += crates;

            stacks[from].crates[stacks[from].top + 1] = '\0';
            stacks[to].crates[stacks[to].top + 1] = '\0';
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
