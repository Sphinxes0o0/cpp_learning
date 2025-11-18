#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "common.h"


int main(int argc, char const *argv[]) {

    mqd_t mq = mq_open(MQ_NAME, O_RDWR);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    char buf[MQ_MSGSIZE];
    uint8_t prio;
    ssize_t n;
    while ((n = mq_receive(mq, buf, MQ_MSGSIZE, &prio)) != -1) {
        printf("[prio=%u] %s\n", prio, buf);
        if (strcmp(buf, "exit") == 0) break;
    }
    perror("mq_receive");
    mq_close(mq);
    mq_unlink(MQ_NAME);   // 删除队列

    return 0;
}
