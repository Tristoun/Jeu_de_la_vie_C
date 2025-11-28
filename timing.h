#ifndef TIMING_H
#define TIMING_H

#include "grid.h"

typedef struct
{
    double mean_ms;
    double worst_ms;
    double jitter_ms;
} timing_stats;

timing_stats measure_generations_total_time(grid *current, grid *next, int generations, int target_hz);
#endif
