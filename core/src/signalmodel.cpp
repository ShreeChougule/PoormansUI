#include "core/inc/signalmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SignalModel::SignalModel(QObject *parent) : QAbstractListModel(parent) {
    initialize();
}

// ✅ Load data from DB into local structure
void SignalModel::initialize() {
    beginResetModel();
    allSignals.clear();
    filteredKeys.clear();

    QSqlQuery query(R"(
        SELECT Signal.name, Signal_Attributes.default_value
        FROM Signal
        LEFT JOIN Signal_Attributes ON Signal.signal_id = Signal_Attributes.signal_id
    )");

    QList<SignalData> fetchedData;
    int signalIndex = 0;

    while (query.next()) {
        SignalData entry;
        entry.index = signalIndex++;
        entry.name = query.value(0).toString();
        entry.mode = "Auto";
        entry.period = "";
        entry.value = query.value(1).toString();
        fetchedData.append(entry);
    }

    if (query.lastError().isValid()) {
        qDebug() << "SQL Error:" << query.lastError().text();
    }

    for (const auto &signal : fetchedData) {
        allSignals.insert(signal.name, signal);
    }

    // Initially, show all signals
    filteredKeys = allSignals.keys();

    endResetModel();
}

// ✅ Define roles
QHash<int, QByteArray> SignalModel::roleNames() const {
    return {
        { Qt::UserRole, "index" },
        { NameRole, "name" },
        { ModeRole, "mode" },
        { PeriodRole, "period" },
        { ValueRole, "value" }
    };
}

// ✅ Fast updates using `QMap`
void SignalModel::updateValue(const QString &name, const QString &newValue) {
    if (allSignals.contains(name)) {
        allSignals[name].value = newValue;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { ValueRole });
        }
        qDebug() << "Updated value for:" << name << "Period Mode :"<< allSignals[name].mode << "Period(ms) : "
                 << allSignals[name].period << "New Value:" << newValue;
    }
}

void SignalModel::updatePeriod(const QString &name, const QString &newPeriod){
    if (allSignals.contains(name)) {
        allSignals[name].period = newPeriod;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { PeriodRole });
        }
        qDebug() << "Updated Row for:" << name << "New Period:" << newPeriod;

    }
}

void SignalModel::updateMode(const QString &name, const QString &newMode) {
    if (allSignals.contains(name)) {
        allSignals[name].mode = newMode;
        int row = filteredKeys.indexOf(name);
        if (row != -1) {
            emit dataChanged(index(row, 0), index(row, 0), { ModeRole });
        }
        qDebug() << "Updated mode for:" << name << "New Mode:" << newMode;
    }
}

// ✅ Filtering function
void SignalModel::filterData(const QString &searchText) {
    beginResetModel();
    filteredKeys.clear();

    if (searchText.isEmpty()) {
        filteredKeys = allSignals.keys();  // Show all if no filter
    } else {
        for (const QString &key : allSignals.keys()) {
            if (key.contains(searchText, Qt::CaseInsensitive)) {
                filteredKeys.append(key);
            }
        }
    }

    endResetModel();
    qDebug() << "Filtered signals count:" << filteredKeys.size();
}

// ✅ Correct implementation of rowCount()
int SignalModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);  // Avoids unused parameter warning
    return filteredKeys.size();  // ✅ Use filtered keys, not allSignals
}

QVariant SignalModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= filteredKeys.size()) {
        return QVariant();
    }

    const QString &signalKey = filteredKeys.at(index.row());  // ✅ Correct indexing
    const SignalData &signal = allSignals.value(signalKey);

    switch (role) {
        case Qt::UserRole: return signal.index;
        case NameRole: return signal.name;
        case ModeRole: return signal.mode;
        case PeriodRole: return signal.period;
        case ValueRole: return signal.value;
        default: return QVariant();
    }
}
