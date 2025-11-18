#include "msg_common.h"

int main() {
    int msgid;
    struct msg_buffer message;
    
    // 创建或获取消息队列
    msgid = create_msg_queue();
    if (msgid == -1) {
        exit(EXIT_FAILURE);
    }
    
    printf("Producer: Message queue created with id %d\n", msgid);
    
    // 发送几条不同类型的消息
    // 1. 发送ping消息
    message.msg_type = MSG_TYPE_PING;
    snprintf(message.msg_text, MAX_MSG_SIZE, "Ping from producer (PID: %d)", getpid());
    if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
        print_error("msgsnd failed for ping message");
        exit(EXIT_FAILURE);
    }
    printf("Producer: Sent ping message: %s\n", message.msg_text);
    
    sleep(1); // 等待消费者处理
    
    // 2. 发送数据消息
    message.msg_type = MSG_TYPE_DATA;
    snprintf(message.msg_text, MAX_MSG_SIZE, "Current time: %s", ctime(&message.msg_type));
    if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
        print_error("msgsnd failed for data message");
        exit(EXIT_FAILURE);
    }
    printf("Producer: Sent data message: %s\n", message.msg_text);
    
    sleep(1);
    
    // 3. 发送更多数据消息
    for (int i = 0; i < 3; i++) {
        message.msg_type = MSG_TYPE_DATA;
        snprintf(message.msg_text, MAX_MSG_SIZE, "Data item %d from producer", i+1);
        if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
            print_error("msgsnd failed for data message");
            exit(EXIT_FAILURE);
        }
        printf("Producer: Sent data message: %s\n", message.msg_text);
        sleep(1);
    }
    
    // 4. 发送退出消息
    message.msg_type = MSG_TYPE_QUIT;
    strncpy(message.msg_text, "Quit command", MAX_MSG_SIZE);
    if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
        print_error("msgsnd failed for quit message");
        exit(EXIT_FAILURE);
    }
    printf("Producer: Sent quit message\n");
    
    printf("Producer: All messages sent. Exiting.\n");
    return 0;
}