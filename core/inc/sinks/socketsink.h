////////////////////////////////////////////////////////////////////////////////
// NAME:  socketsink.h
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
////////////////////////////////////////////////////////////////////////////////
#ifndef SOCKETSINK_H
#define SOCKETSINK_H
#ifdef WIN
#include <windows.h>
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <list>

#include "core/api/IDataSink.h"

class SocketSink : public IDataSink {
 private:
  static SocketSink *mSocketSink;
  SocketSink *client;
  int sock_cli;
  fd_set rfds;
  struct timeval tv;
  int retval, maxfd;
  bool isConnected;
  struct sockaddr_in servaddr;

 public:
  /**
   * @brief SocketSink - Constructor
   */
  SocketSink();

  /**
   * Singletons should not be assignable.
   */
  void operator=(const SocketSink &) = delete;

  ~SocketSink();

  void resetSocket();
  int RecieveData(char data[]);
  void closeSocket();
  int SendData(const char *data);
  bool connected();
  virtual bool Connect(const char *ip, const unsigned int port);
};

#endif  // SOCKETSINK_H
