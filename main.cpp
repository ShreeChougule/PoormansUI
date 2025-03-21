#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSql/QSqlDatabase>
#include <QQmlContext>
#include <QDebug>
#include "signalmodel.h"


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set up database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/kpit/Desktop/PoormansUI/common/vehicle_twin.db");

    if (!db.open()) {
        return -1;
    }

    // Create and expose the SignalModel and SignalSender to QML
    SignalModel signalModel;

    engine.rootContext()->setContextProperty("signalModel", &signalModel);

    // Load QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
