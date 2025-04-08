////////////////////////////////////////////////////////////////////////////////////
// NAME:  IDataSink.h
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef IDATASINK_H
#define IDATASINK_H

class IDataSink {
 public:
  virtual ~IDataSink() {}
  virtual bool Connect(const char *ip, const unsigned int port) = 0;
  virtual int SendData(const char *data) = 0;
  virtual int RecieveData(char data[]) = 0;
};

#endif  // IDATASINK_H
