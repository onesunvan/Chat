#include <QApplication>
#include "client.h"
#include "clientlogic.h"

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    Client * client = new Client("192.168.1.126", 2323);
    ClientLogic * logic = new ClientLogic();

    QObject::connect(client, SIGNAL(serverAnswer(QMap<QString,QString>)),
                     logic, SLOT(slotServerAnswer(QMap<QString,QString>)));
    QObject::connect(logic, SIGNAL(sendToServer(QMap<QString,QString>)),
                     client, SLOT(slotSendToServer(QMap<QString,QString>)));
    QObject::connect(client, SIGNAL(socketError(QString)),
                     logic, SLOT(slotSocketError(QString)));
    QObject::connect(logic, SIGNAL(startConnect()),
                     client, SLOT(slotStartConnect()));
    QObject::connect(client, SIGNAL(connected()),
                     logic, SLOT(slotConnected()));
    QObject::connect(logic, SIGNAL(closeConnection()),
                     client, SLOT(slotCloseConnection()));
    logic->show();

    return app.exec();
}
