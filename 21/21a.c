#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct expr_t expr_t;
struct expr_t {
    char name[5];
    char op;
    char varleft[5];
    int left;
    char varright[5];
    int right;
    int hasval;
    long val;
};

static expr_t exprs[2000];
int nexpr;

int lookup_name(char *name)
{
    for (int i = 0; i < nexpr; i++)
        if (strcmp(exprs[i].name, name) == 0)
            return i;
    return -1;
}

int read() 
{
    char line[100];
    int root = -1;

    while (fgets(line, sizeof(line), stdin)) {
        strncpy(exprs[nexpr].name, line, 4);
        if (strcmp(exprs[nexpr].name, "root") == 0)
            root = nexpr;

        char *p = line + 6;
        if (isdigit(*p)) {
            exprs[nexpr].op = '#';
            exprs[nexpr].val = strtol(p, NULL, 10);
            exprs[nexpr].hasval = 1;
        } else {
            strncpy(exprs[nexpr].varleft, p, 4);
            exprs[nexpr].op = p[5];
            strncpy(exprs[nexpr].varright, p+7, 4);
        }

        nexpr++;
    }

    for (int i = 0; i < nexpr; i++) {
        if (exprs[i].op == '#')
            continue;
        
        if ((exprs[i].left = lookup_name(exprs[i].varleft)) == -1) {
            printf("could't find %s\n", exprs[i].varleft);
            return -1;
        }

        if ((exprs[i].right = lookup_name(exprs[i].varright)) == -1) {
            printf("could't find %s\n", exprs[i].varright);
            return -1;
        }
    }

    return root;
}

long eval(int node)
{
    expr_t *ex = &exprs[node];
    if (ex->hasval)
        return ex->val;

    expr_t *left = &exprs[ex->left];
    expr_t *right = &exprs[ex->right];

    if (!left->hasval) {
        left->val = eval(ex->left);
        left->hasval = 1;
    }

    if (!right->hasval) {
        right->val = eval(ex->right);
        right->hasval = 1;
    }

    switch (ex->op) {
        case '+':
            ex->val = left->val + right->val;
            break;

        case '-':
            ex->val = left->val - right->val;
            break;
            
        case '*':
            ex->val = left->val * right->val;
            break;
            
        case '/':
            ex->val = left->val / right->val;
            break;
            
        default:
            printf("operator '%c'??\n", ex->op);
    }

    ex->hasval = 1;
    return ex->val;
}

int main()
{
    int root = read();
    if (root == -1)
        return 1;
    printf("%ld\n", eval(root));
}