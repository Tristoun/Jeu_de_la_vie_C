#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "grid.h"

int main() {
    srand(time(NULL));

    printf("-- Projet Jeu de la Vie (Debug - Toutes générations visibles) --\n");

    // Petite grille plus lisible pour debug
    grid Grid = generate_grid(32, 16);

    // ======= CHOISIS LE MODE =======
    Grid.mode = BOUNDARY_EDGE;
    //Grid.mode = BOUNDARY_TORUS;
    //Grid.mode = BOUNDARY_MIRROR;
    //Grid.mode = BOUNDARY_ALIVE_RIM;
    // ================================

    fill_random_grid(&Grid);

    printf("\nMode de bordure utilisé : ");
    switch (Grid.mode) {
        case BOUNDARY_EDGE:      printf("EDGE\n"); break;
        case BOUNDARY_TORUS:     printf("TORUS\n"); break;
        case BOUNDARY_MIRROR:    printf("MIRROR\n"); break;
        case BOUNDARY_ALIVE_RIM: printf("ALIVE_RIM\n"); break;
    }

    // Affiche la grille initiale
    printf("\n=== Generation 0 ===\n");
    show_grid(&Grid);

    // Grille suivante
    grid Next = generate_grid(Grid.width, Grid.height);
    Next.mode = Grid.mode;

    // Afficher les 20 générations
    for (int gen = 1; gen <= 20; gen++) {

        next_generation(&Grid, &Next);

        printf("\n=== Generation %d ===\n", gen);
        show_grid(&Next);

        // échange simple des grilles
        grid tmp = Grid;
        Grid = Next;
        Next = tmp;
    }

    free_grid(&Grid);
    free_grid(&Next);

    return 0;
}


// gcc main.c grid.c cell.c -Wall -Wextra -o jeu