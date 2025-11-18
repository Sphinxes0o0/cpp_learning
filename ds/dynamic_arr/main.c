#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPACITY 8

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} array_t;


static void array_init(array_t* a) {
    a->data = (int*)malloc(MAX_CAPACITY * sizeof(int));
    a->size = 0;
    a->capacity = MAX_CAPACITY;

    if (a->data == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
}

static void array_free(array_t* a) {
    free(a->data);
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
}

static void array_grow(array_t* a) {
    size_t new_capacity = a->capacity * 2;
    int *new_data = (int*)realloc(a->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    a->data = new_data;
    a->capacity = new_capacity;
    printf("array grow: %zu -> %zu\n", new_capacity / 2, new_capacity);
}

static void array_push(array_t* a, int value) {
    if (a->size == a->capacity) array_grow(a);

    a->data[a->size++] = value;
}

int main(int argc, char** argv) {
    array_t my_array;
    array_init(&my_array);
    for (int i=0; i < 15; ++i) {
        array_push(&my_array, i);
    }

    printf("currenty size: %zu, capacity： %zu\n", my_array.size, my_array.capacity);
    for(size_t i = 0; i < my_array.size; ++i)
        printf("%d ", my_array.data[i]);
    
    putchar('\n');
    array_free(&my_array);

    return 0;
}