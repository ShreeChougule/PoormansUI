#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include "SocketClient.h"

struct SignalEntry {
    QString name;
    QString mode;
    int period;
    QString value;
};

class SignalModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit SignalModel(QObject *parent = nullptr);
    virtual ~SignalModel() {}
    void initialize();

    Q_INVOKABLE void filterData(const QString &searchTerm);
    Q_INVOKABLE void updateData(const QString &name, const QString &mode, int period, const QString &value);
    Q_INVOKABLE void sendData();
    Q_INVOKABLE void connectToServer(const QString &inputIP);
    Q_INVOKABLE void disconnectFromServer();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<SignalEntry> allSignals;
    QVector<SignalEntry> filteredSignals;
    SocketClient m_socket;
};

#endif // SIGNALMODEL_H
