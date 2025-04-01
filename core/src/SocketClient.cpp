////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NAME:  ISocketClient.cpp
//
//  PURPOSE:
//
//  COPYRIGHT NOTICE:
//  (C) KPIT Technologies Ltd
//  Created in 2015 as an unpublished copyright work.
//  All rights reserved.
//  This document and the information it contains is
//  confidential and proprietary to KPIT Technologies Ltd. Hence, it may
//  not be used, copied, reproduced, transmitted, or stored in any
//  form or by any means, electronic, recording, photocopying,
//  mechanical or otherwise, without the prior written
//  permission of KPIT Technologies Ltd.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "core/inc/SocketClient.h"
#define BUFFER_SIZE 1024

SocketClient::SocketClient()
{
    /// Define sockfd

#ifdef WIN
    WSADATA wsaData;
    // Initialize Winsock version 2.2
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        // MSDN: An application can call the WSAGetLastError() function to determine the
        // extended error code for other Windows sockets functions as is normally
        // done in Windows Sockets even if the WSAStartup function fails or the WSAStartup
        // function was not called to properly initialize Windows Sockets before calling a
        // Windows Sockets function. The WSAGetLastError() function is one of the only functions
        // in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
        printf("Server: WSAStartup failed with error %d\n", WSAGetLastError());
        // Exit with error
    }
    else
    {
        printf("Server: The Winsock dll found!\n");
        printf("Server: The current status is %s.\n", wsaData.szSystemStatus);
    }
#endif
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
}

SocketClient::~SocketClient()
{
    closeSocket();
}

void SocketClient::resetSocket() {
    closeSocket();
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    if (sock_cli < 0) {
        std::cout << "Socket creation failed" << std::endl;
    }
}

bool SocketClient::connectToServer(const char* ip, const unsigned int port)
{

    /// Define sockaddr_in
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  /// Server Port
    servaddr.sin_addr.s_addr = inet_addr(ip);  /// server ip
    isConnected = true;

    //Connect to the server, successfully return 0, error return - 1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cout<< "Client "  << sock_cli << std::endl;
        close(sock_cli);
        isConnected = false;
        return isConnected;
        perror("connect");
        exit(1);
    }
    return isConnected;
}
bool SocketClient::connected()
{
    return isConnected;
};
int SocketClient::receiveVehicleData(char data[])
{
    int len = ::recvfrom(sock_cli, data, 300, 0, NULL, 0);
    if(0<len)
        printf("receiveVehicleData: %s\n", data);
//    /*Clear the collection of readable file descriptors*/
//    FD_ZERO(&rfds);
//    /*Add standard input file descriptors to the collection*/
//    FD_SET(0, &rfds);
//    maxfd = 0;
//    /*Add the currently connected file descriptor to the collection*/
//    FD_SET(sock_cli, &rfds);
//    /*Find the largest file descriptor in the file descriptor set*/
//    if(maxfd < sock_cli)
//        maxfd = sock_cli;
//    /*Setting timeout time*/
//    tv.tv_sec = 5;
//    tv.tv_usec = 0;
//    /*Waiting for data*/
//    retval = select(1, &rfds, NULL, NULL, &tv);

//    /*The server sent a message.*/
//    if(FD_ISSET(sock_cli,&rfds)){
//        char recvbuf[BUFFER_SIZE];
//        int len;
//        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
//        printf("%s\n", recvbuf);
//        strcpy(data,recvbuf);
//        memset(recvbuf, 0, sizeof(recvbuf));
//    }
//    /*When the user enters the information, he begins to process the information and send it.*/
//    if(FD_ISSET(0, &rfds)){
//        char sendbuf[BUFFER_SIZE];
//        fgets(sendbuf, sizeof(sendbuf), stdin);
//        send(sock_cli, sendbuf, strlen(sendbuf),0); //Send out
//        memset(sendbuf, 0, sizeof(sendbuf));
//    }
    return retval;
}

void SocketClient::closeSocket()
{

#ifndef WIN
    shutdown(1, SHUT_RDWR);
#endif
    ::close(sock_cli);
    sock_cli = -1;
}
int SocketClient::sendVehicleData(const char* data)
{
    char buf[1024];
    strcpy(buf, data);
    std::list<int>::iterator it;
    //Send out
    int ret = send(sock_cli, buf, strlen(buf),0);
    std::cout << "sendVehicleData ret: " << ret << std::endl;
    return ret;
}
