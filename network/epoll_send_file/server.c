#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUF_SIZE 1024
#define SERVER_PORT 12345

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        return -1;
    }
    return 0;
}

void handle_connection(int new_socket, struct sockaddr_in *client_addr) {
    char buffer[BUF_SIZE];
    char file_name[64]; // 增加了缓冲区大小以适应IP地址字符串

    // 将客户端的IP地址转换为字符串形式，并用它来构造文件名
    inet_ntop(AF_INET, &client_addr->sin_addr, file_name, INET_ADDRSTRLEN);
    strcat(file_name, "_file.dat");

    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        perror("fopen");
        close(new_socket);
        return;
    }

    ssize_t read_bytes;
    while ((read_bytes = recv(new_socket, buffer, BUF_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), read_bytes, file);
    }

    if (read_bytes == -1 && errno != EAGAIN) {
        perror("read");
    }

    fclose(file);
    close(new_socket);
    printf("File '%s' received and saved from %s.\n", file_name, inet_ntoa(client_addr->sin_addr));
}

int main() {
    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address;
    struct epoll_event event, events[MAX_EVENTS];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (set_nonblocking(server_fd) == -1) {
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    event.data.fd = server_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            } else if (server_fd == events[i].data.fd) {
                // We have a notification on the listening socket, which means one or more incoming connections.
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len = sizeof(in_addr);
                    new_socket = accept(server_fd, &in_addr, &in_len);
                    if (new_socket == -1) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) 
                            break; // We have processed all incoming connections.
                        else {
                            perror("accept");
                            break;
                        }
                    }

                    if (set_nonblocking(new_socket) == -1) 
                        abort();


                    event.data.fd = new_socket;
                    event.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                        perror("epoll_ctl");
                        abort();
                    }
                }
                continue;
            } else {
                // We have data on the fd waiting to be read. Read and display it.
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                getpeername(events[i].data.fd, (struct sockaddr *)&client_addr, &client_len);

                new_socket = events[i].data.fd;
                handle_connection(events[i].data.fd, &client_addr);
            }
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;
}