# 一句话原理
消息队列 = 内核态链表:

- 每条消息 = { type + 数据块 }
- 进程通过 msgrcv/msgsnd 原子地 push/pop 链表节点
- 链表头保存在 struct msg_queue，由 msgctl(IPC_RMID) 整体释放

## 目录
- [内核数据结构](#1️⃣-内核数据结构linux-6x-源码片段)
- [核心 API](#2️⃣-4-个核心-api)
- [消息结构体](#3️⃣-消息结构体)
- [使用流程](#4️⃣-使用流程)
- [消息类型详解](#5️⃣-消息类型详解)
- [错误处理](#6️⃣-错误处理)
- [实际示例](#7️⃣-实际示例)
- [优缺点分析](#8️⃣-优缺点分析)

## 1️⃣ 内核数据结构（Linux 6.x 源码片段）
路径：ipc/msg.c
```c
struct msg_queue {
    struct kern_ipc_perm q_perm;   // 权限 + key
    time_t               q_stime;  // 最后发送时间
    time_t               q_rtime;  // 最后接收时间
    time_t               q_ctime;  // 最后修改时间
    struct list_head     q_messages; // 链表头，挂接 struct msg_msg
    size_t               q_cbytes;   // 当前字节数
    unsigned long        q_qnum;     // 当前消息条数
    size_t               q_qbytes;   // 队列最大字节数
    pid_t                q_lspid;   // 最后发送进程PID
    pid_t                q_lrpid;   // 最后接收进程PID
};

struct msg_msg {
    struct list_head m_list;   // 链表节点
    long             m_type;   // 消息类型
    size_t           m_ts;     // 正文长度
    void             *mtext[]; // 柔性数组，数据紧跟其后
};
```

- 链表节点 整块分配（kmalloc）→ 减少碎片
- msgrcv 时把节点从链表摘下 → kfree → 原子完成

## 2️⃣ 4 个核心 API
| API                                                                | 作用        | 关键形参陷阱                                          |
| ------------------------------------------------------------------ | --------- | ----------------------------------------------- |
| `int msgget(key_t key, int flag)`                                  | 创建 / 获取队列 | key 用 `ftok(path, proj)`；flag 必须带 `IPC_CREAT`   |
| `int msgsnd(int id, const void *msgp, size_t sz, int flg)`         | 发送        | `sz` **不含 `long mtype`**                        |
| `ssize_t msgrcv(int id, void *msgp, size_t sz, long typ, int flg)` | 接收        | `typ=0` 取第一条；`typ>0` 只取该类型；`flg=IPC_NOWAIT` 非阻塞 |
| `int msgctl(int id, int cmd, struct msqid_ds *buf)`                | 控制        | `cmd=IPC_RMID` 立即删除队列                           |

### 2.1 msgget - 获取消息队列标识符
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgget(key_t key, int msgflg);
```

**参数说明：**
- `key`: 消息队列的键值，通常通过 ftok() 生成
- `msgflg`: 权限标志和控制标志，常用组合：
  - `IPC_CREAT`: 如果不存在则创建
  - `IPC_EXCL`: 配合 IPC_CREAT 使用，如果已存在则报错
  - 权限位：如 0666（读写权限）

**返回值：**
- 成功：消息队列标识符 (msgid)
- 失败：-1，并设置 errno

### 2.2 msgsnd - 发送消息
```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

**参数说明：**
- `msqid`: 消息队列标识符
- `msgp`: 指向消息结构体的指针
- `msgsz`: 消息正文的字节数（不包括 mtype 字段）
- `msgflg`: 控制标志
  - `0`: 阻塞发送，如果队列满则等待
  - `IPC_NOWAIT`: 非阻塞发送，如果队列满则立即返回错误

### 2.3 msgrcv - 接收消息
```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

**参数说明：**
- `msqid`: 消息队列标识符
- `msgp`: 指向接收消息结构体的指针
- `msgsz`: 接收缓冲区大小
- `msgtyp`: 消息类型选择
  - `0`: 读取队列中第一条消息
  - `>0`: 读取类型为 msgtyp 的第一条消息
  - `<0`: 读取类型小于等于 abs(msgtyp) 的最小类型的第一个消息
- `msgflg`: 控制标志
  - `0`: 阻塞接收
  - `IPC_NOWAIT`: 非阻塞接收
  - `MSG_NOERROR`: 消息大小超过 msgsz 时截断而不是出错

### 2.4 msgctl - 控制消息队列
```c
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

**参数说明：**
- `msqid`: 消息队列标识符
- `cmd`: 命令类型
  - `IPC_STAT`: 获取队列状态信息到 buf
  - `IPC_SET`: 设置队列状态信息
  - `IPC_RMID`: 删除队列
- `buf`: 指向 msqid_ds 结构的指针

## 3️⃣ 消息结构体
使用消息队列时，必须定义自己的消息结构体，第一个字段必须是 long 类型的消息类型：

```c
struct my_msg {
    long mtype;        // 消息类型，必须 > 0
    char mtext[100];   // 消息正文，可以是任意类型的数据
};
```

或者更灵活的定义方式：
```c
struct my_msg {
    long mtype;        // 消息类型
    union {
        char text[100];
        int  number;
        struct {
            int id;
            char name[50];
        } student;
    } data;
};
```

## 4️⃣ 使用流程

### 4.1 创建/访问消息队列
```c
key_t key = ftok("/tmp", 'A');
int msgid = msgget(key, IPC_CREAT | 0666);
```

### 4.2 发送消息
```c
struct my_msg msg;
msg.mtype = 1;
strcpy(msg.mtext, "Hello World");

if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
    perror("msgsnd");
}
```

### 4.3 接收消息
```c
struct my_msg msg;
ssize_t bytes_received = msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0);
if (bytes_received == -1) {
    perror("msgrcv");
} else {
    printf("Received: %s\n", msg.mtext);
}
```

### 4.4 删除消息队列
```c
if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    perror("msgctl");
}
```

## 5️⃣ 消息类型详解

消息类型是消息队列中非常重要的概念，它决定了消息的接收方式：

1. **类型为正整数**：类型值必须大于0
2. **接收时的类型选择**：
   - `msgtyp = 0`：接收队列中的第一条消息，不考虑类型
   - `msgtyp > 0`：接收队列中第一个类型为 msgtyp 的消息
   - `msgtyp < 0`：接收队列中类型小于等于 abs(msgtyp) 的最小类型的第一个消息

通过合理使用消息类型，可以实现：
- 优先级队列（小类型值优先）
- 点对点通信（指定类型）
- 广播通信（多个接收者监听同一类型）

## 6️⃣ 错误处理

常见的错误码：
- `EAGAIN`：队列满（非阻塞发送）或队列空（非阻塞接收）
- `EACCES`：没有权限访问队列
- `EIDRM`：队列已被删除
- `EINVAL`：参数无效（如 msgsz 超限）
- `E2BIG`：消息太大
- `ENOMSG`：没有匹配的消息（非阻塞接收）

## 7️⃣ 实际示例

### 7.1 客户端-服务器模型
```c
// 服务器端
key_t key = ftok("/tmp", 'A');
int msgid = msgget(key, IPC_CREAT | 0666);

while (1) {
    struct msg_buf {
        long mtype;
        char mtext[100];
    } msg;
    
    // 接收客户端消息（类型1）
    msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0);
    printf("Received from client: %s\n", msg.mtext);
    
    // 回复客户端（类型2）
    msg.mtype = 2;
    strcpy(msg.mtext, "Hello from server");
    msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
}
```

```c
// 客户端
key_t key = ftok("/tmp", 'A');
int msgid = msgget(key, 0666);

struct msg_buf {
    long mtype;
    char mtext[100];
} msg;

// 发送消息给服务器（类型1）
msg.mtype = 1;
strcpy(msg.mtext, "Hello from client");
msgsnd(msgid, &msg, sizeof(msg.mtext), 0);

// 接收服务器回复（类型2）
msgrcv(msgid, &msg, sizeof(msg.mtext), 2, 0);
printf("Received from server: %s\n", msg.mtext);
```

## 8️⃣ 优缺点分析

### 优点
1. **面向记录**：消息有明确的边界，不会出现粘包问题
2. **异步通信**：发送方和接收方不需要同时活跃
3. **类型区分**：可以通过消息类型实现选择性接收
4. **内核持久性**：消息在内核中保存，即使进程退出也不会丢失
5. **流控制**：可以设置队列大小限制，防止发送方过快

### 缺点
1. **平台相关**：不同系统间不兼容
2. **容量限制**：受内核参数限制（消息大小、队列长度等）
3. **性能开销**：涉及用户态和内核态的数据拷贝
4. **命名空间限制**：key 值可能冲突
5. **资源管理**：需要显式删除，否则会残留内核中

### 适用场景
- 进程间通信
- 解耦发送方和接收方
- 实现简单的消息中间件
- 需要消息类型区分的场景