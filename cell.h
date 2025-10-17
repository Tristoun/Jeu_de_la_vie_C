
#ifndef CELL_H
#define CELL_H


typedef struct {
    int x, y;
    int state; // 1 si vivante, 0 si morte
}cell;

int get_cell_state(cell Cell);


#endif // CELL_H

