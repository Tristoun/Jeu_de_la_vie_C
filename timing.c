#define _POSIX_C_SOURCE 200809L
#include "timing.h"
#include "grid.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static double diff_ms(struct timespec a, struct timespec b)
{
    return (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_nsec - a.tv_nsec) / 1e6;
}

timing_stats measure_generations(grid *current, grid *next, int generations, int target_hz)
{

    timing_stats stats = {0};

    double *times = calloc(generations, sizeof(double));
    if (!times)
    {
        fprintf(stderr, "Erreur alloc timings\n");
        return stats;
    }

    long target_ns = 0;
    if (target_hz > 0)
        target_ns = 1000000000L / target_hz;

    for (int g = 0; g < generations; g++)
    {

        struct timespec t1, t2;
        clock_gettime(CLOCK_MONOTONIC, &t1);

        // calcul d'une génération
        next_generation(current, next);

        clock_gettime(CLOCK_MONOTONIC, &t2);

        // durée réelle de calcul
        double ms = diff_ms(t1, t2);
        times[g] = ms;

        // swap des grilles
        grid tmp = *current;
        *current = *next;
        *next = tmp;

        // maintien de cadence si demandé
        if (target_ns > 0)
        {
            long elapsed_ns =
                (t2.tv_sec - t1.tv_sec) * 1000000000L +
                (t2.tv_nsec - t1.tv_nsec);

            long remaining = target_ns - elapsed_ns;
            if (remaining > 0)
            {
                struct timespec req = {
                    .tv_sec = remaining / 1000000000L,
                    .tv_nsec = remaining % 1000000000L};
                nanosleep(&req, NULL);
            }
        }
    }

    // Calcul stats
    double sum = 0, worst = 0;
    for (int i = 0; i < generations; i++)
    {
        sum += times[i];
        if (times[i] > worst)
            worst = times[i];
    }

    stats.mean_ms = sum / generations;
    stats.worst_ms = worst;
    stats.jitter_ms = stats.worst_ms - stats.mean_ms;

    free(times);
    return stats;
}
