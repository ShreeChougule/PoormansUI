#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include <QSqlQueryModel>
#include <QObject>

class SignalModel : public QSqlQueryModel {
    Q_OBJECT  // Important: Enables Qt's meta-object system
public:
    explicit SignalModel(QObject *parent = nullptr);
    virtual ~SignalModel(){}

    void refresh();  // Method to reload data

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // SIGNALMODEL_H

