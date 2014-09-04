#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

void Server::slotSendToClient(QTcpSocket *socket, const QMap<QString, QString> &map)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << map;

    out.device()->seek(0);

    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void Server::slotCloseConnection(QTcpSocket *socket)
{
    socket->close();
}

Server::Server(int port, QObject *parent) :
    QObject(parent), nextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to server: " +
                              tcpServer->errorString());
        return;
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

}

void Server::slotNewConnection()
{
    QTcpSocket * socket = tcpServer->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    emit newConnection(socket);
}

void Server::slotReadClient()
{
    QTcpSocket * socket = (QTcpSocket *) sender();

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_5);
    for(;;) {
        if (!nextBlockSize) {
            if (socket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize) {
            break;
        }


        QMap<QString, QString> map;
        in >> map;

        emit clientRequest(socket, map);

        nextBlockSize = 0;
    }
}

void Server::slotDisconnected()
{
    QTcpSocket * socket = (QTcpSocket *) sender();

    emit disconnectedSocket(socket);
}
