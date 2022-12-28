#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100

typedef struct monkey_t monkey_t;
struct monkey_t {
    char op;                // + or *
    int operand;            // old +/* operand or old +/ old if operand == -1
    int testdiv;            // divisor for test of who to throw to
    int trueto;
    int falseto;
    int nitems;
    unsigned long items[MAX_ITEMS];
    int inspections;
};

static void readmonks(void);

static monkey_t monkeys[10];
static int nmonk;

int main()
{
    int round;
    monkey_t *mon;
    int item;
    unsigned long worry;
    int opnd;
    int to;
    monkey_t *top[2];
    int i;

    readmonks();

    for (round = 0; round < 20; round++) {
        for (mon = monkeys; mon < monkeys + nmonk; mon++) {
            mon->inspections += mon->nitems;
            for (item = 0; item < mon->nitems; item++) {
                worry = mon->items[item];
                opnd = mon->operand == -1 ? worry : mon->operand;
                if (mon->op == '+')
                    worry += opnd;
                else
                    worry *= opnd;
                worry /= 3;
                to = (worry % mon->testdiv) == 0 ? mon->trueto : mon->falseto;
                monkeys[to].items[monkeys[to].nitems++] = worry;
            }
            mon->nitems = 0;
        }
    }

    for (i = 0; i < 2; i++) {
        top[i] = monkeys;
        for (mon = monkeys; mon < monkeys + nmonk; mon++) {
            if (mon->inspections > top[i]->inspections)
                top[i] = mon;
        }

        top[i]->inspections = -top[i]->inspections;
    }

    printf("%d\n", top[0]->inspections*top[1]->inspections);
}

void readline(char *line, size_t size) 
{
    if (!fgets(line, size, stdin)) {
        printf("premature eof\n");
        exit(1);
    }
}

void readmonks(void)
{
    char line[100];
    monkey_t *mon = monkeys;
    char *p;
    
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] != 'M') {
            // Monkey 0:
            continue;
        }

        //  Starting items: 64, 55
        readline(line, sizeof(line));
        p = strchr(line, ':') + 1;
        while (true) {
            sscanf(p, "%lu", &mon->items[mon->nitems++]);
            p = strchr(p, ',');
            if (!p) {
                break;
            }
            p++;
        }

        // Operation: new = old * 7
        readline(line, sizeof(line));
        p = strstr(line, "old") + 4;
        mon->op = *p;
        p += 2;

        if (*p == 'o')  // "old"
            mon->operand = -1;
        else
            sscanf(p, "%d", &mon->operand);

        //  Test: divisible by 13
        readline(line, sizeof(line));
        p = strstr(line, "by") + 3;
        sscanf(p, "%d", &mon->testdiv);

        //  If true: throw to monkey 1
        //  If false: throw to monkey 3
        readline(line, sizeof(line));
        p = strstr(line, "monkey");
        sscanf(p+ 7, "%d", &mon->trueto);

        readline(line, sizeof(line));
        p = strstr(line, "monkey");
        sscanf(p+ 7, "%d", &mon->falseto);

        mon++;
        nmonk++;
    }
}
