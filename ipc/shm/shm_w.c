#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "shm_comm.h"


int main() {

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(fd, SHM_SIZE) < 0) {
        perror("ftruncate");
        return -1;
    }

    char *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }

    const char *msg = "Hello, World!";
    strncpy(ptr, msg, SHM_SIZE);
    printf("Wrote %s to shared memory\n", msg);

    sleep(3);

    // signal the reader
    sem_post(sem);

    sleep(2);

    if (munmap(ptr, SHM_SIZE) < 0) {
        perror("munmap");
        return -1;
    }
    if (close(fd) < 0) {
        perror("close");
        return -1;
    }
    if (sem_close(sem) < 0) {
        perror("sem_close");
        return -1;
    }
    if (sem_unlink(SEM_NAME) < 0) {
        perror("sem_unlink");
        return -1;
    }
    if (shm_unlink(SHM_NAME) < 0) {
        perror("shm_unlink");
        return -1;
    }


    return 0;
}

