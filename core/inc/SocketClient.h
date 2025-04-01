////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NAME:  ISocketClient.h
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

#ifndef ISOCKETCLIENT_H
#define ISOCKETCLIENT_H
#ifdef WIN
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <iostream>
#include <list>

#define PORT 7000

class SocketClient
{

private:
    static SocketClient* mSocketClient;
    SocketClient *client;
    int sock_cli;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;
    bool isConnected;
    struct sockaddr_in servaddr;

public:

    /**
     * @brief SocketClient - Constructor
     */
    SocketClient();

    /**
     * Singletons should not be assignable.
     */
    void operator=(const SocketClient &) = delete;

    ~SocketClient();

    void resetSocket();
    int receiveVehicleData(char data[]);
    void closeSocket();
    int sendVehicleData(const char *data);
    bool connected();
    bool connectToServer(const char* ip, const unsigned int port);


};

#endif // ISOCKETCLIENT_H
