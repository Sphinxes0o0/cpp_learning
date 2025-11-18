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

/**
 * Client connection state for managing file transfers
 */
typedef struct {
    int fd;
    FILE *file;
    char filename[256];
    struct sockaddr_in addr;
} client_conn_t;

/**
 * Set socket to non-blocking mode
 */
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        return -1;
    }
    
    return 0;
}

/**
 * Create and initialize a new client connection
 */
client_conn_t* create_client_conn(int fd, struct sockaddr_in *addr) {
    client_conn_t *conn = malloc(sizeof(client_conn_t));
    if (!conn) {
        perror("malloc");
        return NULL;
    }
    
    conn->fd = fd;
    conn->addr = *addr;
    
    // Generate filename based on client IP
    inet_ntop(AF_INET, &addr->sin_addr, conn->filename, INET_ADDRSTRLEN);
    strcat(conn->filename, "_file.dat");
    
    // Open file for writing
    conn->file = fopen(conn->filename, "wb");
    if (!conn->file) {
        perror("fopen");
        free(conn);
        return NULL;
    }
    
    printf("New client connected from %s, saving to %s\n", 
           inet_ntoa(addr->sin_addr), conn->filename);
    
    return conn;
}

/**
 * Clean up client connection
 */
void cleanup_client_conn(client_conn_t *conn) {
    if (!conn) return;
    
    if (conn->file) {
        fclose(conn->file);
        printf("File '%s' received and saved from %s\n", 
               conn->filename, inet_ntoa(conn->addr.sin_addr));
    }
    
    close(conn->fd);
    free(conn);
}

/**
 * Handle data from client (non-blocking)
 */
int handle_client_data(client_conn_t *conn) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    
    while ((bytes_read = recv(conn->fd, buffer, BUF_SIZE, 0)) > 0) {
        if (fwrite(buffer, sizeof(char), bytes_read, conn->file) != bytes_read) {
            perror("fwrite");
            return -1;
        }
    }
    
    if (bytes_read == 0) {
        // Client closed connection
        return 0;
    } else if (bytes_read == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No more data available now
            return 1;
        } else {
            perror("recv");
            return -1;
        }
    }
    
    return 1;
}

int main() {
    int server_fd, epoll_fd;
    struct sockaddr_in address;
    struct epoll_event event, events[MAX_EVENTS];
    
    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Set non-blocking
    if (set_nonblocking(server_fd) == -1) {
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Enable address reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Bind to address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Start listening
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Add server socket to epoll
    event.data.fd = server_fd;
    event.events = EPOLLIN | EPOLLET; // Edge-triggered
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", SERVER_PORT);
    
    // Main event loop
    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            break;
        }
        
        for (int i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || 
                (events[i].events & EPOLLHUP) || 
                (!(events[i].events & EPOLLIN))) {
                
                // Error or hang-up
                fprintf(stderr, "epoll error on fd %d\n", events[i].data.fd);
                if (events[i].data.fd != server_fd) {
                    cleanup_client_conn((client_conn_t*)events[i].data.ptr);
                }
                continue;
            }
            
            if (server_fd == events[i].data.fd) {
                // New connection(s) available
                while (1) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                    
                    if (client_fd == -1) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            break; // All connections processed
                        } else {
                            perror("accept");
                            break;
                        }
                    }
                    
                    // Set client socket non-blocking
                    if (set_nonblocking(client_fd) == -1) {
                        close(client_fd);
                        continue;
                    }
                    
                    // Create client connection state
                    client_conn_t *conn = create_client_conn(client_fd, &client_addr);
                    if (!conn) {
                        close(client_fd);
                        continue;
                    }
                    
                    // Add client to epoll
                    event.data.ptr = conn;
                    event.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                        perror("epoll_ctl: client_fd");
                        cleanup_client_conn(conn);
                        continue;
                    }
                }
            } else {
                // Data available on client socket
                client_conn_t *conn = (client_conn_t*)events[i].data.ptr;
                int result = handle_client_data(conn);
                
                if (result <= 0) {
                    // Connection closed or error
                    cleanup_client_conn(conn);
                }
            }
        }
    }
    
    close(server_fd);
    close(epoll_fd);
    return 0;
} 