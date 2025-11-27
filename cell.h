#include <stdint.h>
#ifndef CELL_H
#define CELL_H


typedef struct {
    int x, y;
    uint64_t state; // 1 si vivante, 0 si morte
}cell;




#endif // CELL_H

