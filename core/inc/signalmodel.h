#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include <QAbstractListModel>
#include <QMap>

struct SignalData {
    int index;
    QString name;
    QString mode;
    QString period;
    QString value;
};

class SignalModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit SignalModel(QObject *parent = nullptr);

    enum SignalRoles {
        NameRole = Qt::UserRole + 1,
        ModeRole,
        PeriodRole,
        ValueRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updateValue(const QString &name, const QString &newValue);
    Q_INVOKABLE void updateMode(const QString &name, const QString &newMode);
    Q_INVOKABLE void filterData(const QString &searchText);
    Q_INVOKABLE void updatePeriod(const QString &name, const QString &newPeriod);

    void initialize();  // Load data from DB

private:
    QMap<QString, SignalData> allSignals;  // Stores all signals
    QList<QString> filteredKeys;  // Holds the filtered list for UI display
};

#endif // SIGNALMODEL_H
