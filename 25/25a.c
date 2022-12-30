#include <stdio.h>
#include <string.h>

// 25a - the input is a bunch of numbers in a version of base 5
//       where the representation contains negative as well as 
//       positive digit values. we must add up all of the given
//       numbers and convert the result back into the base 5
//       representation.
//
// idea: this is just base conversion, but the negative digit values
//       mean that when converting from another base, a negative
//       digit will require a carry into the next digit to account for 
//       the negative digits. for example, there is no 4 digit, so 4 is
//       represented as a -1 in the 1's place, and a 1 in the 5's place.
//
// https://adventofcode.com/2022/day/25
//

int main()
{
    char line[100];

    long sum = 0;
    while (fgets(line, sizeof(line), stdin)) {
        int l = strlen(line);
        if (l && line[l-1] == '\n')
            l--;
        if (!l)
            break;
        line[l--] = '\0';

        long result = 0;
        long pow = 1;

        for (; l >= 0; l--) {
            int digit = 0;
            switch (line[l]) {
                case '=': digit = -2; break;
                case '-': digit = -1; break;
                case '0': digit =  0; break;
                case '1': digit =  1; break;
                case '2': digit =  2; break;
                default:
                    printf("??? %c\n", line[l]);
            }

            result += pow * digit;
            pow *= 5;
        }
        sum += result;
    }

    char *p = &line[sizeof(line)];
    *--p = '\0';
    long carry = 0;
    long pow = 1;

    while (sum || carry) {
        sum += carry;
        carry = 0;

        long digit = sum % 5;
        sum /= 5;

        switch (digit) {
            case 0: *--p = '0'; break;
            case 1: *--p = '1'; break;
            case 2: *--p = '2'; break;
            case 3: *--p = '='; carry = 1; break;
            case 4: *--p = '-'; carry = 1; break;
        }
    }

    printf("%s\n", p);





}
