// client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#pragma comment(lib,"ws2_32.lib")

SOCKET clientSocket;

void recvAndshow()
{
    char buff[256];
    for(auto lineNum = 0;;++lineNum)
    {
        memset(buff, 0, 256);
        auto recvFlag = recv(clientSocket, buff, 256, NULL);
        if (recvFlag > 0)
        {
            outtextxy(0, lineNum * 20, buff);
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
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "创建socket失败!" << std::endl;
        return -1;
    }
    std::cout << "创建socket成功!" << std::endl;

    system("cls");
    //附加，自选服务器
    std::string serverIPAddr;
    std::cout << "请输入服务器地址：";
    std::cin >> serverIPAddr;
    //3 获取服务器协议地址族
    //包括协议，主机，网络端口
    SOCKADDR_IN serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(serverIPAddr.c_str());
    serverAddr.sin_port = htons(10086);

    //4 连接服务器
    auto connectFlag = connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (connectFlag == -1)
    {
        std::cout << "连接服务器失败!" << std::endl;
        return -1;
    }
    std::cout << "连接服务器成功!" << std::endl;

    //附加，创建一个用于接受数据的窗口
    initgraph(300, 300, SHOWCONSOLE);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recvAndshow, NULL, NULL, NULL);

    //5 通信
    while (1)
    {
        char buff[256];
        while (1)
        {
            memset(buff, 0, 256);
            std::cout << "请输入要发送的文字：";
            std::cin >> buff;
            int receiveFlag = send(clientSocket, buff, 255, NULL);
            if (receiveFlag > 0)
            {
                std::cout << buff << std::endl;
            }
        }
    }


    while (1);
    return 0;
}


