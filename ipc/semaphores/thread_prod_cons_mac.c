#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <dispatch/dispatch.h>

// --- 所有定义都放在这里 ---
#define BUFFER_SIZE 10
#define MAX_THREADS 5

// 共享内存中的结构
struct shared_data {
    char buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;  // 当前缓冲区中的项目数
};

// 全局共享数据
struct shared_data g_data;

// 使用dispatch_semaphore替代POSIX信号量
dispatch_semaphore_t mutex_sem;    // 互斥锁
dispatch_semaphore_t empty_sem;    // 空槽位计数器
dispatch_semaphore_t full_sem;     // 满槽位计数器

// 线程参数结构
struct thread_args {
    int id;
    int items;  // 每个线程处理的项目数
};

// 生产者线程函数
void* producer_thread(void* arg) {
    struct thread_args* args = (struct thread_args*)arg;
    srand(time(NULL) + args->id);
    
    char item = 'A' + (rand() % 26);
    printf("Producer thread %d started, will produce '%c'\n", args->id, item);

    for (int i = 0; i < args->items; ++i) {
        // 等待空槽位
        dispatch_semaphore_wait(empty_sem, DISPATCH_TIME_FOREVER);
        
        // 获取互斥锁
        dispatch_semaphore_wait(mutex_sem, DISPATCH_TIME_FOREVER);
        
        // 生产产品
        g_data.buffer[g_data.in] = item;
        g_data.in = (g_data.in + 1) % BUFFER_SIZE;
        g_data.count++;
        printf("Producer thread %d: put '%c' at index %d, total items: %d\n", 
               args->id, item, (g_data.in - 1 + BUFFER_SIZE) % BUFFER_SIZE, g_data.count);
        
        // 释放互斥锁
        dispatch_semaphore_signal(mutex_sem);
        
        // 增加满槽位计数
        dispatch_semaphore_signal(full_sem);
        
        // 随机休眠0-1秒
        usleep((rand() % 2) * 1000000);
    }
    
    printf("Producer thread %d finished.\n", args->id);
    return NULL;
}

// 消费者线程函数
void* consumer_thread(void* arg) {
    struct thread_args* args = (struct thread_args*)arg;
    srand(time(NULL) * args->id);
    
    printf("Consumer thread %d started.\n", args->id);

    for (int i = 0; i < args->items; ++i) { 
        // 等待有产品可消费
        dispatch_semaphore_wait(full_sem, DISPATCH_TIME_FOREVER);
        
        // 获取互斥锁
        dispatch_semaphore_wait(mutex_sem, DISPATCH_TIME_FOREVER);
        
        // 消费产品
        char item = g_data.buffer[g_data.out];
        g_data.out = (g_data.out + 1) % BUFFER_SIZE;
        g_data.count--;
        printf("Consumer thread %d: got '%c' from index %d, total items: %d\n", 
               args->id, item, g_data.out, g_data.count+1);
        
        // 释放互斥锁
        dispatch_semaphore_signal(mutex_sem);
        
        // 增加空槽位计数
        dispatch_semaphore_signal(empty_sem);
        
        // 随机休眠1-2秒
        usleep((1 + rand() % 2) * 1000000);
    }
    
    printf("Consumer thread %d finished.\n", args->id);
    return NULL;
}

// 打印用法提示
void print_usage(const char* prog_name) {
    fprintf(stderr, "Usage: %s [producers] [consumers] [items_per_thread]\n", prog_name);
    fprintf(stderr, "  producers        - 生产者线程数 (1-%d)\n", MAX_THREADS);
    fprintf(stderr, "  consumers        - 消费者线程数 (1-%d)\n", MAX_THREADS);
    fprintf(stderr, "  items_per_thread - 每个线程处理的项目数 (1-20)\n");
    exit(1);
}

// 初始化信号量
void init_semaphores() {
    // 初始化互斥锁 (初始值为1)
    mutex_sem = dispatch_semaphore_create(1);
    
    // 初始化空槽位计数器 (初始值为BUFFER_SIZE)
    empty_sem = dispatch_semaphore_create(BUFFER_SIZE);
    
    // 初始化满槽位计数器 (初始值为0)
    full_sem = dispatch_semaphore_create(0);
}

// 显示当前状态
void print_status() {
    printf("\n=== 缓冲区状态 ===\n");
    printf("缓冲区大小: %d\n", BUFFER_SIZE);
    printf("写入位置: %d\n", g_data.in);
    printf("读取位置: %d\n", g_data.out);
    printf("当前项目数: %d\n", g_data.count);
    
    printf("\n=== 缓冲区内容 ===\n");
    int pos = g_data.out;
    for (int i = 0; i < g_data.count; i++) {
        printf("[%d]: '%c' ", pos, g_data.buffer[pos]);
        pos = (pos + 1) % BUFFER_SIZE;
        if ((i + 1) % 5 == 0) printf("\n");
    }
    if (g_data.count == 0) {
        printf("缓冲区为空\n");
    } else {
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc != 4) {
        print_usage(argv[0]);
    }
    
    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    int items_per_thread = atoi(argv[3]);
    
    // 验证参数
    if (num_producers < 1 || num_producers > MAX_THREADS ||
        num_consumers < 1 || num_consumers > MAX_THREADS ||
        items_per_thread < 1 || items_per_thread > 20) {
        print_usage(argv[0]);
    }
    
    printf("Starting %d producer threads and %d consumer threads, each handling %d items\n", 
           num_producers, num_consumers, items_per_thread);
    
    // 初始化共享数据
    memset(&g_data, 0, sizeof(g_data));
    
    // 初始化信号量
    init_semaphores();
    
    // 创建线程数组
    pthread_t producer_threads[MAX_THREADS];
    pthread_t consumer_threads[MAX_THREADS];
    struct thread_args producer_args[MAX_THREADS];
    struct thread_args consumer_args[MAX_THREADS];
    
    // 创建生产者线程
    for (int i = 0; i < num_producers; i++) {
        producer_args[i].id = i + 1;
        producer_args[i].items = items_per_thread;
        if (pthread_create(&producer_threads[i], NULL, producer_thread, &producer_args[i]) != 0) {
            perror("pthread_create producer");
            exit(1);
        }
    }
    
    // 创建消费者线程
    for (int i = 0; i < num_consumers; i++) {
        consumer_args[i].id = i + 1;
        consumer_args[i].items = items_per_thread;
        if (pthread_create(&consumer_threads[i], NULL, consumer_thread, &consumer_args[i]) != 0) {
            perror("pthread_create consumer");
            exit(1);
        }
    }
    
    // 等待所有生产者线程完成
    for (int i = 0; i < num_producers; i++) {
        if (pthread_join(producer_threads[i], NULL) != 0) {
            perror("pthread_join producer");
            exit(1);
        }
    }
    
    // 等待所有消费者线程完成
    for (int i = 0; i < num_consumers; i++) {
        if (pthread_join(consumer_threads[i], NULL) != 0) {
            perror("pthread_join consumer");
            exit(1);
        }
    }
    
    // 显示最终状态
    print_status();
    
    printf("All threads completed.\n");
    return 0;
}