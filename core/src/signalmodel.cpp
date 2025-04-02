#include "core/inc/signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QAbstractListModel(parent) {
    initialize();
}

// ✅ Load data from DB into local structure
void SignalModel::initialize() {
    allSignals.clear();

    QSqlQuery query(R"(
        SELECT Signal.name, Signal_Attributes.default_value
        FROM Signal
        LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
    )");

    while (query.next()) {
        SignalEntry entry;
        entry.name = query.value(0).toString();
        entry.mode = "Override";
        entry.period = 1000;
        entry.value = query.value(1).toString();
        allSignals.append(entry);
    }

    if (query.lastError().isValid()) {
        qDebug() << "SQL Error:" << query.lastError().text();
    }

    qDebug() << "Loaded" << allSignals.size() << "signals from DB.";
    filterData("");  // 🔹 Ensure initial data is loaded
}

// ✅ Filter from local structure instead of DB
void SignalModel::filterData(const QString &searchTerm) {
    beginResetModel();
    filteredSignals.clear();

    for (const auto &entry : allSignals) {
        if (entry.name.contains(searchTerm, Qt::CaseInsensitive)) {
            filteredSignals.append(entry);
        }
    }

    qDebug() << "Filtered signals count:" << filteredSignals.size(); // Debugging

    endResetModel();
}

int SignalModel::rowCount(const QModelIndex &) const {
    return filteredSignals.size();
}

QVariant SignalModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= filteredSignals.size()) return QVariant();

    const SignalEntry &entry = filteredSignals[index.row()];
    switch (role) {
        case Qt::UserRole: return entry.name;
        case Qt::UserRole + 1: return entry.mode;
        case Qt::UserRole + 2: return entry.period;
        case Qt::UserRole + 3: return entry.value;
    }
    return QVariant();
}

QHash<int, QByteArray> SignalModel::roleNames() const {
    return { {Qt::UserRole, "name"}, {Qt::UserRole + 1, "mode"}, {Qt::UserRole + 2, "period"}, {Qt::UserRole + 3, "value"} };
}

// ✅ Update local structure when user edits a field
void SignalModel::updateData(const QString &name, const QString &mode, int period, const QString &value) {
    for (auto &entry : allSignals) {
        if (entry.name == name) {
            entry.mode = mode;
            entry.period = period;
            entry.value = value;
            break;
        }
    }
}

// ✅ Send updated data
void SignalModel::sendData() {
    qDebug() << "Sending updated data:";
    for (const auto &entry : allSignals) {
        qDebug() << entry.name << entry.mode << entry.period << entry.value;
    }
}

// ✅ Socket Functions
void SignalModel::connectToServer(const QString &inputIP) {
    m_socket.connectToServer(inputIP.toStdString().c_str(), PORT);
}

void SignalModel::disconnectFromServer() {
    m_socket.closeSocket();
}
