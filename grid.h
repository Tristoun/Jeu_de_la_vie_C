#ifndef GRID_H
#define GRID_H

#include <stdint.h>

typedef enum {
    BOUNDARY_EDGE,
    BOUNDARY_TORUS,
    BOUNDARY_MIRROR,
    BOUNDARY_ALIVE_RIM
} boundary_mode;


#define MAX_MEMORY_BYTES (64 * 1024)

typedef struct {
    int width, height;
    int words_per_row;
    uint64_t* data;
    boundary_mode mode;
} grid;

grid generate_grid(int width, int height);
void fill_random_grid(grid* g);
void show_grid(const grid* g);
int get_cell(const grid* g, int x, int y);
void set_cell(grid* g, int x, int y, int state);
int num_neighbors(const grid* g, int x, int y);
int get_next_state(const grid* g, int x, int y);
void next_generation(grid* current, grid* next);
void free_grid(grid* g);
void load_grid(char* FileName, grid* g);
void write_final_grid(char* FileName, const grid* g);

#endif