#include "clientlogic.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QVector>
#include <QMap>
#include "inputnamedialog.h"
#include "selectdialog.h"

ClientLogic::ClientLogic(QWidget *parent) :
    QWidget(parent)
{
    txtInfo = new QTextEdit();

    txtInfo->setReadOnly(true);

    txtInput = new QLineEdit();
    connect(txtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToAllClicked()));
    txtInput->setEnabled(false);

    connectButton = new QPushButton("&Connect");
    connect(connectButton, SIGNAL(clicked()), this, SLOT(slotConnectClicked()));

    disconnectButton = new QPushButton("&Disconnect");
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(slotDisconnectClicked()));
    disconnectButton->setEnabled(false);

    sendButton = new QPushButton("&Send...");
    connect(sendButton, SIGNAL(clicked()), this, SLOT(slotSendClicked()));
    sendButton->setEnabled(false);


    sendToAllButton = new QPushButton("&Send to all");
    connect(sendToAllButton, SIGNAL(clicked()), this, SLOT(slotSendToAllClicked()));
    sendToAllButton->setEnabled(false);



    loginLabel = new QLabel("");
    loginLabel->setVisible(false);

//Compose GUI
    QVBoxLayout * vbxLayout = new QVBoxLayout();
    vbxLayout->addWidget(new QLabel("<H2>Chat</H2>"));
    vbxLayout->addWidget(connectButton);
    vbxLayout->addWidget(disconnectButton);
    vbxLayout->addWidget(loginLabel);
    vbxLayout->addWidget(txtInput);

    QHBoxLayout * hbxLayout = new QHBoxLayout();
    hbxLayout->addWidget(sendButton);
    hbxLayout->addWidget(sendToAllButton);

    vbxLayout->addLayout(hbxLayout);

    vbxLayout->addWidget(txtInfo);
    setLayout(vbxLayout);
}

void ClientLogic::slotServerAnswer(QMap<QString, QString> map)
{
    qDebug() << "-------------------------------";
    QVector<QString> keys = map.keys().toVector();
    QString key;
    foreach (key, keys) {
        qDebug() << key + " : " + map[key];
    }


    if (map["type"] == "give_name") {
        giveNameProcessor(map);
    } else if (map["type"] == "notification_disconnect") {
        notificationDisconnectProcessor(map);
    } else if (map["type"] == "notification_connect") {
        notificationConnectProcessor(map);
    } else if (map["type"] == "receive_private") {
        receivePrivateProcessor(map);
    } else if (map["type"] == "receive_to_all") {
        receiveToAllProcessor(map);
    } else if (map["type"] == "verification_name") {
        verificationNameProccesor(map);
    }

}

void ClientLogic::slotSocketError(QString error)
{
    txtInfo->append("<I>" + error + "</I>");
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    sendToAllButton->setEnabled(false);
    sendButton->setEnabled(false);
    txtInput->setEnabled(false);
}

void ClientLogic::slotConnected()
{
}

void ClientLogic::slotConnectClicked()
{
    emit startConnect();
}

void ClientLogic::slotDisconnectClicked()
{
    emit closeConnection();
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    sendToAllButton->setEnabled(false);
    sendButton->setEnabled(false);
    txtInput->setEnabled(false);
    txtInfo->append("<I> You has gone offline </I>");
}

void ClientLogic::slotSendClicked()
{
    QMap<QString, QString> map;
    map.insert("type", "send_private");
    map.insert("text", txtInput->text());
    map.insert("login", *login);
    InputNameDialog * dialog = new InputNameDialog();
    if (dialog->exec() == QDialog::Accepted) {
        txtInput->clear();
        QString receiver = dialog->name();
        map.insert("receiver", receiver);
    } else {
        return;
    }
    sendToServer(map);
}

void ClientLogic::slotSendToAllClicked()
{
    QMap<QString, QString> map;
    map.insert("type", "send_to_all");
    map.insert("text", txtInput->text());
    map.insert("login", *login);
    txtInput->clear();
    sendToServer(map);
}


//-----------PROCESSORS---------------------------------------------------

void ClientLogic::giveNameProcessor(QMap<QString, QString> map)
{
    QString tmp;
    QMap<QString, QString> request_map;
    if (login == NULL) {
        InputNameDialog * dialog = new InputNameDialog();
        if (dialog->exec() == QDialog::Accepted) {
            tmp = dialog->name();
//            loginLabel->setText("<H3>" + *login + "</H3>");
//            loginLabel->setVisible(true);
            request_map.insert("name", tmp);
        } else {
            return;
        }
        request_map.insert("type", "process_name");
        qDebug() << "Hello, Ivan";
    } else {
        request_map.insert("type", "check_messages");
        request_map.insert("login", *login);
    }


    emit sendToServer(request_map);
    connectButton->setEnabled(false);
    disconnectButton->setEnabled(true);
    sendButton->setEnabled(true);
    sendToAllButton->setEnabled(true);
    txtInput->setEnabled(true);
}

void ClientLogic::notificationDisconnectProcessor(QMap<QString, QString> map)
{
    txtInfo->append("<I>" + map["login"] + " has gone offline </I>" );
}

void ClientLogic::notificationConnectProcessor(QMap<QString, QString> map)
{
    txtInfo->append("<I>" + map["login"] + " has appeared online </I>");
}

void ClientLogic::receivePrivateProcessor(QMap<QString, QString> map)
{
    txtInfo->append("<B>" + map["sender"] + " : " + map["text"] + "</B>");
}

void ClientLogic::receiveToAllProcessor(QMap<QString, QString> map)
{
    txtInfo->append(map["sender"] + " : " + map["text"]);
}

void ClientLogic::verificationNameProccesor(QMap<QString, QString> map)
{
    QString type;
    QString message;
    QString name;
    QMap<QString, QString> request_map;
    txtInfo->append(map["message"]);
    SelectDialog * dialog = new SelectDialog(map["message"]);
    if (dialog->exec() == QDialog::Accepted) {
        name = map["name"];
        message = "I am register client, " + name;
        type = "new_name";
        request_map.insert("login", name);
        request_map.insert("message", message);
        request_map.insert("type", type);
        login = new QString(name);
        loginLabel->setText("<H3>" + *login + "</H3>");
        loginLabel->setVisible(true);

        txtInfo->append("Yes");
        emit sendToServer(request_map);
    } else {
        txtInfo->append("No");
        InputNameDialog * nameDialog = new InputNameDialog();
        if (nameDialog->exec() == QDialog::Accepted) {
            request_map.insert("type", "process_name");
            name = nameDialog->name();
            request_map.insert("name", name);
            emit sendToServer(request_map);
        } else {
            emit closeConnection();
            connectButton->setEnabled(true);
            txtInfo->append("Connection has closed. Initiator is client");
        }
    }
}
