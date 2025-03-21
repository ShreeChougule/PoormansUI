#include "signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QSqlQueryModel(parent) {
    filterData(""); // to load every data
}


// Filter data based on the search term
void SignalModel::filterData(const QString &searchTerm) {
    // Update query with a filter based on search term
    QString queryStr = QString(R"(
        SELECT Signal.name, Signal_Attributes.default_value
        FROM Signal
        LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
        WHERE Signal.name LIKE '%%1%'
    )").arg(searchTerm);

    setQuery(queryStr);

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
