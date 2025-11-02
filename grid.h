#
#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include "constant.h"

#ifndef grid 

    typedef struct grid {
        int width, height;
        cell content[DEFAULT_WIDTH][DEFAULT_HEIGHT]; //Toutes les cellules sont mortes
    } grid;
#endif

grid generate_grid(int width, int height);

void fill_random_grid(grid *Grid);

void show_grid(grid *Grid);

int num_neighbors(grid *Grid, cell Cell);

int get_next_state(grid* Grid, int x, int y);

void next_generation(grid* current, grid* next);



#endif