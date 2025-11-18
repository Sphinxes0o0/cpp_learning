#include "msg_common.h"
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int msgid;
    struct msg_buffer message;
    ssize_t bytes_received;
    int consumer_id = 0;
    int msg_count = 0;
    
    // 如果提供了消费者ID参数
    if (argc > 1) {
        consumer_id = atoi(argv[1]);
    }
    
    // 获取消息队列
    msgid = get_msg_queue();
    if (msgid == -1) {
        printf("Consumer %d: Message queue does not exist. Please run producer first to create it.\n", consumer_id);
        exit(EXIT_FAILURE);
    }
    
    printf("Consumer %d: Connected to message queue with id %d\n", consumer_id, msgid);
    printf("Consumer %d: Waiting for messages...\n", consumer_id);
    
    // 循环接收消息
    while (1) {
        // 接收任何类型的消息
        bytes_received = msgrcv(msgid, &message, MAX_MSG_SIZE, 0, 0);
        if (bytes_received == -1) {
            print_error("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        
        msg_count++;
        printf("Consumer %d: Received message #%d (type: %ld, size: %zd): %s\n", 
               consumer_id, msg_count, message.msg_type, bytes_received, message.msg_text);
        
        // 根据消息类型处理消息
        switch (message.msg_type) {
            case MSG_TYPE_PING:
                printf("Consumer %d: Received ping\n", consumer_id);
                break;
                
            case MSG_TYPE_PONG:
                printf("Consumer %d: Received pong\n", consumer_id);
                break;
                
            case MSG_TYPE_DATA:
                printf("Consumer %d: Processing data...\n", consumer_id);
                // 模拟处理时间
                sleep(1);
                break;
                
            case MSG_TYPE_QUIT:
                printf("Consumer %d: Received quit command. Exiting after processing %d messages.\n", 
                       consumer_id, msg_count);
                exit(EXIT_SUCCESS);
                break;
                
            default:
                printf("Consumer %d: Unknown message type: %ld\n", consumer_id, message.msg_type);
                break;
        }
    }
    
    return 0;
}