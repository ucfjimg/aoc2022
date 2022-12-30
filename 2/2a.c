#include <stdio.h>

// 2a - simulate rock, paper, scissors based on the input
//      data, given simple scoring rules.
//
// idea: simple simulation.
//
// https://adventofcode.com/2022/day/2
//
// A=X=Rock, B=Y=Paper, C=Z=Scissors

int main(int argc, char *argv[])
{
    char left, right;

    int score = 0;
    while (scanf("%c %c\n", &left, &right) == 2) {
        left -= 'A';
        right -= 'X';

        score += 1 + right;

        if (left == right) {
            score += 3;
        } else if ((right + 2) % 3 == left) {
            score += 6;
        }
    }

    printf("%d\n", score);
}
