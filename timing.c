#define _POSIX_C_SOURCE 200809L
#include "timing.h"
#include "grid.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Convertit la différence de temps en millisecondes
static double diff_ms(struct timespec a, struct timespec b)
{
    return (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_nsec - a.tv_nsec) / 1e6;
}

// Mesure le temps total par frame et retourne des stats
timing_stats measure_generations_total_time(grid *current, grid *next, int generations, int target_hz)
{
    timing_stats stats = {0};

    if (target_hz <= 0) target_hz = 60;
    long interval_ns = 1000000000L / target_hz;

    double *frame_times = calloc(generations, sizeof(double));
    if (!frame_times)
    {
        fprintf(stderr, "Erreur alloc timings\n");
        return stats;
    }

    struct timespec next_frame;
    clock_gettime(CLOCK_MONOTONIC, &next_frame);

    for (int g = 0; g < generations; g++)
    {
        struct timespec frame_start, frame_end, t1, t2;
        clock_gettime(CLOCK_MONOTONIC, &frame_start);

        // Temps de calcul
        clock_gettime(CLOCK_MONOTONIC, &t1);
        next_generation(current, next);
        clock_gettime(CLOCK_MONOTONIC, &t2);
        double calc_ms = diff_ms(t1, t2);

        // Swap des grilles
        grid tmp = *current;
        *current = *next;
        *next = tmp;

        // Maintien exact de la cadence
        next_frame.tv_nsec += interval_ns;
        while (next_frame.tv_nsec >= 1000000000L)
        {
            next_frame.tv_nsec -= 1000000000L;
            next_frame.tv_sec += 1;
        }

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        long wait_ns = (next_frame.tv_sec - now.tv_sec) * 1000000000L +
                       (next_frame.tv_nsec - now.tv_nsec);

        if (wait_ns > 0)
        {
            struct timespec req = { .tv_sec = wait_ns / 1000000000L,
                                    .tv_nsec = wait_ns % 1000000000L };
            nanosleep(&req, NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &frame_end);
        double total_ms = diff_ms(frame_start, frame_end);

        // Stocke le temps total par frame
        frame_times[g] = total_ms;

        // Optionnel : affichage du temps de calcul + total pour debug
        // printf("Gen %4d : calcul = %.3f ms, total = %.3f ms\n", g, calc_ms, total_ms);
    }

    // Calcul des statistiques sur le temps total par frame
    double sum = 0.0, worst = 0.0;
    for (int i = 0; i < generations; i++)
    {
        sum += frame_times[i];
        if (frame_times[i] > worst) worst = frame_times[i];
    }

    stats.mean_ms = sum / generations;
    stats.worst_ms = worst;
    stats.jitter_ms = worst - stats.mean_ms;

    free(frame_times);
    return stats;
}
