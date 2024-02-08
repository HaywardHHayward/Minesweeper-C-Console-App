#include <stdio.h>
#include <stdlib.h>
#include "minesweeper_functions.h"

int same(const void* one, const void* two){
    return *(int*)one - *(int*)two;
}

int main() {
    array_t* array1 = create_array(sizeof(int));
    array_t* array2 = create_array(sizeof(int));
    int x = 1, y = 2, z = 3, w = 4;
    add_to_array(array1, (void*)&z);
    add_to_array(array1, (void*)&z);
    add_to_array(array1, (void*)&w);
    add_to_array(array2, (void*)&y);
    add_to_array(array2, (void*)&z);
    add_to_array(array2, (void*)&w);
    printf("%d\n", array_set_equal(array1, array2, same));
    free_array(array1);
    free_array(array2);
    getchar();
}
