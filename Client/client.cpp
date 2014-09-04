#include "client.h"

#include <QDebug>

Client::Client(const QString &host, int port, QObject *parent) :
    QObject(parent), nextBlockSize(0)
{      
    this->host = host;
    this->port = port;
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

void Client::slotReadyRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_5);
    for (;;) {
        if (!nextBlockSize) {
            if (tcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }
        if (tcpSocket->bytesAvailable() < nextBlockSize) {
            break;
        }
        QMap<QString, QString> map;
        in >> map;

        emit serverAnswer(map);

        nextBlockSize = 0;
    }
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString error;
    switch (err) {
    case QAbstractSocket::HostNotFoundError:
        error = "The host was not found.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        error = "The remote host is closed.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        error = "Connection was refused.";
        break;
    default:
        error = QString(tcpSocket->errorString());
    }
    emit socketError(error);
}

void Client::slotSendToServer(QMap<QString, QString> map)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0) << map;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);

}

void Client::slotCloseConnection()
{
    tcpSocket->close();
}

void Client::slotStartConnect()
{
    tcpSocket->connectToHost(host, port);
}

void Client::slotConnected()
{
    emit connected();
}
