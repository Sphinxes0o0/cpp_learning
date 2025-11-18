#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "shm_common.h"

void create_kv(uint32_t key, uint32_t value) {
    printf("create_kv: key=%d, value=%d\n", key, value);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return;
    }

    size_t sz = sizeof(HashMap);
    if (ftruncate(SHM_NAME, sz) == -1) {
        perror("ftruncate");
        return;
    }
    
    HashMap *hmap = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (hmap == MAP_FAILED) {
        perror("mmap");
        return;
    }

    if (sem_init(&hmap->sem, 1, 1) == -1) {
        perror("sem_init");
        return;
    }
    for (int i = 0; i < BUCKETS; i++) {
        hmap->buckets[i].size = 0;
        for (int j = 0; j < MAX_SLOTS; j++) {
            hmap->buckets[i].kv[j].key = 0;
            hmap->buckets[i].kv[j].value = 0;
        }
    }

    mn

}



int main() {



    return 0;
}