/*
Definition des règles :
​- Une cellule morte ayant​​exactement trois voisines​​vivantes​​devient vivante.​
​- ​Une cellule vivante ayant​​deux ou trois voisines vivantes​​survit.​
​- ​Dans tous les autres cas, la cellule meurt ou reste morte.​

Ce fichier gère les cellules, leur vie
*/

#include <stdio.h>

#include "cell.h"
#include "constant.h"

int get_cell_state(cell Cell) {
    // Donne l'état de la cellule actuellement (vivant ou morte)
    return Cell.state; //1 si vivante, 0 si morte
} 







