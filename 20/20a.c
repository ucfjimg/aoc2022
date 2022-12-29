#include <assert.h>
#include <stdio.h>
#include <string.h>

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