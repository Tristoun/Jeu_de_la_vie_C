/*
Gestion de la grille
*/

#include <stdlib.h>
#include <stdio.h>

#include "grid.h"

grid generate_grid(int width, int height) {

    //Genere la grilel

    if(width == 0 || height == 0) { //Si notre grille est celle par défaut retourner une grille classique
        grid default_grid;
        default_grid.width = DEFAULT_WIDTH;
        default_grid.height = DEFAULT_HEIGHT;
        return default_grid;
    }
    else {
        grid grille;
        grille.width = width;
        grille.height = height;
        return grille;
        //Besoin d'allouer dynamiquement la taille de notre nouvelle grille
    }
}


void fill_random_grid(grid *Grid) {
    int wi = Grid->width;
    int hei = Grid->height;

    for (int i =0; i<wi; i++) {
        for (int j=0; j<hei; j++) {
            int value = rand() % 2;
            Grid->content[i][j].x = i;
            Grid->content[i][j].y = j;
            Grid->content[i][j].state = value;
            printf("%d", value);
        }
        printf("\n");
    }
}


void show_grid(grid *Grid) {
    int wi = Grid->width;
    int hei = Grid->height;

    for (int i =0; i<wi; i++) {
        for (int j=0; j<hei; j++) {
            if(Grid->content[i][j].state == 1) {
                printf("X");
            }
            else {
                printf("-");
            }
        }
        printf("\n");
    }
}

int num_neighbors(grid *Grid, cell Cell) {
    //Regarde le nombre de voisins vivantes
    int nb = 0; //Nombre de voisins
    int x = Cell.x; 
    int y = Cell.y;
    int lst_voisins[CELL_CHECK][COORD] = {
        {x-1, y-1}, 
        {x-1, y}, 
        {x-1, y+1},
        {x, y-1},
        {x, y+1},
        {x+1, y-1},
        {x+1, y},
        {x+1, y+1}
    };


    for (int i=0; i<CELL_CHECK; i++) {
        int x_voisins = lst_voisins[i][0];
        int y_voisins = lst_voisins[i][1];
        printf("Checking coord of cell (%d, %d)\n", x_voisins, y_voisins);
        if(get_cell_state(Grid->content[x_voisins][y_voisins]) == 1) {
            nb ++;
        }
    }

    return nb;
}

int get_next_state(grid* Grid, int x, int y) {
    // Récupère le nombre de voisins vivants
    int alive_neighbors = num_neighbors(Grid, Grid->content[x][y]);

    int current_state = Grid->content[x][y].state;

    if (current_state == 1) {
        // Cellule vivante survit avec 2 ou 3 voisins
        if (alive_neighbors == 2 || alive_neighbors == 3)
            return 1;
        else
            return 0;
    }
    else {
        // Cellule morte naît avec exactement 3 voisins
        if (alive_neighbors == 3)
            return 1;
        else
            return 0;
    }
}


void next_generation(grid* current, grid* next) {
    int width = current->width;
    int height = current->height;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            
            int next_state = get_next_state(current, i, j);

            // Mise à jour du prochain monde
            next->content[i][j].state = next_state;
            next->content[i][j].x = i;
            next->content[i][j].y = j;
        }
    }
}