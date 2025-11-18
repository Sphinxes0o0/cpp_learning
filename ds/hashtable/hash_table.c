#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item *ht_new_item(char *k, char *v) {
    ht_item *item = malloc(sizeof(ht_item));
    item->key = strdup(k);
    item->value = strdup(v);

    return item;
}

ht_hash_table *ht_new() {
    ht_hash_table *table = malloc(sizeof(ht_hash_table));

    table->count = 0;
    table->size = 53;
    table->items = calloc((size_t)table->size, sizeof(ht_item *));

    return table;
}


static void ht_del_item(ht_item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

void ht_hash_table_del(ht_hash_table *table) {
    for (int i = 0; i < table->size; i++) {
        ht_item *item = table->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }

    free(table->items);
    free(table);
}