#include "signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QSqlQueryModel(parent) {
    refresh();  // Load initial data
}

void SignalModel::refresh() {
    setQuery(R"(
        SELECT Signal.name, Signal_Attributes.default_value
        FROM Signal
        LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
    )");

    if (lastError().isValid()) {
        qDebug() << "SQL Error:" << lastError().text();
    }
}

// Override data retrieval
QVariant SignalModel::data(const QModelIndex &index, int role) const {
    if (role < Qt::UserRole) {
        return QSqlQueryModel::data(index, role);
    }

    int columnIdx = role - Qt::UserRole;
    return QSqlQueryModel::data(this->index(index.row(), columnIdx), Qt::DisplayRole);
}

// Define custom roles
QHash<int, QByteArray> SignalModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "name";
    roles[Qt::UserRole + 1] = "value";
    return roles;
}

