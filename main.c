#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "grid.h"

int main() {
    srand(time(NULL));

    grid Grid;
    printf("--Projet Jeu De La Vie En C--\n");
    Grid = generate_grid(0, 0); // 0 0 for default values
    fill_random_grid(&Grid);

    show_grid(&Grid);
    printf("%d\n", num_neighbors(&Grid, Grid.content[5][5]));

    grid nextGrid = generate_grid(Grid.width, Grid.height);
    next_generation(&Grid, &nextGrid);
    printf("\n--- Generation suivante ---\n");
    show_grid(&nextGrid);

    return 0;
}


// gcc main.c grid.c cell.c -Wall -Wextra -o jeu