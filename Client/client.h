#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

#include <QMap>
class Client : public QObject
{
    Q_OBJECT

private:
    QTcpSocket * tcpSocket;
    quint16 nextBlockSize;

    QString host;
    int port;

public:
    explicit Client(const QString & host, int port, QObject *parent = 0);


signals:
    void serverAnswer(QMap<QString, QString> map);
    void socketError(QString error);
    void connected();

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);
    void slotConnected();

public slots:
    void slotSendToServer(QMap<QString, QString> map);
    void slotCloseConnection();
    void slotStartConnect();

};

#endif // CLIENT_H
