#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

// --- 所有定义都放在这里 ---
#define BUFFER_SIZE 10
#define SHM_NAME    "/pshm_prod_cons"
#define MUTEX_SEM   "/psem_mutex"
#define EMPTY_SEM   "/psem_empty"
#define FULL_SEM    "/psem_full"

// 共享内存中的结构
struct shared_data {
    char buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;  // 当前缓冲区中的项目数
};

// 打印用法提示
void print_usage(const char* prog_name) {
    fprintf(stderr, "Usage: %s [init|produce|consume|clean|status]\n", prog_name);
    fprintf(stderr, "  init     - 初始化共享内存和信号量\n");
    fprintf(stderr, "  produce  - 启动生产者进程\n");
    fprintf(stderr, "  consume  - 启动消费者进程\n");
    fprintf(stderr, "  clean    - 清理所有IPC资源\n");
    fprintf(stderr, "  status   - 显示当前缓冲区状态\n");
    exit(1);
}

// 检查并打印信号量值
void print_sem_value(const char* name, sem_t* sem) {
    int value;
    if (sem_getvalue(sem, &value) == 0) {
        printf("%s: %d\n", name, value);
    } else {
        perror("sem_getvalue");
    }
}

int main(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc != 2) {
        print_usage(argv[0]);
    }

    // ====================================================================
    // 角色 1: 初始化 (init)
    // ====================================================================
    if (strcmp(argv[1], "init") == 0) {
        printf("Initializing resources...\n");
        
        // 1. 创建并初始化共享内存
        int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) { 
            perror("shm_open"); 
            exit(1); 
        }
        
        if (ftruncate(shm_fd, sizeof(struct shared_data)) == -1) {
            perror("ftruncate");
            exit(1);
        }
        
        struct shared_data *data = (struct shared_data*)mmap(0, sizeof(struct shared_data), 
                                                             PROT_READ | PROT_WRITE, 
                                                             MAP_SHARED, shm_fd, 0);
        if (data == MAP_FAILED) { 
            perror("mmap"); 
            exit(1); 
        }
        
        data->in = 0;
        data->out = 0;
        data->count = 0;
        
        munmap(data, sizeof(struct shared_data));
        close(shm_fd);

        // 2. 创建并初始化三个信号量
        sem_t* mutex_sem = sem_open(MUTEX_SEM, O_CREAT, 0666, 1);
        if (mutex_sem == SEM_FAILED) {
            perror("sem_open mutex");
            exit(1);
        }
        
        sem_t* empty_sem = sem_open(EMPTY_SEM, O_CREAT, 0666, BUFFER_SIZE);
        if (empty_sem == SEM_FAILED) {
            perror("sem_open empty");
            exit(1);
        }
        
        sem_t* full_sem = sem_open(FULL_SEM, O_CREAT, 0666, 0);
        if (full_sem == SEM_FAILED) {
            perror("sem_open full");
            exit(1);
        }
        
        // 关闭信号量描述符
        sem_close(mutex_sem);
        sem_close(empty_sem);
        sem_close(full_sem);
        
        printf("Resources initialized.\n");
        printf("Shared memory: %s\n", SHM_NAME);
        printf("Semaphores: %s, %s, %s\n", MUTEX_SEM, EMPTY_SEM, FULL_SEM);
    }

    // ====================================================================
    // 角色 2: 生产者 (produce)
    // ====================================================================
    else if (strcmp(argv[1], "produce") == 0) {
        // 打开IPC资源
        int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(1);
        }
        
        struct shared_data *data = (struct shared_data*)mmap(0, sizeof(struct shared_data), 
                                                             PROT_READ | PROT_WRITE, 
                                                             MAP_SHARED, shm_fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        
        sem_t *mutex = sem_open(MUTEX_SEM, 0);
        if (mutex == SEM_FAILED) {
            perror("sem_open mutex");
            exit(1);
        }
        
        sem_t *empty = sem_open(EMPTY_SEM, 0);
        if (empty == SEM_FAILED) {
            perror("sem_open empty");
            exit(1);
        }
        
        sem_t *full  = sem_open(FULL_SEM, 0);
        if (full == SEM_FAILED) {
            perror("sem_open full");
            exit(1);
        }

        // 生成一个基于进程ID和时间的字符作为产品
        srand(time(NULL) + getpid());
        char item = 'A' + (rand() % 26);
        printf("Producer %d started, will produce '%c'\n", getpid(), item);

        for (int i = 0; i < 5; ++i) {
            // 等待空槽位
            if (sem_wait(empty) == -1) {
                perror("sem_wait empty");
                exit(1);
            }
            
            // 获取互斥锁
            if (sem_wait(mutex) == -1) {
                perror("sem_wait mutex");
                exit(1);
            }
            
            // 生产产品
            data->buffer[data->in] = item;
            data->in = (data->in + 1) % BUFFER_SIZE;
            data->count++;
            printf("Producer %d: put '%c' at index %d, total items: %d\n", 
                   getpid(), item, (data->in - 1 + BUFFER_SIZE) % BUFFER_SIZE, data->count);
            
            // 释放互斥锁
            if (sem_post(mutex) == -1) {
                perror("sem_post mutex");
                exit(1);
            }
            
            // 增加满槽位计数
            if (sem_post(full) == -1) {
                perror("sem_post full");
                exit(1);
            }
            
            // 随机休眠0-2秒
            sleep(rand() % 3);
        }
        
        printf("Producer %d finished.\n", getpid());
        
        // 清理资源
        munmap(data, sizeof(struct shared_data));
        close(shm_fd);
        sem_close(mutex);
        sem_close(empty);
        sem_close(full);
    }

    // ====================================================================
    // 角色 3: 消费者 (consume)
    // ====================================================================
    else if (strcmp(argv[1], "consume") == 0) {
        int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(1);
        }
        
        struct shared_data *data = (struct shared_data*)mmap(0, sizeof(struct shared_data), 
                                                             PROT_READ | PROT_WRITE, 
                                                             MAP_SHARED, shm_fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        
        sem_t *mutex = sem_open(MUTEX_SEM, 0);
        if (mutex == SEM_FAILED) {
            perror("sem_open mutex");
            exit(1);
        }
        
        sem_t *empty = sem_open(EMPTY_SEM, 0);
        if (empty == SEM_FAILED) {
            perror("sem_open empty");
            exit(1);
        }
        
        sem_t *full  = sem_open(FULL_SEM, 0);
        if (full == SEM_FAILED) {
            perror("sem_open full");
            exit(1);
        }
        
        printf("Consumer %d started.\n", getpid());
        srand(time(NULL) + getpid());

        for (int i = 0; i < 5; ++i) { 
            // 等待有产品可消费
            if (sem_wait(full) == -1) {
                perror("sem_wait full");
                exit(1);
            }
            
            // 获取互斥锁
            if (sem_wait(mutex) == -1) {
                perror("sem_wait mutex");
                exit(1);
            }
            
            // 消费产品
            char item = data->buffer[data->out];
            data->out = (data->out + 1) % BUFFER_SIZE;
            data->count--;
            printf("Consumer %d: got '%c' from index %d, total items: %d\n", 
                   getpid(), item, data->out, data->count+1);
            
            // 释放互斥锁
            if (sem_post(mutex) == -1) {
                perror("sem_post mutex");
                exit(1);
            }
            
            // 增加空槽位计数
            if (sem_post(empty) == -1) {
                perror("sem_post empty");
                exit(1);
            }
            
            // 随机休眠1-3秒
            sleep(1 + rand() % 3);
        }
        
        printf("Consumer %d finished.\n", getpid());
        
        // 清理资源
        munmap(data, sizeof(struct shared_data));
        close(shm_fd);
        sem_close(mutex);
        sem_close(empty);
        sem_close(full);
    }

    // ====================================================================
    // 角色 4: 清理 (clean)
    // ====================================================================
    else if (strcmp(argv[1], "clean") == 0) {
        if (shm_unlink(SHM_NAME) == -1) {
            if (errno != ENOENT) {
                perror("shm_unlink");
            }
        }
        
        if (sem_unlink(MUTEX_SEM) == -1) {
            if (errno != ENOENT) {
                perror("sem_unlink mutex");
            }
        }
        
        if (sem_unlink(EMPTY_SEM) == -1) {
            if (errno != ENOENT) {
                perror("sem_unlink empty");
            }
        }
        
        if (sem_unlink(FULL_SEM) == -1) {
            if (errno != ENOENT) {
                perror("sem_unlink full");
            }
        }
        
        printf("All IPC resources cleaned up.\n");
    }
    
    // ====================================================================
    // 角色 5: 状态查看 (status)
    // ====================================================================
    else if (strcmp(argv[1], "status") == 0) {
        // 打开共享内存
        int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(1);
        }
        
        struct shared_data *data = (struct shared_data*)mmap(0, sizeof(struct shared_data), 
                                                             PROT_READ, MAP_SHARED, shm_fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        
        // 打开信号量
        sem_t *mutex = sem_open(MUTEX_SEM, 0);
        if (mutex == SEM_FAILED) {
            perror("sem_open mutex");
            exit(1);
        }
        
        sem_t *empty = sem_open(EMPTY_SEM, 0);
        if (empty == SEM_FAILED) {
            perror("sem_open empty");
            exit(1);
        }
        
        sem_t *full  = sem_open(FULL_SEM, 0);
        if (full == SEM_FAILED) {
            perror("sem_open full");
            exit(1);
        }
        
        // 显示状态
        printf("=== 共享内存状态 ===\n");
        printf("缓冲区大小: %d\n", BUFFER_SIZE);
        printf("写入位置: %d\n", data->in);
        printf("读取位置: %d\n", data->out);
        printf("当前项目数: %d\n", data->count);
        printf("\n=== 信号量状态 ===\n");
        print_sem_value("互斥锁 (mutex)", mutex);
        print_sem_value("空槽位 (empty)", empty);
        print_sem_value("满槽位 (full)", full);
        
        // 显示缓冲区内容
        printf("\n=== 缓冲区内容 ===\n");
        int pos = data->out;
        for (int i = 0; i < data->count; i++) {
            printf("[%d]: '%c' ", pos, data->buffer[pos]);
            pos = (pos + 1) % BUFFER_SIZE;
            if ((i + 1) % 5 == 0) printf("\n");
        }
        if (data->count == 0) {
            printf("缓冲区为空\n");
        } else {
            printf("\n");
        }
        
        // 清理资源
        munmap(data, sizeof(struct shared_data));
        close(shm_fd);
        sem_close(mutex);
        sem_close(empty);
        sem_close(full);
    }

    // ====================================================================
    // 无效参数
    // ====================================================================
    else {
        print_usage(argv[0]);
    }

    return 0;
}
