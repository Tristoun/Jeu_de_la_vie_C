#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "grid.h"

int main() {
    srand(time(NULL));

    printf("--Projet Jeu De La Vie En C--\n");

    // Crée une grille par défaut (tu peux mettre 64x64 par ex.)
    grid Grid = generate_grid(64, 64);

    fill_random_grid(&Grid);

    printf("Grille initiale :\n");
    show_grid(&Grid);

    // Exemple : voisins de la cellule (5,5)
    printf("\nVoisins de (5,5) : %d\n", num_neighbors(&Grid, 5, 5));

    // Génération suivante
    grid nextGrid = generate_grid(Grid.width, Grid.height);
    next_generation(&Grid, &nextGrid);

    printf("\n--- Generation suivante ---\n");
    show_grid(&nextGrid);

    // Libération mémoire
    free_grid(&Grid);
    free_grid(&nextGrid);

    return 0;
}


// gcc main.c grid.c cell.c -Wall -Wextra -o jeu