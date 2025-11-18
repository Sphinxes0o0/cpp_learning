#include "msg_common.h"
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int msgid;
    struct msg_buffer message;
    int producer_id = 0;
    
    // 如果提供了生产者ID参数
    if (argc > 1) {
        producer_id = atoi(argv[1]);
    }
    
    // 创建或获取消息队列
    msgid = create_msg_queue();
    if (msgid == -1) {
        exit(EXIT_FAILURE);
    }
    
    printf("Producer %d: Message queue created with id %d\n", producer_id, msgid);
    
    // 发送10条数据消息
    for (int i = 1; i <= 10; i++) {
        message.msg_type = MSG_TYPE_DATA;
        
        // 获取当前时间作为消息内容的一部分
        struct timeval tv;
        gettimeofday(&tv, NULL);
        
        snprintf(message.msg_text, MAX_MSG_SIZE, 
                "Producer %d - Message %d (timestamp: %ld.%06ld)", 
                producer_id, i, tv.tv_sec, (long)tv.tv_usec);
        
        if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
            print_error("msgsnd failed for data message");
            exit(EXIT_FAILURE);
        }
        printf("Producer %d: Sent message: %s\n", producer_id, message.msg_text);
        sleep(1); // 每秒发送一条消息
    }
    
    // 如果是第一个生产者，发送退出消息
    if (producer_id == 1) {
        message.msg_type = MSG_TYPE_QUIT;
        snprintf(message.msg_text, MAX_MSG_SIZE, "QUIT command from producer %d", producer_id);
        if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
            print_error("msgsnd failed for quit message");
            exit(EXIT_FAILURE);
        }
        printf("Producer %d: Sent quit message\n", producer_id);
    }
    
    printf("Producer %d: All messages sent. Exiting.\n", producer_id);
    return 0;
}