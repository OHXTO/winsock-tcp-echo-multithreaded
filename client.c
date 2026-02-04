#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
//#pragma comment(lib, "ws2_32.lib")
//  gcc client.c -o client.exe -lws2_32

int main()
{

    // windows 给网络权限| network authorize
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 1. 创建socket

    // SOCKET socket(
    // 	int af,      // 协议地址簇 IPV4(AF_INET) IPV6(AF_INET6) | Protocol address family
    // 	int type,    // 类型 流式协议TCP (SOCK_STREAM) 帧式协议(SOCK_DGRAM)
    // 	int protocol // 保护协议 0 | Protection Protocol 0 not used
    // );

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(INVALID_SOCKET == client_socket){
        printf("create socket failed !!\n");
        return -1;
    }


    // 2. 连接服务器 | connect server

    // typedef struct sockaddr_in{
    //     ADDRESS_FAMILY sin_family;  // 协议地址簇
    //     USHORT sin_port;            // 端口号               2 byte
    //     IN_ADDR sin_addr;           // IP address          4 byte
    //     CHAR sin_zero[8];           // 保留字节？协议升级    8 byte
    // };

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(8080);
    target.sin_addr.s_addr = inet_addr("127.0.0.1");


    if ( -1 == connect(client_socket, (struct sockaddr*)&target, sizeof(target))){
        printf("connect server failed !!\n");
        closesocket(client_socket);
        return -1;
    }


    // 3. 开始通讯 | start communication

    // recv(
        //     SOCKET s,   // 客户端 client socket
        //     char* buf,  // received data saved
        //     int len,    // length
        //     int flags   // 0
        // )
    
    while (1)
    {
        char sbuffer[1024] = {0};
        printf("Please Enter>\n");
        fgets(sbuffer, sizeof(sbuffer), stdin); // fgets can include space

        send(client_socket, sbuffer, (int)strlen(sbuffer), 0); // send | blocking

        char rbuffer[1024] = {0};
        int ret = recv(client_socket, rbuffer, 1024, 0);
        if (ret <= 0) break;    // when client closed
        printf("[RECEIVED] %s\n", rbuffer);

    }


    // 4. 关闭连接 | Close connection

    closesocket(client_socket);

    return 0;
}