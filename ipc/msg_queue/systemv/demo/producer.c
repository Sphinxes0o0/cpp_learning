#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <signal.h>

#include "msg_common.h"

void sig_handler(int signo) {
    if (signo == SIGINT) {
        msgctl(msgget(ftok(KEY_FILE, PROJ_ID), 0), IPC_RMID, NULL);
        exit(0);
    }
}


int main() {
    key_t key = ftok(KEY_FILE, PROJ_ID);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct MsgBuf msg = {
        .mtype = MSG_TYPE,
    };

    signal(SIGINT, sig_handler);

    while (fgets(msg.mtext, sizeof(msg.mtext), stdin)) {
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd"); break;
        }
        if (strcmp(msg.mtext, "exit") == 0) break;
    }

    msgctl

    return 0;
}