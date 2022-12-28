#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct dirnode_t dirnode_t;
struct dirnode_t {
    dirnode_t *sib;
    dirnode_t *child;
    dirnode_t *parent;
    char *name;
    unsigned long long bytes;
};

void trim(char *p)
{
    char *q = p + strlen(p) - 1;

    while (q >= p && isspace(*q))
        q--;

    q[1] = '\0';
}

char *split(char *p)
{
    char *q = strchr(p, ' ');
    if (q) {
        *q = '\0';
        return q+1;
    }

    return NULL;
}

dirnode_t *make_node(const char *name) 
{
    dirnode_t *node = malloc(sizeof(dirnode_t));

    node->sib = NULL;
    node->child = NULL;
    node->parent = NULL;
    node->name = strdup(name);
    node->bytes = 0;

    return node;
}

void print_cwd(dirnode_t *cwd)
{
    if (cwd->parent == cwd)
        return;

    print_cwd(cwd->parent);
    printf("/%s", cwd->name);
}

void print_tree(dirnode_t *cwd, char *prefix)
{
    char path[200];
    char *rootpath;
    dirnode_t *p;

    strcpy(path, prefix);
    strcat(path, "/");

    rootpath = path + strlen(path);

    printf("%15llu %s\n", cwd->bytes, path);

    for (p = cwd->child; p; p = p->sib) {
        strcpy(rootpath, p->name);
        print_tree(p, path);
    }
}

void bubble_up(dirnode_t *root)
{
    dirnode_t *p;

    for (p = root->child; p; p = p->sib) {
        bubble_up(p);
        root->bytes += p->bytes;
    }
}

unsigned long long small_files(dirnode_t *root)
{
    const int MAX = 100000;
    unsigned long long sum = 0;
    dirnode_t *p;

    if (root->bytes <= MAX) 
        sum += root->bytes;

    for (p = root->child; p; p = p->sib)
        sum += small_files(p);

    return sum;
}

dirnode_t *cd(dirnode_t *cwd, char *name)
{
    dirnode_t *p;

    if (strcmp(name, "..") == 0) {
        return cwd->parent;
    }

    for (p = cwd->child; p; p = p->sib) {
        if (strcmp(p->name, name) == 0) {
            return p;
        }
    }

    p = make_node(name);
    p->parent = cwd;
    p->sib = cwd->child;
    cwd->child = p;

    return p;
}

void ls(dirnode_t *cwd)
{
    char line[100];
    char *first;
    char *name;
    dirnode_t *p;

    while (1) {
        int ch = getchar();
        ungetc(ch, stdin);

        if (ch == '$' || ch == EOF) {
            return;
        }

        if (!fgets(line, sizeof(line), stdin)) {
            printf("premature eof\n");
            exit(1);
        }

        trim(line);

        first = line;
        name = split(first);

        if (strcmp(first, "dir") == 0) {
            p = make_node(name);
            p->parent = cwd;
            p->sib = cwd->child;
            cwd->child = p;
            continue;
        }

        cwd->bytes += strtoull(first, NULL, 10);
    } 
}

int main()
{
    char line[100];
    char *cmd;
    char *arg;
    dirnode_t *root = make_node("");
    dirnode_t *cwd = root;

    root->parent = root;

    while (fgets(line, sizeof(line), stdin)) {
        trim(line);
        if (line[0] == '$') {
            cmd = &line[2];
            arg = split(cmd);

            if (strcmp(cmd, "cd") == 0) {
                if (strcmp(arg, "/") == 0) {
                    cwd = root;
                } else {
                    cwd = cd(cwd, arg);
                }
                continue;
            } 

            if (strcmp(cmd, "ls") == 0) {
                ls(cwd);
                continue;
            }

            fprintf(stderr, "bad command `%s'\n", cmd);
        } else {
            fprintf(stderr, "unexpected command '%s'\n", line);
        }
    }

    bubble_up(root);
//  print_tree(root, "");

    printf("%llu\n", small_files(root));
}
