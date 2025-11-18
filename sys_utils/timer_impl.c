#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

// 定时器结构体
typedef struct {
    time_t start_time;    // 定时器开始时间
    time_t end_time;      // 定时器结束时间
    time_t remaining_time;// 剩余时间
    bool is_running;      // 定时器是否在运行
    bool is_paused;       // 定时器是否被暂停
} Timer;

// 回调函数类型定义
typedef void (*TimerCallback)(void);

void resume_timer(Timer* timer, TimerCallback callback);

// 停止定时器
void stop_timer(Timer* timer) {
    timer->is_running = false;
    timer->is_paused = false;
}

// 示例回调函数1
void timer1_callback() {
    printf("定时器1时间到!\n");
}

// 示例回调函数2
void timer2_callback() {
    printf("定时器2时间到!\n");
}

// 设置定时时间
void settime(Timer* timer, time_t seconds) {
    timer->end_time = seconds;
    timer->is_running = false;
    timer->is_paused = false;
}

// 获取当前时间
time_t gettimeofday() {
    return time(NULL);
}

// 启动定时器
void start_timer(Timer* timer, TimerCallback callback) {
    if (!timer->is_running && !timer->is_paused) {
        timer->start_time = time(NULL);
        timer->is_running = true;
        timer->is_paused = false;
        
        // 定时器主循环
        while (timer->is_running) {
            time_t current_time = time(NULL);
            time_t elapsed_time = current_time - timer->start_time;
            
            if (elapsed_time >= timer->end_time) {
                // 触发回调函数
                if (callback != NULL) {
                    callback();
                }
                stop_timer(timer);
                break;
            }
            
            // 计算剩余时间
            timer->remaining_time = timer->end_time - elapsed_time;
            
            // 模拟非阻塞等待(实际应用中可以使用更高效的等待机制)
            sleep(1);
        }
    } else if (timer->is_paused) {
        resume_timer(timer, callback);
    }
}

// 恢复定时器
void resume_timer(Timer* timer, TimerCallback callback) {
    if (timer->is_paused) {
        timer->start_time = time(NULL) - (timer->end_time - timer->remaining_time);
        timer->is_running = true;
        timer->is_paused = false;
        
        // 继续定时器主循环
        while (timer->is_running) {
            time_t current_time = time(NULL);
            time_t elapsed_time = current_time - timer->start_time;
            
            if (elapsed_time >= timer->end_time) {
                // 触发回调函数
                if (callback != NULL) {
                    callback();
                }
                stop_timer(timer);
                break;
            }
            
            // 计算剩余时间
            timer->remaining_time = timer->end_time - elapsed_time;
            
            // 模拟非阻塞等待
            sleep(1);
        }
    }
}

// 暂停定时器
void pause_timer(Timer* timer) {
    if (timer->is_running) {
        timer->is_running = false;
        timer->is_paused = true;
    }
}

int main() {
    Timer timer1, timer2;
    
    // 初始化定时器
    timer1.is_running = false;
    timer1.is_paused = false;
    timer2.is_running = false;
    timer2.is_paused = false;
    
    // 设置定时器1为10秒
    settime(&timer1, 10);
    printf("定时器1开始，设置时间为%d秒\n", (int)timer1.end_time);
    
    // 设置定时器2为15秒
    settime(&timer2, 3);
    printf("定时器2开始，设置时间为%d秒\n", (int)timer2.end_time);
    
    // 启动定时器1
    start_timer(&timer1, timer1_callback);
    
    // 启动定时器2
    start_timer(&timer2, timer2_callback);
    
    return 0;
}

