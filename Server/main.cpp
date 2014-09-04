#include "server.h"
#include "serverlogic.h"

#include <QCoreApplication>

#include <QDebug>

int main(int argc, char ** argv) {
    QCoreApplication app(argc, argv);
    Server * server = new Server(2323);

    ServerLogic * logic = new ServerLogic();

    QObject::connect(server, SIGNAL(clientRequest(QTcpSocket*,QMap<QString,QString>)),
                     logic, SLOT(slotClientRequest(QTcpSocket*,QMap<QString,QString>)));
    QObject::connect(logic, SIGNAL(sendToClient(QTcpSocket*,QMap<QString,QString>)),
                     server, SLOT(slotSendToClient(QTcpSocket*,QMap<QString,QString>)));
    QObject::connect(logic, SIGNAL(closeConnection(QTcpSocket*)),
                     server, SLOT(slotCloseConnection(QTcpSocket*)));
    QObject::connect(server, SIGNAL(newConnection(QTcpSocket*)),
                     logic, SLOT(slotNewConnection(QTcpSocket*)));
    QObject::connect(server, SIGNAL(disconnectedSocket(QTcpSocket*)),
                     logic, SLOT(slotDisconnectedClient(QTcpSocket*)));

    return app.exec();
}
