#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include <QSqlQueryModel>
#include <QObject>

class SignalModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit SignalModel(QObject *parent = nullptr);
    virtual ~SignalModel(){}

    Q_INVOKABLE void filterData(const QString &searchTerm);  // Make filterData invokable


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // SIGNALMODEL_H
