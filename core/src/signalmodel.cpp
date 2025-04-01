#include "core/inc/signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QSqlQueryModel(parent) {
    filterData(""); // to load every data initially
}

// Filter data based on the search term
void SignalModel::filterData(const QString &searchTerm) {
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

// Handle the data sent from QML (Name + Value)
void SignalModel::sendData(const QStringList &data) {
    qDebug() << "Recieved data from qml, data size -  "<<data.size();
    for (const QString &entry : data) {
        qDebug() << "Sending data:" << entry;  // You can replace this with actual sending logic
    }
}

QString SignalModel::getDataAt(int index, int role) {
    if (index < 0 || index >= rowCount()) return "";

    QModelIndex modelIndex = this->index(index, 0);  // Get model index
    return data(modelIndex, role).toString();
}

int SignalModel::rowCount() {
    return QSqlQueryModel::rowCount();  // Get total rows
}

void SignalModel::connectToServer(const QString &inputIP){
    m_socket.connectToServer(inputIP.toStdString().c_str(),PORT);
}

void SignalModel::disconnectFromServer(){
    m_socket.closeSocket();
}

