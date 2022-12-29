#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct expr_t expr_t;
struct expr_t {
    char name[5];
    char op;
    char varleft[5];
    expr_t *left;
    char varright[5];
    expr_t *right;
    expr_t *parent;
    int hasval;
    long val;
};

typedef struct nodes_t nodes_t;
struct nodes_t {
    expr_t *root;
    expr_t *humn;
};

static expr_t exprs[2000];
static nodes_t nodes;
int nexpr;

expr_t *lookup_name(char *name)
{
    for (int i = 0; i < nexpr; i++)
        if (strcmp(exprs[i].name, name) == 0)
            return &exprs[i];
    return NULL;
}

void read() 
{
    char line[100];
    int root = -1;

    while (fgets(line, sizeof(line), stdin)) {
        strncpy(exprs[nexpr].name, line, 4);
        if (strcmp(exprs[nexpr].name, "root") == 0)
            nodes.root = &exprs[nexpr];

        int is_human = strcmp(exprs[nexpr].name, "humn") == 0;        
        if (is_human) 
            nodes.humn = &exprs[nexpr];
        
        char *p = line + 6;
        if (isdigit(*p)) {
            if (!is_human) {
                exprs[nexpr].op = '#';
                exprs[nexpr].val = strtol(p, NULL, 10);
                exprs[nexpr].hasval = 1;
            } else {
                exprs[nexpr].op = '?';
            }
        } else {
            strncpy(exprs[nexpr].varleft, p, 4);
            exprs[nexpr].op = p[5];
            strncpy(exprs[nexpr].varright, p+7, 4);
        }

        nexpr++;
    }

    for (int i = 0; i < nexpr; i++) {
        expr_t *ex = &exprs[i];

        if (ex->op == '#' || ex->op == '?')
            continue;
        
        if ((ex->left = lookup_name(ex->varleft)) == NULL) {
            printf("could't find %s\n", ex->varleft);
            return;
        }
        ex->left->parent = ex;

        if ((ex->right = lookup_name(ex->varright)) == NULL) {
            printf("could't find %s\n", ex->varright);
            return;
        }
        ex->right->parent = ex;
    }
}

int contains(expr_t *root, expr_t *search)
{
    if (root == search)
        return 1;

    if (root->op == '#')
        return 0;

    return contains(root->left, search) || contains(root->right, search);
}

void eval(expr_t *ex)
{
    if (ex->hasval || ex->op == '?')
        return;

    expr_t *left = ex->left;
    expr_t *right = ex->right;

    if (!left->hasval) {
        eval(left);
    }

    if (!right->hasval) {
        eval(right);
    }

    if (!(left->hasval && right->hasval)) {
        return;
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
}

void inv_eval(expr_t *ex)
{
    if (ex->hasval) {
        return;
    }

    if (!ex->parent->hasval) {
        inv_eval(ex->parent);
    }

    expr_t *left = ex->parent->left;
    expr_t *right = ex->parent->right;

    assert(ex->parent->hasval);
    assert(left->hasval || right->hasval);

    switch (ex->parent->op) {
        case '+':
            if (!left->hasval)
                left->val = ex->parent->val - right->val;
            else
                right->val = ex->parent->val - left->val;
            break;

        case '-':
            if (!left->hasval)
                left->val = ex->parent->val + right->val;
            else
                right->val = left->val - ex->parent->val;
            break;

        case '*':
            if (!left->hasval)
                left->val = ex->parent->val / right->val;
            else                 
                right->val = ex->parent->val / left->val;
            break;

        case '/':
            if (!left->hasval)
                left->val = ex->parent->val * right->val;
            else
                right->val = left->val / ex->parent->val;
            break;

        default:
            printf("bad operator %c in inv_eval\n", ex->parent->op);
            return;
    }

    left->hasval = 1;
    right->hasval = 1;
}

int main()
{
    read();

    expr_t *fixed;
    expr_t *var;    

    if (contains(nodes.root->left, nodes.humn)) {
        var = nodes.root->left;
        fixed = nodes.root->right;
    } else {
        var = nodes.root->right;
        fixed = nodes.root->left;
    }

    eval(nodes.root);

    var->hasval = 1;
    var->val = fixed->val;
    
    inv_eval(nodes.humn);
    printf("%ld\n", nodes.humn->val);
}