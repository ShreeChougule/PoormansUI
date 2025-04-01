#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include <QSqlQueryModel>
#include <QObject>
#include "SocketClient.h"

class SignalModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit SignalModel(QObject *parent = nullptr);
    virtual ~SignalModel(){}

    Q_INVOKABLE void filterData(const QString &searchTerm);  // Make filterData invokable
    Q_INVOKABLE void sendData(const QStringList &data);  // Add new method to handle the data
    Q_INVOKABLE QString getDataAt(int index, int role);
    virtual Q_INVOKABLE int rowCount();  // Add this to get total rows
    Q_INVOKABLE void connectToServer(const QString &inputIP);  // Make connectToServer invokable
    Q_INVOKABLE void disconnectFromServer();  // Make disconnectFromServer invokable


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:

    SocketClient m_socket;
};

#endif // SIGNALMODEL_H
