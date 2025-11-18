#include "msg_common.h"

int main() {
    int msgid;
    struct msg_buffer message;
    ssize_t bytes_received;
    
    // 获取消息队列
    msgid = get_msg_queue();
    if (msgid == -1) {
        printf("Consumer: Message queue does not exist. Please run producer first to create it.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Consumer: Connected to message queue with id %d\n", msgid);
    printf("Consumer: Waiting for messages...\n");
    
    // 循环接收消息
    while (1) {
        // 接收任何类型的消息
        bytes_received = msgrcv(msgid, &message, MAX_MSG_SIZE, 0, 0);
        if (bytes_received == -1) {
            print_error("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        
        printf("Consumer: Received message (type: %ld, size: %zd): %s\n", 
               message.msg_type, bytes_received, message.msg_text);
        
        // 根据消息类型处理消息
        switch (message.msg_type) {
            case MSG_TYPE_PING:
                printf("Consumer: Received ping, sending pong...\n");
                // 回复pong消息
                message.msg_type = MSG_TYPE_PONG;
                snprintf(message.msg_text, MAX_MSG_SIZE, "Pong from consumer (PID: %d)", getpid());
                if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
                    print_error("msgsnd failed for pong message");
                } else {
                    printf("Consumer: Sent pong message: %s\n", message.msg_text);
                }
                break;
                
            case MSG_TYPE_PONG:
                printf("Consumer: Received pong response\n");
                break;
                
            case MSG_TYPE_DATA:
                printf("Consumer: Processing data...\n");
                break;
                
            case MSG_TYPE_QUIT:
                printf("Consumer: Received quit command. Exiting.\n");
                // 删除消息队列
                if (remove_msg_queue(msgid) == 0) {
                    printf("Consumer: Message queue removed successfully.\n");
                }
                exit(EXIT_SUCCESS);
                break;
                
            default:
                printf("Consumer: Unknown message type: %ld\n", message.msg_type);
                break;
        }
        
        sleep(1); // 模拟处理时间
    }
    
    return 0;
}