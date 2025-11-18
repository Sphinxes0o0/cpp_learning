#ifndef MSG_COMMON_H
#define MSG_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <time.h>

// 消息队列键值
#define MSG_QUEUE_KEY 0x1234

// 消息类型定义
#define MSG_TYPE_PING 1     // 客户端发送的ping消息
#define MSG_TYPE_PONG 2     // 服务器回复的pong消息
#define MSG_TYPE_DATA 3     // 数据消息
#define MSG_TYPE_QUIT 4     // 退出消息

// 消息缓冲区大小
#define MAX_MSG_SIZE 256

// 消息结构体
struct msg_buffer {
    long msg_type;              // 消息类型
    char msg_text[MAX_MSG_SIZE]; // 消息内容
};

// 错误处理函数
void print_error(const char* msg) {
    fprintf(stderr, "Error: %s (errno: %d)\n", msg, errno);
    perror("");
}

// 获取消息队列ID
int get_msg_queue() {
    key_t key = MSG_QUEUE_KEY;
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
    }
    return msgid;
}

// 创建消息队列
int create_msg_queue() {
    key_t key = MSG_QUEUE_KEY;
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
    }
    return msgid;
}

// 删除消息队列
int remove_msg_queue(int msgid) {
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return -1;
    }
    return 0;
}

#endif // MSG_COMMON_H