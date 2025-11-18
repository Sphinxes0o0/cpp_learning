#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MQ_NAME  "/demo_posix_mq"
#define MQ_MAXMSG 10          // 最多 10 条
#define MQ_MSGSIZE 256        // 每条最大 256 字节

