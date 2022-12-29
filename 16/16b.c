#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME 26
#define MAX_NODES 50

//////////////////////////////////////////////////////////////////////////////
//
// original graph
//
typedef struct og_node_t og_node_t;
struct og_node_t
{
    char name[3];
    int edges[MAX_NODES];
    int nedges;
    int rate;
};

typedef struct og_graph_t og_graph_t;
struct og_graph_t 
{
    og_node_t nodes[MAX_NODES];
    int nnodes;
    int name_to_index[26 * 26];
};

static og_node_t orig_graph[MAX_NODES];
static int orig_nodes;

int og_find_node(og_graph_t *graph, char *name)
{
    int i = (name[0] - 'A') * 26 + name[1] - 'A';
    if (graph->name_to_index[i] == -1) {
        graph->name_to_index[i] = graph->nnodes;
        og_node_t *node = &graph->nodes[graph->nnodes++];
        node->name[0] = name[0];
        node->name[1] = name[1];
        node->name[2] = '\0';
        node->nedges = 0;
    }

    return graph->name_to_index[i];
}

void og_print_graph(og_graph_t *graph)
{
    for (int i = 0; i < graph->nnodes; i++) {
        og_node_t *node = &graph->nodes[i];
        printf("%s rate %d", node->name, node->rate);
        for (int j = 0; j < node->nedges; j++) {
            printf(" %s", graph->nodes[node->edges[j]].name);
        }
        printf("\n");
    }
}

void read_graph(og_graph_t *graph)
{
    char line[100];

    for (int i = 0; i < 26*26; i++) {
        graph->name_to_index[i] = -1;
    }
    graph->nnodes = 0;

    while (fgets(line, sizeof(line), stdin)) {
        if (*line != 'V') {
            break;
        }

        int from = og_find_node(graph, &line[6]);
        from = og_find_node(graph, &line[6]);
        og_node_t *node = &graph->nodes[from];

        node->rate = strtol(&line[23], NULL, 10);

        char *valve = strstr(line, "valve") + 5;
        if (*valve == 's') {
            valve++;
        }
        valve++;
        
        for (;;) {
            int to = og_find_node(graph, valve);
            node->edges[node->nedges++] = to;
            valve += 2;
            if (*valve != ',') {
                break;
            }
            valve += 2;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// extract compressed subgraph
//
typedef struct edge_t edge_t;
struct edge_t
{
    int to;
    int dist;
};

typedef struct node_t node_t;
struct node_t
{
    char name[3];
    int rate;
    int nedges;
    edge_t edges[MAX_NODES];
    bool is_open;
};

typedef struct graph_t graph_t;
struct graph_t
{
    node_t nodes[MAX_NODES];
    int nnodes;
    int oncount;
    int onmask;
};

void graph_print(const graph_t *graph)
{
    for (int i = 0; i < graph->nnodes; i++) {
        const node_t *node = &graph->nodes[i];

        printf("%s (%d) %s:", node->name, node->rate, node->is_open ? "open" : "closed");
        for (int j = 0; j < node->nedges; j++) {
            const edge_t *edge = &node->edges[j];
            printf(" %s (%d)", graph->nodes[edge->to].name, edge->dist);
        }
        printf("\n");
    }
}

void extract_subgraph(const og_graph_t *og, graph_t *graph, int includes[], int nincludes)
{
    int dist[MAX_NODES][MAX_NODES];
    int next[MAX_NODES][MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            dist[i][j] = 9999999;
            next[i][j] = -1;
        }
    }

    for (int i = 0; i < og->nnodes; i++) {
        const og_node_t *node = &og->nodes[i];
        for (int j = 0; j < node->nedges; j++) {
            dist[i][node->edges[j]] = 1;
            next[i][node->edges[j]] = node->edges[j];
        }
    }

    for (int i = 0; i < og->nnodes; i++) {
        dist[i][i] = 0;
        next[i][i] = i;
    }

    for (int k = 0; k < og->nnodes; k++) {
        for (int i = 0; i < og->nnodes; i++) {
            for (int j = 0; j < og->nnodes; j++) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    graph->nnodes = nincludes;
    int includemap[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        includemap[i] = -1;
    }

    for (int i = 0; i < nincludes; i++) {
        includemap[includes[i]] = i;
    }

    graph->oncount = 0;
    graph->onmask = 0;

    for (int i = 0; i < nincludes; i++) {
        int u = includes[i];

        // Copy over basics of node
        //
        node_t *node = &graph->nodes[i];
        const og_node_t *og_node = &og->nodes[u];

        strcpy(node->name, og_node->name);
        node->rate = og_node->rate;
        node->nedges = 0;
        node->is_open = false;

        int edgedist[MAX_NODES];
        for (int j = 0; j < MAX_NODES; j++) {
            edgedist[j] = 0;
        }

        for (int j = 0; j < nincludes; j++) {
            if (i == j) {
                continue;
            }

            int v = includes[j];
            int n = next[u][v];
            while (includemap[n] == -1) {
                n = next[n][v];
            }

            edgedist[includemap[n]] = dist[u][n];
        }

        node->nedges = 0;
        for (int i = 0; i < nincludes; i++) {
            if (edgedist[i]) {
                node->edges[node->nedges].to = i;
                node->edges[node->nedges].dist = edgedist[i]; 
                node->nedges++;
            }
        }
    }
}

int remain[30][65536];

void build_remain(graph_t *g)
{
    int max = 1 << g->nnodes;

    for (int i = 0; i < max; i++) {
        int rate = 0;
        for (int j = 0; j < g->nnodes; j++) {
            if ((i & (1 << j)) == 0) {
                rate += g->nodes[j].rate;
            }
        }

        for (int j = 0; j < TIME; j++) {
            remain[j][i] = (TIME - (j + 1)) * rate; 
        }
    }
}

void solve(graph_t *g, int from, int t, int sofar, int *best)
{
    node_t *fromnode = &g->nodes[from];

    // if we're out of time or all the valves are on, we're done
    //
    if (t >= TIME || g->oncount == g->nnodes) {
        if (*best < sofar) { 
            *best = sofar;
        }
        return;
    }

    if (sofar + remain[t][g->onmask] < *best) {
        return;
    }

    // we can turn this node on and recurse
    //
    if (!fromnode->is_open && t+1 < TIME) {
        int fromflow = sofar + (TIME - (t + 1)) * fromnode->rate;
        fromnode->is_open = true;
        g->oncount++;
        g->onmask |= (1 << from);

        for (int i = 0; i < fromnode->nedges; i++) {
            edge_t *e = &fromnode->edges[i];
            solve(g, e->to, t+1+e->dist, fromflow, best);
        }

        g->onmask &= ~(1 << from);
        g->oncount--;
        fromnode->is_open = false;
    }

    // or, we can just recurse
    //
    for (int i = 0; i < fromnode->nedges; i++) {
        edge_t *e = &fromnode->edges[i];
        solve(g, e->to, t+e->dist, sofar, best);
    }
}

int main()
{
    og_graph_t og_graph;
    read_graph(&og_graph);

    int startnode = og_find_node(&og_graph, "AA");
    int includes[MAX_NODES];
    int nincludes = 0;

    int nvalve = 0;
    int valves[MAX_NODES];

    for (int i = 0; i < og_graph.nnodes; i++) {
        if (og_graph.nodes[i].rate) {
            valves[nvalve++] = i;
        }
    }

    int max = 1 << nvalve;

    int *solves = malloc(max * sizeof(int));

    for (int i = 0; i < max; i++) {
        nincludes = 0;
        includes[nincludes++] = startnode;
        
        for (int j = 0; j < nvalve; j++) {
            if (i & (1 << j)) {
                includes[nincludes++] = valves[j];
            }
        }

        graph_t graph;
        extract_subgraph(&og_graph, &graph, includes, nincludes);
        build_remain(&graph);

        graph.nodes[0].is_open = true;
        graph.oncount = 1;
        graph.onmask = 1;

        solve(&graph, 0, 0, 0, &solves[i]);
    }

    int best = 0;
    for (int i = 0; i < max; i++) {
        int j = (~i) & (max - 1);
        int sol = solves[i] + solves[j];
        if (sol > best) {
            best = sol;
        }
    }

    printf("%d\n", best);
}