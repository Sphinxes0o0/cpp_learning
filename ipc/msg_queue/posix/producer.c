#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#include "common.h"

int main(int argc, char* argv[]) {
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = MQ_MAXMSG,
        .mq_msgsize = MQ_MSGSIZE,
        .mq_curmsgs = 0
    };

    mqd_t mq = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    char buf[MQ_MSGSIZE];
    uint8_t prio = 0;
    while (fgets(buf, MQ_MSGSIZE, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (mq_send(mq, buf, strlen(buf), prio) == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
        if (strcmp(buf, "quit") == 0) break;
        prio = (prio + 1) % 10;
    }

    mq_close(mq);

    return 0;
}