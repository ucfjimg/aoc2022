#include <stdio.h>
#include <string.h>

// 10a - the input is a set of nop and add instructions with
//       strict timing. nop takes one cycle; add takes two
//       cycles and updates a register (X). the output is
//       the cycle times X for cycles 20, 40, ... 240.
//
// idea: simulate the instructions and save the value of
//       X on every cycle. use that series of values to 
//       compute the answer.
//
// https://adventofcode.com/2022/day/10
//

int main()
{
    int clock = 1;
    int x = 1;
    int dx;
    char opcode[10];
    int values[250];
    int i;
    int sum;

    while (scanf("%s", opcode) == 1) {
        if (strcmp(opcode, "noop") == 0) {
            values[clock++] = x;
            continue;
        } else if (strcmp(opcode, "addx") == 0) {
            if (scanf("%d", &dx) != 1) {
                printf("couldn't read dx\n");
                return 1;
            }
            values[clock++] = x;
            values[clock++] = x;
            x += dx;
        }
    }

    sum = 0;
    for (int i = 20; i < 241; i += 40)
        sum += i * values[i];

    printf("%d\n", sum);
}
