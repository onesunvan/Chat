#ifndef SERVER_H
#define SERVER_H

#include <QObject>


#include <QMap>
class QTcpSocket;
class QTcpServer;
class QTextEdit;


class Server : public QObject
{
    Q_OBJECT
private:
    QTcpServer * tcpServer;
    quint16 nextBlockSize;

public slots:
    void slotSendToClient(QTcpSocket * socket, const QMap<QString, QString> &map);
    void slotCloseConnection(QTcpSocket * socket);

public:
    explicit Server(int port, QObject *parent = 0);



signals:
    void clientRequest(QTcpSocket* socket, QMap<QString, QString> map);
    void newConnection(QTcpSocket * socket);
    void disconnectedSocket(QTcpSocket * socket);

private slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotDisconnected();
};

#endif // SERVER_H
