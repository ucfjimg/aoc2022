#include <assert.h>
#include <stdio.h>
#include <string.h>

// 20a - the input is a list of numbers. we are to process the numbers
//       in rounds; in each round we move the numbers forward or 
//       back based in the list based on their value. we need to
//       compute to sum of the numbers in the 1000, 2000, and
//       3000 positions past zero after one round.
//
// idea: just move the numbers around in a an array. the only wrinkle
//       is that the numbers are processed from their original positions
//       (i.e. if we have `x y z` and moving x gives us `y x z` we still
//       continue on to process y and z). so we actually move around 
//       indices to the numbers.
//
// https://adventofcode.com/2022/day/20
//

int main()
{
    int numbers[5000];
    int index[5000];
    int nnumbers = 0;

    while (scanf("%d", &numbers[nnumbers]) == 1) { 
        index[nnumbers] = nnumbers;
        nnumbers++;
    }

    for (int i = 0; i < nnumbers; i++) {
        int pos = -1;
        for (int j = 0; j < nnumbers; j++) {
            if (index[j] == i) {
                pos = j;
                break;
            }
        }

        assert(pos != -1);
        int newpos = (pos + numbers[i] + 3 * (nnumbers - 1)) % (nnumbers - 1);

        for (int j = pos; j < nnumbers - 1; j++)
            index[j] = index[j+1];
        
        for (int j = nnumbers - 1; j > newpos; j--) 
            index[j] = index[j-1];
        

        index[newpos] = i;
    }

    int newnum[5000];

    int zero = -1;
    for (int i = 0; i < nnumbers; i++) {
        newnum[i] = numbers[index[i]];
        if (newnum[i] == 0) {
            zero = i; 
            break;
        }
    }

    printf("%d\n",
        newnum[(zero + 1000) % nnumbers] +
        newnum[(zero + 2000) % nnumbers] +
        newnum[(zero + 3000) % nnumbers]
    );
}
