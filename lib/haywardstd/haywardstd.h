#ifndef HAYWARDSTD_HAYWARDSTD_H
#define HAYWARDSTD_HAYWARDSTD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define INITIAL_DYNAMIC_ARRAY_SIZE 2

typedef struct dynamic_array {
    void* array;

    void (* free_func)(void*);

    unsigned long data_size;
    unsigned long size;
    unsigned long capacity;
} array_t;

array_t* create_array(unsigned long data_size);

array_t* create_array_with_length(unsigned long data_size, unsigned long length);

void resize_array(array_t* array, unsigned long new_size);

void free_array(array_t* array);

void expand_array(array_t* array);

void* get_array_index(array_t* array, unsigned long index);

void add_to_array(array_t* array, void* data);

void range_add_to_array(array_t* array, array_t* new_data);

void* find_in_array(array_t* array, void* data, int compar(const void*, const void*), unsigned long* index);

int contained_in_array(array_t* array, void* data, int compar(const void*, const void*));

int remove_index_from_array(array_t* array, unsigned long index);

int remove_from_array(array_t* array, void* data, int compar(const void*, const void*));

void* array_insert_at_index(array_t* array, unsigned long index, void* data);

int array_set_equal(array_t* array1, array_t* array2, int compar(const void*, const void*));

int random_number(int min_num, int max_num);

#endif //HAYWARDSTD_HAYWARDSTD_H
