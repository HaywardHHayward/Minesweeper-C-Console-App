#include "haywardstd.h"


array_t* create_array(unsigned long data_size) {
    array_t* array = (array_t*) calloc(1, sizeof(array_t));
    array->data_size = data_size;
    array->free_func = NULL;
    array->array = calloc(INITIAL_DYNAMIC_ARRAY_SIZE, data_size);
    array->capacity = INITIAL_DYNAMIC_ARRAY_SIZE;
    return array;
}

array_t* create_array_with_length(unsigned long data_size, unsigned long length) {
    array_t* array = create_array(data_size);
    resize_array(array, length);
    return array;
}

void free_array(array_t* array) {
    if (!array->free_func) {
        free(array->array);
        free(array);
        return;
    }
    for (int i = 0; i < array->size; i++) {
        array->free_func(get_array_index(array, i));
    }
    free(array->array);
    free(array);
}

void resize_array(array_t* array, unsigned long new_size) {
    if (array->size > new_size) {
        perror("Cannot resize to smaller than the current array size");
        abort();
    }
    array->capacity = new_size;
    void* new_array = realloc(array->array, array->data_size * array->capacity);
    if (!new_array) {
        perror("No memory");
        abort();
    }
    array->array = new_array;
}

void expand_array(array_t* array) {
    resize_array(array, array->capacity * 2);
}

void* get_array_index(array_t* array, unsigned long index) {
    return array->array + (index * array->data_size);
}

void set_array_index(array_t* array, unsigned long index, void* data) {
    if (index >= array->size) {
        perror("You indexed out of the bounds of the array");
        abort();
    }
    memmove(get_array_index(array, index), data, array->data_size);
}

void add_to_array(array_t* array, void* data) {
    if (array->size + 1 == array->capacity) {
        expand_array(array);
    }
    memmove(get_array_index(array, array->size), data, array->data_size);
    array->size++;
}

void* find_in_array(array_t* array, void* data, int compar(const void*, const void*), unsigned long* index) {
    for (int i = 0; i < array->size; i++) {
        if (compar(get_array_index(array, i), data) != 0) continue;
        if (index) {
            *index = i;
        }
        return get_array_index(array, i);
    }
    return NULL;
}

int remove_from_array(array_t* array, void* data, int compar(const void*, const void*)) {
    unsigned long* index = calloc(1, sizeof(unsigned long));
    void* result = find_in_array(array, data, compar, index);
    if (!result) {
        free(index);
        return 0;
    }
    unsigned long i = *index;
    int ret = remove_index_from_array(array, i);
    free(index);
    if(array->size < (array->capacity / 2) + 1 && (array->capacity / 2) + 1 != array->capacity){
        resize_array(array, (array->capacity / 2) + 1);
    }
    return ret;
}

int remove_index_from_array(array_t* array, unsigned long index) {
    if (index >= array->size) {
        return 0;
    }
    if (index + 1 < array->size) {
        memmove(get_array_index(array, index), get_array_index(array, index + 1),
                (array->data_size) * (array->size - index - 1));
        array->size--;
        return 1;
    }
    memset(get_array_index(array, index), '\0', array->data_size);
    array->size--;
    if(array->size < (array->capacity / 2) + 1 && (array->capacity / 2) + 1 != array->capacity){
        resize_array(array, (array->capacity / 2) + 1);
    }
    return 1;
}

void* array_insert_at_index(array_t* array, unsigned long index, void* data) {
    if (index >= array->size) {
        perror("You indexed outside the bounds of the array");
        abort();
    }
    if (array->size + 1 == array->capacity) {
        expand_array(array);
    }
    memmove(get_array_index(array, index + 1), get_array_index(array, index),
            (array->data_size) * (array->size - index));
    array->size++;
    set_array_index(array, index, data);
    return get_array_index(array, index);
}

void range_add_to_array(array_t* array, array_t* new_data) {
    if (array->data_size != new_data->data_size) {
        perror("Data sizes mismatch");
        abort();
    }
    for (int i = 0; i < new_data->size; i++) {
        add_to_array(array, get_array_index(new_data, i));
    }
}

int contained_in_array(array_t* array, void* data, int (* compar)(const void*, const void*)) {
    void* result = find_in_array(array, data, compar, NULL);
    return result ? 1 : 0;
}

int array_set_equal(array_t* array1, array_t* array2, int compar(const void*, const void*)) {
    if (array1->size != array2->size || array1->data_size != array2->data_size) {
        return 0;
    }
    array_t* difference = create_array(array1->data_size);
    range_add_to_array(difference, array1);
    difference->free_func = array1->free_func;
    for (int i = 0; i < array2->size; i++) {
        int result = remove_from_array(difference, get_array_index(array2, i), compar);
        if (!result) {
            free_array(difference);
            return 0;
        }
    }
    if (difference->array) {
        free_array(difference);
    }
    return 1;
}

int random_number(int min_num, int max_num) {
    int result, low_num, hi_num;

    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num; // include max_num in output
    } else {
        low_num = max_num; // include max_num in output
        hi_num = min_num;
    }
    result = rand() % (hi_num - low_num) + low_num;
    return result;
}
