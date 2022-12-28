#include <stdio.h>
#include <string.h>

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
