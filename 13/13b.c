#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int qscompare(const void *left, const void *right)
{
    pair_t pair;

    pair.left = (char*)*(char**)left;
    pair.right = (char*)*(char**)right;
    return compare(&pair);
}

int main()
{
    int cap;
    int i;
    int nlines;
    char **lines;
    char line[512];
    char *key0 = "[[2]]";
    char *key1 = "[[6]]";
    int ikey0 = -1;
    int ikey1 = -1;

    cap = 4;
    nlines = 0;
    lines = calloc(cap, sizeof(char*));

    lines[nlines++] = key0;
    lines[nlines++] = key1;

    while (1) {
        if (scanf("%s", line) != 1) {
            break;
        }

        if (nlines == cap) {
            cap *= 2;
            lines = realloc(lines, cap * sizeof(char*));
        }

        lines[nlines++] = strdup(line);
    }

    qsort(lines, nlines, sizeof(char*), &qscompare);

    for (int i = 0; i < nlines; i++) {
        if (lines[i] == key0)
            ikey0 = i+1;
        else if (lines[i] == key1)
            ikey1 = i+1;
    }

    printf("%d\n", ikey0 * ikey1);
}