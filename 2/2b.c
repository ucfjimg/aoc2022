#include <stdio.h>

// 2b - like 2a, except we're now told whether to win,
//      lose, or draw as opposed to the actual move.
//
// idea: simple simulation.
//
// https://adventofcode.com/2022/day/2
//
// A=Rock, B=Paper, C=Scissors
// X=Lost, Y=Draw, C=Win

int main(int argc, char *argv[])
{
    char left, right, move;

    int score = 0;
    while (scanf("%c %c\n", &left, &right) == 2) {
        left -= 'A';

        switch (right) {
            case 'X':       // I lose
                move = (left + 2) % 3;
                break;

            case 'Y':       // draw
                move = left;
                score += 3;
                break;

            case 'Z':       // I win
                move = (left + 1) % 3;
                score += 6;
                break;
        }

        score += 1 + move;
    }

    printf("%d\n", score);
}
