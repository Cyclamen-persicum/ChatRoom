#include "pch.h"

#pragma comment(lib,"ws2_32.lib")

SOCKET clientSocket[1024];
int count = 0;

DWORD communication(LPVOID _clientNum)
{
    //7 通信

    char buff[256];
    char clientStr[256];
    int clientNum = (int)_clientNum;
    while (1)
    {
        memset(buff, 0, 256);
        int receiveFlag = recv(clientSocket[clientNum], buff, 255, NULL);
        if (receiveFlag > 0)
        {
            std::cout << clientNum <<"："<< buff << std::endl;
            _itoa(clientNum, clientStr, 10);
            strcat(clientStr, " ");
            strcat(clientStr, buff);
            for (int clientNum_ = 0; clientNum_ < count; ++clientNum_)
            {
                send(clientSocket[clientNum_], clientStr, strlen(clientStr), NULL);
            }
        }
    }
}



int main()
{
    //1 请求版本协议

    //定型文开始
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        std::cout << "请求版本失败!" << std::endl;
        return -1;
    }
    std::cout << "请求版本成功!" << std::endl;
    //定型文结束

    //2 创建socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "创建socket失败!" << std::endl;
        return -1;
    }
    std::cout << "创建socket成功!" << std::endl;

    //3 创建协议地址族
    //包括协议，主机，网络端口
    SOCKADDR_IN serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(IPAddress);
    serverAddr.sin_port = htons(10086);

    //4 绑定
    int bindFlag = bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bindFlag == -1)
    {
        std::cout << "绑定失败!" << std::endl;
        return -1;
    }
    std::cout << "绑定成功!" << std::endl;

    //5 监听
    auto listenFlag = listen(serverSocket, 10);
    if (listenFlag == -1)
    {
        std::cout << "监听失败!" << std::endl;
        return -1;
    }
    std::cout << "监听成功!" << std::endl;

    //6 等待客户端连接
    SOCKADDR_IN clientAddr = { 0 };
    int length = sizeof(clientAddr);
    for (int clientNum = 0; clientNum < 1024; ++clientNum)
    {
        clientSocket[clientNum] = accept(serverSocket, (sockaddr *)&clientAddr, &length);
        if(clientSocket[clientNum] == SOCKET_ERROR)
        {
            std::cout << "错误的客户端!" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        std::cout << "客户端" << inet_ntoa(clientAddr.sin_addr) << "连入" << std::endl;
        count++;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, (LPVOID)clientNum, NULL, NULL);
    }
    
    while (1);
    return 0;
}


