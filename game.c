#define _POSIX_C_SOURCE 200809L
#include "game.h"
#include "grid.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

static void get_time_windows(struct timespec *ts)
{
    static LARGE_INTEGER freq;
    static int freq_init = 0;
    LARGE_INTEGER counter;

    if (!freq_init)
    {
        QueryPerformanceFrequency(&freq);
        freq_init = 1;
    }

    QueryPerformanceCounter(&counter);

    ts->tv_sec = counter.QuadPart / freq.QuadPart;
    ts->tv_nsec = (long)(((counter.QuadPart % freq.QuadPart) * 1000000000LL) / freq.QuadPart);
}

static void sleep_ns_windows(long ns)
{
    LARGE_INTEGER ft;
    ft.QuadPart = -(ns / 100); // Windows en 100ns units

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, FALSE);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

#define clock_gettime(a,b) get_time_windows(b)
#define nanosleep(req,rem) sleep_ns_windows((req)->tv_nsec)

#endif

// Convertit la différence de temps en millisecondes
static double diff_ms(struct timespec a, struct timespec b)
{
    return (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_nsec - a.tv_nsec) / 1e6;
}

// Joue la simulation avec affichage live et cadence exacte
timing_stats play_game_live(grid *current, grid *next, int generations, int target_hz)
{
    timing_stats stats = {0};
    if (target_hz <= 0) target_hz = 60;

    long interval_ns = 1000000000L / target_hz;
    double *frame_times = calloc(generations, sizeof(double));
    if (!frame_times) return stats;

    // Heure absolue de départ pour synchroniser les frames
    struct timespec start_abs;
    clock_gettime(CLOCK_MONOTONIC, &start_abs);

    for (int gen = 0; gen < generations; gen++)
    {
        // --- Temps de calcul pur ---
        struct timespec calc_start, calc_end;
        clock_gettime(CLOCK_MONOTONIC, &calc_start);

        next_generation(current, next);

        // Swap des grilles
        grid tmp = *current;
        *current = *next;
        *next = tmp;

        clock_gettime(CLOCK_MONOTONIC, &calc_end);
        double calc_ms = diff_ms(calc_start, calc_end);

        // --- Maintien exact de la cadence ---
        long target_ns = (gen + 1) * interval_ns;
        struct timespec now;
        while (1)
        {
            clock_gettime(CLOCK_MONOTONIC, &now);
            long elapsed_ns = (now.tv_sec - start_abs.tv_sec) * 1000000000L +
                              (now.tv_nsec - start_abs.tv_nsec);
            long remaining_ns = target_ns - elapsed_ns;
            if (remaining_ns <= 0) break;

            // Sleep pour économiser CPU si beaucoup de temps reste
            if (remaining_ns > 50000L)
            {
                struct timespec req = {0, remaining_ns - 10000L};
                nanosleep(&req, NULL);
            }
        }

        // --- Mesure du temps total de la frame (calcul + attente) ---
        struct timespec frame_end;
        clock_gettime(CLOCK_MONOTONIC, &frame_end);
        frame_times[gen] = diff_ms(calc_start, frame_end);

        // --- Affichage live après avoir respecté la cadence ---
        printf("\n=== Generation %d ===\n", gen + 1);
        show_grid(current);
    }

    // --- Statistiques sur le temps total par frame ---
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
