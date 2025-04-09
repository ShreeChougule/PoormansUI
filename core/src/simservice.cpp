#include <QSqlError>


#include "core/inc/simservice.h"


SimService::SimService() : m_pdataSink(new SocketSink()) {
  m_signalModel.SetCb(this);
}

SimService::~SimService() {}

bool SimService::LoadQML() {
  bool status = true;

    // Set up database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/kpit/Desktop/PoormansUI/common/vehicle_twin.db");


    if (!db.open()) {
            qDebug() << "Failed to open DB:" << db.lastError().text();
      status = false;
    }

  m_engine.rootContext()->setContextProperty("signalModel", &m_signalModel);

  // Load QML file
  m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  if (m_engine.rootObjects().isEmpty()) status = false;
    populateData();
  return status;
}

bool SimService::populateData(){
    bool status = true;

    QSqlQuery query(R"(
          SELECT Signal.signal_id, Signal.name, Signal_Attributes.default_value
          FROM Signal
          LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
      )");

    QList<SignalData> fetchedData;

    while (query.next()) {
      SignalData entry;
      entry.id = query.value(0).toInt();
      entry.name = query.value(1).toString();
      entry.mode = "Auto";
      entry.period = "";
      entry.value = query.value(2).toString();
      fetchedData.append(entry);
    }

    m_signalModel.initialize(fetchedData);

    return status;
}

bool SimService::InitializeConnection(const char *ip, const unsigned int port) {
    qDebug() << "Connect to -  " << ip <<":"<<port;
    m_pdataSink->Connect(ip,port);
}

int SimService::SendVehicleData(std::vector<SignalData> signalList) {

}
