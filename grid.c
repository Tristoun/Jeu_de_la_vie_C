#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "grid.h"
#include "constant.h"

//  Création de la grille
grid generate_grid(int width, int height) {

    if (width <= 0)  width = DEFAULT_WIDTH;
    if (height <= 0) height = DEFAULT_HEIGHT;

    grid g;
    g.width  = width;
    g.height = height;

    g.words_per_row = (width + 63) / 64;
    g.data = calloc(g.words_per_row * height, sizeof(uint64_t));

    return g;
}

//  Lecture d'une cellule (bitboard)
int get_cell(const grid* g, int x, int y) {
    if (x < 0 || x >= g->width || y < 0 || y >= g->height)
        return 0;

    uint64_t word = g->data[y * g->words_per_row + (x >> 6)];
    uint64_t mask = (uint64_t)1 << (x & 63);

    return (word & mask) != 0;
}

//  Écriture d'une cellule (bitboard)
void set_cell(grid* g, int x, int y, int state) {
    if (x < 0 || x >= g->width || y < 0 || y >= g->height)
        return;

    uint64_t* word = &g->data[y * g->words_per_row + (x >> 6)];
    uint64_t mask = (uint64_t)1 << (x & 63);

    if (state)
        *word |= mask;
    else
        *word &= ~mask;
}

//  Remplir la grille aléatoirement
void fill_random_grid(grid* g) {
    for (int y = 0; y < g->height; y++) {
        for (int x = 0; x < g->width; x++) {
            set_cell(g, x, y, rand() % 2);
        }
    }
}

//  Affichage de la grille
void show_grid(const grid* g) {
    for (int y = 0; y < g->height; y++) {
        for (int x = 0; x < g->width; x++) {
            printf("%c", get_cell(g, x, y) ? 'X' : '-');
        }
        printf("\n");
    }
}

//  Compte les voisins vivants
int num_neighbors(const grid* g, int x, int y) {

    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {

            if (dx == 0 && dy == 0) continue; // exclut la cellule elle-même

            count += get_cell(g, x + dx, y + dy);
        }
    }

    return count;
}

//  Règles du jeu
int get_next_state(const grid* g, int x, int y) {

    int alive = get_cell(g, x, y);
    int n = num_neighbors(g, x, y);

    if (alive) {
        return (n == 2 || n == 3);
    } else {
        return (n == 3);
    }
}

//  Passe de current → next
void next_generation(grid* current, grid* next) {

    // on vide next avant utilisation
    memset(next->data, 0, next->words_per_row * next->height * sizeof(uint64_t));

    for (int y = 0; y < current->height; y++) {
        for (int x = 0; x < current->width; x++) {

            int s = get_next_state(current, x, y);
            set_cell(next, x, y, s);
        }
    }
}

void free_grid(grid* g) {
    if (g->data) free(g->data);
    g->data = NULL;
}