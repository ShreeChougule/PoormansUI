////////////////////////////////////////////////////////////////////////////////////
// NAME:  ISimService.h
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

#ifndef ISIMSERVICE_H
#define ISIMSERVICE_H

class ISimService {
 public:
  ~ISimService() {}
  virtual bool InitializeConnection(const char *ip,
                                    const unsigned int port) = 0;
  virtual int SendVehicleData(const char *data) = 0;
};

#endif  // ISIMSERVICE_H
