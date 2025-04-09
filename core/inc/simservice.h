////////////////////////////////////////////////////////////////////////////////////
// NAME:  simservice.h
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
#ifndef SIMSERVICE_H
#define SIMSERVICE_H

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include "core/inc/signalmodel.h"
#include "core/inc/sinks/socketsink.h"

class SimService : public ISimService {
 public:
  SimService();
  ~SimService();

  bool LoadQML();

  virtual bool InitializeConnection(const char *ip, const unsigned int port);
  virtual int SendVehicleData(std::vector<SignalData> signalList);

 private:
  bool populateData();

 private:
  QQmlApplicationEngine m_engine;
  IDataSink *m_pdataSink;
  SignalModel m_signalModel;
  /*Old kvehicle Simulator vehicle signal ID mapping*/
  std::map<QString, int> m_dataIdMap;
};
#endif  // SIMSERVICE_H
