#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_FILE "/tmp/msg.key"
#define PROJ_ID   42
#define MSG_TYPE  1

struct MsgBuf {
    long mtype;
    char mtext[256];
};
