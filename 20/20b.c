#include <assert.h>
#include <stdio.h>
#include <string.h>

int main()
{
    const long key = 811589153;

    int numbers[5000];
    int index[5000];
    int nnumbers = 0;

    while (scanf("%d", &numbers[nnumbers]) == 1) { 
        index[nnumbers] = nnumbers;
        nnumbers++;
    }
    
    long mod = key % (nnumbers - 1); 

    for (int l = 0; l < 10; l++) {
        for (int i = 0; i < nnumbers; i++) {
            int pos = -1;
            for (int j = 0; j < nnumbers; j++) {
                if (index[j] == i) {
                    pos = j;
                    break;
                }
            }

            assert(pos != -1);
            int newpos = (pos + mod * numbers[i] + 2 * mod * (nnumbers - 1)) % (nnumbers - 1);
            assert(newpos >= 0);

            for (int j = pos; j < nnumbers - 1; j++)
                index[j] = index[j+1];
            
            for (int j = nnumbers - 1; j > newpos; j--) 
                index[j] = index[j-1];
            
            index[newpos] = i;
        
        }
    }

    int newnum[5000];

    int zero = -1;
    for (int i = 0; i < nnumbers; i++) {
        newnum[i] = numbers[index[i]];
        if (newnum[i] == 0) {
            zero = i; 
        }
    }

    printf("%ld\n",
        key * (newnum[(zero + 1000) % nnumbers] +
               newnum[(zero + 2000) % nnumbers] +
               newnum[(zero + 3000) % nnumbers])
    );
}