#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

// #pragma comment(lib, "ws2_32.lib") not work in vscode
//  gcc server.c -o server.exe -lws2_32

// typedef DWORD(WINAPI *PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
DWORD WINAPI thread_func(LPVOID lpThreadParameter)
{
    // 传参数client_socket 释放不用的地址lpThreadParameter
    SOCKET client_socket = *(SOCKET*)lpThreadParameter;
    free(lpThreadParameter);

    while (1)   // 客户端关闭跳出 keep looping until client close
        {
            // 5. 开始通讯 (Browser/Server) | start communication
            char buffer[1024] = {0};

            // recv(
            //     SOCKET s,   // 客户端 client socket
            //     char* buf,  // received data saved
            //     int len,    // length
            //     int flags   // 0
            // )

            int ret = recv(client_socket, buffer, 1024, 0); //received | blocking
            if(ret <=0) break;      // when client closed & disconnect
            printf("%llu: %s\n",client_socket, buffer); // llu is unsigned long long

            send(client_socket, buffer, (int)strlen(buffer),0);
        }
    
    //disconnect inform
    printf("socket: %llu disconnect\n", client_socket);
    // 6. 关闭连接 | close connection
    closesocket(client_socket);
    
    return 0;
}



int main()
{
    // windows 给网络权限| network authorize
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 1. 创建socket套接字 | creating socket

    // SOCKET socket(
    // 	int af,      // 协议地址簇 IPV4(AF_INET) IPV6(AF_INET6) | Protocol address family
    // 	int type,    // 类型 流式协议TCP (SOCK_STREAM) 帧式协议(SOCK_DGRAM)
    // 	int protocol // 保护协议 0 | Protection Protocol 0 not used
    // );

    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == listen_socket) // INVALID_SOCKET = -1
    {
        printf("create listen socket failed !! errocode: %d\n", GetLastError());
        return -1;
    }

    // 2. socket绑定端口号 | socket binds to a port number

    // typedef struct sockaddr_in{
    //     ADDRESS_FAMILY sin_family;  // 协议地址簇
    //     USHORT sin_port;            // 端口号               2 byte
    //     IN_ADDR sin_addr;           // IP address          4 byte
    //     CHAR sin_zero[8];           // 保留字节？协议升级    8 byte
    // };


    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_port = htons(8080);       // 大小端问题用htons/htonl解决: 中间设备(路由器)大端序，数据位反转
                                        // Big/little endian
    
    // Server options 网卡选项 127.0.0.1(本地环回BACKLOOP) 只接受哪个网卡的数据，0地址表全部接收
    //全接收htonl(INADDR_ANY); 或简单inet_addr-字符串IP地址转化为整数IP
    local.sin_addr.s_addr = inet_addr("0.0.0.0");
    
    // int bind(
    //     SOCKET s,
    //     const struct sockaddr * name,
    //     int namelen
    // );

    if ( -1 == bind(listen_socket, (struct sockaddr*)&local, sizeof(local))){
        printf("bind socket failed !! errcode: %d\n", GetLastError());
        return -1;
    }
    
   
    // 3. socket开启监听属性，只用来接收连接 | socket for connection
    // int listen(
    //     SOCKET s,
    //     int backlog // 半连接队列长度，一般10
    // );

    if( -1 == listen(listen_socket, 10)){
        printf("start listen socket failed !! errcode: %d\n", GetLastError());
        return -1;
    }


    // 4. 等待客户端连接 | wait for client connection
    // 返回的客户端socket才是可以通讯的socket | returned client socket is communicatable socket
    // 阻塞函数，等有客户端连接进来就接受，然后返回，否则阻塞 | blocking
    // SOCKET accept(
    //     SOCKET s,                // 监听listen socket
    //     struct sockaddr* addr,   // 客户端IP address和端口号port
    //     int* addrlen             // 结构大小 size
        
    // );

    while(1)        // continue until connect
    {
        SOCKET client_socket = accept(listen_socket, NULL, NULL);
        if(INVALID_SOCKET == client_socket){
            continue;
        }

        // New connect inform
        printf("new connect socket: %llu\n", client_socket);
        
        // 动态空间 | dynamic space for client_socket in case it die before create threads
        SOCKET* sockfd = (SOCKET*)malloc(sizeof(SOCKET));
        *sockfd = client_socket;

        // 创建线程 | Create Socket, default 0 and NULL
        // 调用 自定义thread_func: 
        CreateThread(NULL, 0, thread_func, sockfd, 0, NULL);
 
        
    
    }

    return 0;
}
