#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include <QWidget>
#include <QMap>


class QTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;

class ClientLogic : public QWidget
{
    Q_OBJECT
private:
    QTextEdit * txtInfo;
    QPushButton * connectButton;
    QPushButton * disconnectButton;
    QPushButton * sendButton;
    QPushButton * sendToAllButton;

    QLabel * loginLabel;
    QString * login = NULL;
    QLineEdit * txtInput;



public:
    explicit ClientLogic(QWidget *parent = 0);

signals:
    void sendToServer(QMap<QString, QString> map);
    void startConnect();
    void closeConnection();

public slots:
    void slotServerAnswer(QMap<QString, QString> map);
    void slotSocketError(QString error);
    void slotConnected();

private slots:
    void slotConnectClicked();
    void slotDisconnectClicked();
    void slotSendClicked();
    void slotSendToAllClicked();


private:
    void giveNameProcessor(QMap<QString, QString> map);
    void notificationDisconnectProcessor(QMap<QString, QString> map);
    void notificationConnectProcessor(QMap<QString, QString> map);
    void receivePrivateProcessor(QMap<QString, QString> map);
    void receiveToAllProcessor(QMap<QString, QString> map);
    void verificationNameProccesor(QMap<QString, QString> map);
};

#endif // CLIENTLOGIC_H
