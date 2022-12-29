#include <stdio.h>
#include <stdlib.h>

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define TICKS 32

typedef struct blueprint_t blueprint_t;
struct blueprint_t {
    int num;
    int ore_ore;
    int clay_ore;
    int obs_ore;
    int obs_clay;
    int geo_ore;
    int geo_obs;
};

enum {
    Nothing,
    BuildOre,
    BuildClay,
    BuildObs,
    BuildGeo
};

typedef struct state_t state_t;
struct state_t {
    int ore_robots;
    int clay_robots;
    int obs_robots; 
    int geo_robots;
    int ore;
    int clay;
    int obs;
    int geodes;

    unsigned char tick[TICKS];
};

static blueprint_t blueprints[30];
int nblue;

void print(int tick, state_t *state)
{
    printf("tick %d ore_robots %d clay_robots %d obs_robots %d geo_robots %d ore %d clay %d obs %d geodes %d\n",
        tick,
        state->ore_robots,
        state->clay_robots,
        state->obs_robots, 
        state->geo_robots,
        state->ore,
        state->clay,
        state->obs,
        state->geodes
    );
}

void sim(int tick, blueprint_t *bp, state_t *state, int *best)
{
    state_t next;
    
    if (tick >= TICKS) {
        if (*best < state->geodes) {
            *best = state->geodes;
        }
        return;
    }

    int left = TICKS - tick;
    int left2;

    int maxg = state->geodes + left * state->geo_robots + (left) * (left - 1) / 2;
    if (maxg < *best) {
        return;
    }


    // possibly build new robots
    //
    
    // don't build anything
    //
    next = *state;
    next.ore += left * next.ore_robots;
    next.clay += left * next.clay_robots;
    next.obs += left * next.obs_robots;
    next.geodes += left * next.geo_robots;
    sim(tick + left, bp, &next, best);

    // build an ore robot next
    //
    left = (bp->ore_ore - state->ore + state->ore_robots - 1) / state->ore_robots;
    left = 1 + MAX(left, 0);
    if (tick + left <= TICKS) {
        next = *state;
        next.ore += left * next.ore_robots - bp->ore_ore;
        next.clay += left * next.clay_robots;
        next.obs += left * next.obs_robots;
        next.geodes += left * next.geo_robots;
        next.ore_robots++;
        sim(tick + left, bp, &next, best);
    }

    // build a clay robot next
    //
    left = (bp->clay_ore - state->ore + state->ore_robots - 1) / state->ore_robots;
    left = 1 + MAX(left, 0);
    if (tick + left <= TICKS) {
        next = *state;
        next.ore += left * next.ore_robots - bp->clay_ore;
        next.clay += left * next.clay_robots;
        next.obs += left * next.obs_robots;
        next.geodes += left * next.geo_robots;
        next.clay_robots++;
        sim(tick + left, bp, &next, best);
    }
    
    // build an obsidian robot next
    //
    if (state->clay_robots) {
        left = (bp->obs_ore - state->ore + state->ore_robots - 1) / state->ore_robots;
        left2 = (bp->obs_clay - state->clay + state->clay_robots - 1) / state->clay_robots;
        left = 1 + MAX(left, 0);
        left2 = 1 + MAX(left2, 0);
        left = MAX(left, left2);
        if (tick + left <= TICKS) {
            next = *state;
            next.ore += left * next.ore_robots - bp->obs_ore;
            next.clay += left * next.clay_robots - bp->obs_clay;
            next.obs += left * next.obs_robots;
            next.geodes += left * next.geo_robots;
            next.obs_robots++;
            sim(tick + left, bp, &next, best);
        }
    }
    
    // build a geode robot next
    //
    if (state->obs_robots) {
        left = (bp->geo_ore - state->ore + state->ore_robots - 1) / state->ore_robots;
        left2 = (bp->geo_obs - state->obs + state->obs_robots - 1) / state->obs_robots;
        left = 1 + MAX(left, 0);
        left2 = 1 + MAX(left2, 0);
        left = MAX(left, left2);
        if (tick + left <= TICKS) {
            next = *state;
            next.ore += left * next.ore_robots - bp->geo_ore;
            next.clay += left * next.clay_robots;
            next.obs += left * next.obs_robots - bp->geo_obs;
            next.geodes += left * next.geo_robots;
            next.geo_robots++;
            sim(tick + left, bp, &next, best);
        }
    }
}

int main()
{
    while (scanf(
        "Blueprint %d: Each ore robot costs %d ore. "
        "Each clay robot costs %d ore. "
        "Each obsidian robot costs %d ore and %d clay. "
        "Each geode robot costs %d ore and %d obsidian.\n",
        &blueprints[nblue].num,
        &blueprints[nblue].ore_ore,
        &blueprints[nblue].clay_ore,
        &blueprints[nblue].obs_ore,
        &blueprints[nblue].obs_clay,
        &blueprints[nblue].geo_ore,
        &blueprints[nblue].geo_obs) == 7) {
            nblue++;
    }

    nblue = MIN(nblue, 3);

    int prod = 1;
    
    for (int i = 0; i < nblue; i++) {
        state_t state;
        state.ore_robots = 1;
        state.clay_robots = 0;
        state.obs_robots = 0; 
        state.geo_robots = 0;
        state.ore = 0;
        state.clay = 0;
        state.obs = 0;
        state.geodes = 0;

        int best = 0;
        sim(0, &blueprints[i], &state, &best);
        prod *= best;
    }
    printf("%d\n", prod);
}
