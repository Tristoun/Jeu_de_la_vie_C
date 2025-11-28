#ifndef GAME_H
#define GAME_H

#include "grid.h"

typedef struct
{
    double mean_ms;
    double worst_ms;
    double jitter_ms;
} timing_stats;

timing_stats play_game_live(grid *current, grid *next, int generations, int target_hz);

#endif
