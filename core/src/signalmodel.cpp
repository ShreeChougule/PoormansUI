#include "core/inc/signalmodel.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

SignalModel::SignalModel(QObject *parent)
    : QAbstractListModel(parent), m_psimService(nullptr) {
  //initialize();
}

void SignalModel::SetCb(ISimService *pSimService) {
  m_psimService = pSimService;
}

void SignalModel::initialize(const QList<SignalData>& fetchedData){
    beginResetModel();
    allSignals.clear();
    filteredKeys.clear();

    for (const auto &signal : fetchedData) {
      allSignals.insert(signal.name, signal);
    }

    // Initially, show all signals
    filteredKeys = allSignals.keys();

    endResetModel();
}


// ✅ Define roles
QHash<int, QByteArray> SignalModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[Qt::UserRole + 1] = "id";
  roles[Qt::UserRole + 2] = "name";
  roles[Qt::UserRole + 3] = "mode";
  roles[Qt::UserRole + 4] = "period";
  roles[Qt::UserRole + 5] = "value";
  return roles;
}

void SignalModel::connectToServer(const QString &IpAddr){
m_psimService->InitializeConnection(IpAddr.toUtf8().constData(),PORT);
}

// ✅ Fast updates using `QMap`
void SignalModel::updateValue(const QString &name, const QString &newValue) {
  if (allSignals.contains(name)) {
    allSignals[name].value = newValue;
    int row = filteredKeys.indexOf(name);
    if (row != -1) {
      emit dataChanged(index(row, 0), index(row, 0), {Qt::UserRole + 5});
    }
    //  qDebug() << "Updated value for:" << name << "Period Mode :"<<
    //  allSignals[name].mode << "Period(ms) : "
    //           << allSignals[name].period << "New Value:" << newValue;
  }
}

void SignalModel::updatePeriod(const QString &name, const QString &newPeriod) {
  if (allSignals.contains(name)) {
    allSignals[name].period = newPeriod;
    int row = filteredKeys.indexOf(name);
    if (row != -1) {
      emit dataChanged(index(row, 0), index(row, 0), {Qt::UserRole + 4});
    }
    //  qDebug() << "Updated Row for:" << name << "New Period:" << newPeriod;
  }
}

void SignalModel::updateMode(const QString &name, const QString &newMode) {
  if (allSignals.contains(name)) {
    allSignals[name].mode = newMode;
    int row = filteredKeys.indexOf(name);
    if (row != -1) {
      emit dataChanged(index(row, 0), index(row, 0), {Qt::UserRole + 3});
    }
    // qDebug() << "Updated mode for:" << name << "New Mode:" << newMode;
  }
}

// ✅ Filtering function
void SignalModel::filterData(const QString &searchText) {
  beginResetModel();
  filteredKeys.clear();

  if (searchText.isEmpty()) {
    filteredKeys = allSignals.keys();  // Show all if no filter
  } else {
    for (const QString &key : allSignals.keys()) {
      if (key.contains(searchText, Qt::CaseInsensitive)) {
        filteredKeys.append(key);
      }
    }
  }

  endResetModel();
  //qDebug() << "Filtered signals count:" << filteredKeys.size();
}

// ✅ Correct implementation of rowCount()
int SignalModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);            // Avoids unused parameter warning
  return filteredKeys.size();  // ✅ Use filtered keys, not allSignals
}

QVariant SignalModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= filteredKeys.size()) {
    return QVariant();
  }

  const QString &signalKey =
      filteredKeys.at(index.row());  // ✅ Correct indexing
  const SignalData &signal = allSignals.value(signalKey);
  //  qDebug() << "Fetching data for row" << index.row() << "Role:" << role
  //           << "Name:" << signal.name << "Mode:" << signal.mode;

  switch (role) {
    case Qt::UserRole + 1:
      return signal.id;
    case Qt::UserRole + 2:
      return signal.name;
    case Qt::UserRole + 3:
      return signal.mode;
    case Qt::UserRole + 4:
      return signal.period;
    case Qt::UserRole + 5:
      return signal.value;
    default:
      return QVariant();
  }
}

// Handle the data sent from QML (Name + Value)
void SignalModel::sendData(const QStringList &data) {
  qDebug() << "Recieved data from qml, data size -  " << data.size();

  std::vector<SignalData> signalsToSend;
  for (const QString &entry : data) {
    if (allSignals.contains(entry)) {
      qDebug() << "Sending data: Name- " << allSignals[entry].name << ", Mode- "
               << allSignals[entry].mode << ", Period- "
               << allSignals[entry].period << ", Value- "
               << allSignals[entry].value;
      signalsToSend.emplace_back(allSignals[entry]);
    }
  }
}
