////////////////////////////////////////////////////////////////////////////////////
// NAME:  SimTypes.h
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
#ifndef SIMTYPES_H
#define SIMTYPES_H

#include <QAbstractListModel>
#include <QMap>

#define PORT 7000

struct SignalData {
  int id;
  QString name;
  QString mode;
  QString period;
  QString value;
};

#endif  // SIMTYPES_H
