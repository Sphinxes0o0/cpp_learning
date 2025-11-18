#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "msg_common.h"

int main() {
    key_t key = ftok(KEY_FILE, PROJ_ID);
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct MsgBuf msg;
    while (1) {
        ssize_t n = msgrcv(msgid, &msg, sizeof(msg.mtext), MSG_TYPE, 0);
        if (n == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Consumer received: %s\n", msg.mtext);
        if (strcmp(msg.mtext, "exit") == 0)
            break;
    }

    return 0;
}