#include "core/inc/signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QAbstractListModel(parent) {
    initialize();
}

// ✅ Load data from DB into local structure
void SignalModel::initialize() {
    beginResetModel();
    allSignals.clear();
    filteredKeys.clear();

    QSqlQuery query(R"(
        SELECT Signal.name, Signal_Attributes.default_value
        FROM Signal
        LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
    )");

    QList<SignalData> fetchedData;
    int signalIndex = 0;

    while (query.next()) {
        SignalData entry;
        entry.index = signalIndex++;
        entry.name = query.value(0).toString();
        entry.mode = "Auto";
        entry.period = "";
        entry.value = query.value(1).toString();
        fetchedData.append(entry);
    }

    if (query.lastError().isValid()) {
        qDebug() << "SQL Error:" << query.lastError().text();
    }

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
    roles[Qt::UserRole + 1] = "index";
    roles[Qt::UserRole + 2] = "name";
    roles[Qt::UserRole + 3] = "mode";
    roles[Qt::UserRole + 4] = "period";
    roles[Qt::UserRole + 5] = "value";
    return roles;
}

// ✅ Fast updates using `QMap`
void SignalModel::updateValue(const QString &name, const QString &newValue) {
    if (allSignals.contains(name)) {
        allSignals[name].value = newValue;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { Qt::UserRole + 5 });
        }
      //  qDebug() << "Updated value for:" << name << "Period Mode :"<< allSignals[name].mode << "Period(ms) : "
      //           << allSignals[name].period << "New Value:" << newValue;
    }
}

void SignalModel::updatePeriod(const QString &name, const QString &newPeriod){
    if (allSignals.contains(name)) {
        allSignals[name].period = newPeriod;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { Qt::UserRole + 4 });
        }
      //  qDebug() << "Updated Row for:" << name << "New Period:" << newPeriod;

    }
}

void SignalModel::updateMode(const QString &name, const QString &newMode) {
    if (allSignals.contains(name)) {
        allSignals[name].mode = newMode;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { Qt::UserRole + 3 });
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
//    qDebug() << "Filtered signals count:" << filteredKeys.size();
}

// ✅ Correct implementation of rowCount()
int SignalModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);  // Avoids unused parameter warning
    return filteredKeys.size();  // ✅ Use filtered keys, not allSignals
}

QVariant SignalModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= filteredKeys.size()) {
        return QVariant();
    }


    const QString &signalKey = filteredKeys.at(index.row());  // ✅ Correct indexing
    const SignalData &signal = allSignals.value(signalKey);
  //  qDebug() << "Fetching data for row" << index.row() << "Role:" << role
  //           << "Name:" << signal.name << "Mode:" << signal.mode;

    switch (role) {
        case Qt::UserRole + 1: return signal.index;
        case Qt::UserRole + 2: return signal.name;
        case Qt::UserRole + 3: return signal.mode;
        case Qt::UserRole + 4: return signal.period;
        case Qt::UserRole + 5: return signal.value;
        default: return QVariant();
    }
}

// Handle the data sent from QML (Name + Value)
void SignalModel::sendData(const QStringList &data) {
    qDebug() << "Recieved data from qml, data size -  "<<data.size();
    for (const QString &entry : data) {
        if (allSignals.contains(entry)) {
        qDebug() << "Sending data: Name- " <<  allSignals[entry].name << ", Mode- " << allSignals[entry].mode
                 << ", Period- "<<allSignals[entry].period << ", Value- "<<allSignals[entry].value;
        }

    }
}
