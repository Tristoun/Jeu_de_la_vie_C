#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "grid.h"

int main() {
    srand(time(NULL));

    grid Grid;

    Grid = generate_grid(0, 0); // 0 0 for default values
    fill_random_grid(&Grid);
    show_grid(&Grid);

    printf("%d\n", num_neighbors(&Grid, Grid.content[5][5]));

    return 0;
}