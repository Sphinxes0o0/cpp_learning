#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>

#ifdef __APPLE__
#include <net/if_dl.h>  // 添加这个头文件，包含 struct sockaddr_dl 和 LLADDR
#endif

#ifndef AF_PACKET
#define AF_PACKET AF_LINK
#endif

/**
 * 使用 ioctl 方式获取网络接口信息
 * 适配了 macOS 的特殊处理方式
 */
void get_interfaces_by_ioctl() {
    int sock;
    struct ifconf ifc;
    char buf[1024];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return;
    }

    // 准备缓冲区
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;

    // 获取接口列表
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {
        perror("ioctl(SIOCGIFCONF)");
        close(sock);
        return;
    }

    printf("=== Using ioctl method ===\n");

    // 在 macOS 上遍历接口需要考虑变长结构
    char *ptr = buf;
    char *end = buf + ifc.ifc_len;
    
    while (ptr < end) {
        struct ifreq *ifr = (struct ifreq *)ptr;
        
        printf("Interface: %s\n", ifr->ifr_name);
        
        // 获取 IP 地址
        struct ifreq addr_req = *ifr;
        if (ioctl(sock, SIOCGIFADDR, &addr_req) != -1) {
            if (addr_req.ifr_addr.sa_family == AF_INET) {
                struct sockaddr_in *sin = (struct sockaddr_in *)&addr_req.ifr_addr;
                printf("\tIP Address: %s\n", inet_ntoa(sin->sin_addr));
            }
        }

        // 获取标志位
        struct ifreq flag_req = *ifr;
        if (ioctl(sock, SIOCGIFFLAGS, &flag_req) != -1) {
            printf("\tFlags: 0x%X", flag_req.ifr_flags);
            if (flag_req.ifr_flags & IFF_UP) printf(" UP");
            if (flag_req.ifr_flags & IFF_RUNNING) printf(" RUNNING");
            if (flag_req.ifr_flags & IFF_LOOPBACK) printf(" LOOPBACK");
            printf("\n");
        }

        // 获取网络掩码
        struct ifreq netmask_req = *ifr;
        if (ioctl(sock, SIOCGIFNETMASK, &netmask_req) != -1) {
            if (netmask_req.ifr_addr.sa_family == AF_INET) {
                struct sockaddr_in *sin = (struct sockaddr_in *)&netmask_req.ifr_addr;
                printf("\tNetmask: %s\n", inet_ntoa(sin->sin_addr));
            }
        }

        printf("\n");
        
        // 计算下一个接口的偏移 - macOS 特有的处理方式
#ifdef __APPLE__
        size_t len = IFNAMSIZ + ifr->ifr_addr.sa_len;
#else
        size_t len = sizeof(struct ifreq);
#endif
        ptr += len;
    }

    close(sock);
}

/**
 * 使用 getifaddrs 方式获取网络接口信息
 * 更现代且跨平台的方法
 */
void get_interfaces_by_getifaddrs() {
    struct ifaddrs *ifaddr;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    printf("=== Using getifaddrs method ===\n");

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        /* 显示接口名称和地址族 */
        printf("%-8s %s (%d)\n",
                ifa->ifa_name,
#ifdef __APPLE__
                (family == AF_LINK) ? "AF_LINK" :
#else
                (family == AF_PACKET) ? "AF_PACKET" :
#endif
                (family == AF_INET) ? "AF_INET" :
                (family == AF_INET6) ? "AF_INET6" : "???",
                family);

        /* 对于 AF_INET* 接口地址，显示地址信息 */
        if (family == AF_INET || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                            sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                continue;
            }

            printf("\t\taddress: <%s>\n", host);

            // 显示网络掩码
            if (ifa->ifa_netmask != NULL && family == AF_INET) {
                s = getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                if (s == 0) {
                    printf("\t\tnetmask: <%s>\n", host);
                }
            }

#ifdef __APPLE__
        } else if (family == AF_LINK) {
            struct sockaddr_dl *sdl = (struct sockaddr_dl *)ifa->ifa_addr;
            if (sdl->sdl_alen == 6) {  // MAC 地址长度
                unsigned char *mac = (unsigned char *)LLADDR(sdl);
                printf("\t\tMAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            }
#endif
        }

        // 显示接口标志
        if (ifa->ifa_flags) {
            printf("\t\tflags: 0x%x", ifa->ifa_flags);
            if (ifa->ifa_flags & IFF_UP) printf(" UP");
            if (ifa->ifa_flags & IFF_RUNNING) printf(" RUNNING");
            if (ifa->ifa_flags & IFF_LOOPBACK) printf(" LOOPBACK");
            printf("\n");
        }

        printf("\n");
    }

    freeifaddrs(ifaddr);
}

int main(int argc, char** argv) {
    printf("Network Interface Information\n");
    printf("=============================\n\n");

    // 使用 getifaddrs 方法（推荐）
    get_interfaces_by_getifaddrs();

    printf("\n");

    // 使用 ioctl 方法（传统方法）
    get_interfaces_by_ioctl();

    return 0;
}

