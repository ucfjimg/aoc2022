#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 13a - the input is a list of nested lists, one per line.
//       given a recursive definition of a comparison operation
//       for two lists, we are to count the number of pairs of 
//       lists in which the first list is less than the second
//       list.
//
// idea: since we're given a recursive definition for the 
//       comparison operator, we'll write a recursive list
//       comparison function.
//
// https://adventofcode.com/2022/day/13
//

typedef struct pair_t pair_t;
struct pair_t {
    char *left;
    char *right;
};

int compare(pair_t *p)
{
    int left, right;
    int comp;
    char *save, temp[20];

    if (*p->left != '[' || *p->right != '[') {
        fprintf(stderr, "inputs not lists '%s' '%s'\n", p->left, p->right);
        exit(1);
    }

    p->left++;
    p->right++;

    while (1) {
        // if the the lists are the same length, equal
        if (*p->left == ']' && *p->right == ']') {
            p->left++;
            p->right++;
            return 0;
        } else if (*p->left == ']' && *p->right != ']') {
            // if left list is shorter, left < right
            return -1;
        } else if (*p->left != ']' && *p->right == ']') {
            // if right list is shorter, left > right
            return 1;
        } else if (*p->left == ',' || *p->right == ',') {
            p->left++;
            p->right++;
        }

        // if the next item of both lists is another list, recurse
        //
        if (*p->left == '[' && *p->right == '[') {
            comp = compare(p);
            if (comp != 0)
                return comp;
        } else if (isdigit(*p->left) && isdigit(*p->right)) {
            left = strtol(p->left, &p->left, 10);
            right = strtol(p->right, &p->right, 10);
            if (left != right)
                return left - right;
        } else if (*p->left == '[') {
            // left is a list, right is an integer
            //
            right = strtol(p->right, &p->right, 10);
            save = p->right;
            sprintf(temp, "[%d]", right);
            p->right = temp;
            comp = compare(p);
            p->right = save;
            if (comp != 0)
                return comp;
        } else if (*p->right == '[') {
            // right is a list, left is an integer
            //
            left = strtol(p->left, &p->left, 10);
            save = p->left;
            sprintf(temp, "[%d]", left);
            p->left = temp;
            comp = compare(p);
            p->left = save;
            if (comp != 0)
                return comp;
        } else {
            fprintf(stderr, "where am i? '%s' '%s'\n", p->left, p->right);
        }
    }
}

int main()
{
    char line1[512];
    char line2[512];
    pair_t pair;
    int index = 1;
    int sum = 0;

    while (1) {
        if (scanf("%s %s", line1, line2) != 2) {
            break;
        }

        pair.left = line1;
        pair.right = line2;
        int c = compare(&pair);

        if (c < 0) 
            sum += index;

        index++;
    }
    printf("%d\n", sum);
}
