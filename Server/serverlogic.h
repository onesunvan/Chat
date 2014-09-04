#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include <QObject>
#include <QSet>
#include <QMap>

class QTcpSocket;

class ServerLogic : public QObject
{
    Q_OBJECT

private:
    QMap<QString, QTcpSocket*> * clients;
    QMap<QString, QVector<QVector<QString>>*> * messages;
public:
    explicit ServerLogic(QObject *parent = 0);

signals:
    void sendToClient(QTcpSocket * socket, const QMap<QString, QString> &map);
    void closeConnection(QTcpSocket * socket);
public slots:
    void slotClientRequest(QTcpSocket* socket, QMap<QString, QString> map);
    void slotNewConnection(QTcpSocket * socket);
    void slotDisconnectedClient(QTcpSocket * socket);
private:
    void newNameProcessor(QTcpSocket* socket, QMap<QString, QString> map);
    void checkMessagesProcessor(QTcpSocket* socket, QMap<QString, QString> map);
    void sendPrivateProcessor(QTcpSocket* socket, QMap<QString, QString> map);
    void sendToAllProcessor(QTcpSocket* socket, QMap<QString, QString> map);
    void processNameProcessor(QTcpSocket* socket, QMap<QString, QString> map);

    QString generateSimilarName(QString current);
};

#endif // SERVERLOGIC_H
