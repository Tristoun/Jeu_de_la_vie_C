#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "timing.h" 
#include "constant.h"



int main(int argc, char* argv[]) //argc = nombres de paramètres, argv = paramètres entrées 
{
    srand(time(NULL));
    int width = 20, height = 20;
    int gens = 100;
    boundary_mode mode = BOUNDARY_EDGE;
    char* input_filename = malloc(strlen("glider.txt") + 1);
    strcpy(input_filename, "glider.txt");
    char* output_filename = malloc(strlen("end.txt") + 1);
    strcpy(output_filename, "end.txt");
    int target_hz = 60;


    for (int i =0; i<argc; i++) {
        if(strcmp(argv[i], "--width") == 0) {
            width = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--height") == 0) {
            height = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--gens") == 0){
            gens = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--boundary") == 0) {
            char mode_string[10];
            strcpy(mode_string, argv[i+1]);
            if (strcmp(mode_string, "edge") == 0) {
            mode = BOUNDARY_EDGE;
            }
            else if (strcmp(mode_string, "torus") == 0) {
                mode = BOUNDARY_TORUS;
            }
            else if (strcmp(mode_string, "mirror") == 0) {
                mode = BOUNDARY_MIRROR;
            }
            else if (strcmp(mode_string, "rim") == 0) {
                mode = BOUNDARY_ALIVE_RIM;
            }
            else {
                printf("Unknown mode: %s\n", mode_string);
            }

        }
        else if (strcmp(argv[i], "--in") == 0){
            input_filename = realloc(input_filename, strlen(argv[i+1]) + 1);
            strcpy(input_filename, argv[i+1]);
        }

        else if (strcmp(argv[i], "--out") == 0){
            output_filename = realloc(output_filename, strlen(argv[i+1]) + 1);
            strcpy(output_filename, argv[i+1]);
        }
        else if (strcmp(argv[i], "--target-hz") == 0){
            target_hz = atoi(argv[i+1]);
        }
    }


    printf("-- Projet Jeu de la Vie (Debug - Toutes générations visibles) --\n");

    // Petite grille plus lisible pour debug
    grid Grid = generate_grid(width, height);

    // ======= CHOISIS LE MODE =======
    // Grid.mode = BOUNDARY_EDGE;
    // Grid.mode = BOUNDARY_TORUS;
    // Grid.mode = BOUNDARY_MIRROR;
    // Grid.mode = BOUNDARY_ALIVE_RIM;
    // ================================
    Grid.mode = mode;
    fill_random_grid(&Grid);
    // load_grid(input_filename, &Grid);
    printf("\nMode de bordure utilisé : ");
    switch (Grid.mode)
    {
    case BOUNDARY_EDGE:
        printf("EDGE\n");
        break;
    case BOUNDARY_TORUS:
        printf("TORUS\n");
        break;
    case BOUNDARY_MIRROR:
        printf("MIRROR\n");
        break;
    case BOUNDARY_ALIVE_RIM:
        printf("ALIVE_RIM\n");
        break;
    }

    // Affiche la grille initiale
    printf("\n=== Generation 0 ===\n");
    show_grid(&Grid);

    // Grille suivante
    grid Next = generate_grid(Grid.width, Grid.height);
    Next.mode = Grid.mode;

    for (int gen = 1; gen <= gens; gen++)
    {
        next_generation(&Grid, &Next);

        printf("\n=== Generation %d ===\n", gen);
        show_grid(&Next);

        // échange simple des grilles
        grid tmp = Grid;
        Grid = Next;
        Next = tmp;

        // struct timespec req = {0, 100000000};  // 0.2 seconds
        // nanosleep(&req, NULL);

        // Efface l’écran après l’affichage
        if(gen < gens) {
            printf("\033[2J\033[1;1H");   
            fflush(stdout);
        }

    }
    write_final_grid(output_filename, &Grid);
    // === MESURE TEMPS RÉEL ===
    printf("\n=== Mesure temps réel (1000 générations) ===\n");

    timing_stats ts = measure_generations_total_time(&Grid, &Next, 1000, target_hz);

    printf("Temps moyen : %.3f ms\n", ts.mean_ms);
    printf("Pire cas    : %.3f ms\n", ts.worst_ms);
    printf("Jitter      : %.3f ms\n", ts.jitter_ms);

    free_grid(&Grid);
    free_grid(&Next);

    return 0;
}


// gcc main.c grid.c cell.c -Wall -Wextra -o jeu