#ifndef SIGNALMODEL_H
#define SIGNALMODEL_H

#include "core/api/ISimService.h"
#include "core/api/SimTypes.h"

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
  void SetCb(ISimService *pSimService);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void connectToServer(const QString &IpAddr);
  Q_INVOKABLE void updateValue(const QString &name, const QString &newValue);
  Q_INVOKABLE void updateMode(const QString &name, const QString &newMode);
  Q_INVOKABLE void filterData(const QString &searchText);
  Q_INVOKABLE void updatePeriod(const QString &name, const QString &newPeriod);
  Q_INVOKABLE void sendData(
      const QStringList &data);  // Add new method to handle the data

  void initialize(const QList<SignalData>& fetchedData); // Load data from list

 private:
  QMap<QString, SignalData> allSignals;  // Stores all signals
  QList<QString> filteredKeys;  // Holds the filtered list for UI display
  ISimService *m_psimService;
};

#endif  // SIGNALMODEL_H
