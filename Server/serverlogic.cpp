#include "serverlogic.h"
#include <QDebug>
#include <QVector>
#include <QMap>

ServerLogic::ServerLogic(QObject *parent) :
    QObject(parent)
{
    clients = new QMap<QString, QTcpSocket*>();
    messages = new QMap<QString, QVector<QVector<QString>>*>();
}

void ServerLogic::slotClientRequest(QTcpSocket *socket, QMap<QString, QString> map)
{
    qDebug() << "-------------------------------";
    QVector<QString> keys = map.keys().toVector();
    QString key;
    foreach (key, keys) {
        qDebug() << key + " : " + map[key];
    }


    QString type = map["type"];
    if (type == "new_name") {
        newNameProcessor(socket, map);
    } else if (type == "check_messages" ) {
        checkMessagesProcessor(socket, map);
    } else if (type == "send_private") {
        sendPrivateProcessor(socket, map);
    } else if (type == "send_to_all") {
        sendToAllProcessor(socket, map);
    } else if (type == "process_name") {
        processNameProcessor(socket, map);
    }
}

void ServerLogic::slotNewConnection(QTcpSocket *socket)
{
    QMap<QString, QString> map;
    map.insert("type", "give_name");
    emit sendToClient(socket, map);
}

void ServerLogic::slotDisconnectedClient(QTcpSocket *socket)
{
    QString key;
    QVector<QString> keys = clients->keys().toVector();
    key = keys[0];
    for (int i = 1; (i < keys.size()) && ((*clients)[key] != socket); i++) {
        key = keys[i];
    }
    (*clients)[key] = NULL;
    qDebug() << "Slot disconnected for client : " + key;


    QMap<QString, QString> map;
    map.insert("type", "notification_disconnect");
    map.insert("login", key);
    QString key_;
    foreach(key_, keys) {
        if ((*clients)[key_] != NULL) {
            emit sendToClient((*clients)[key_], map);
        }
    }
}


//-------------------------PROCESSORS------------------

void ServerLogic::newNameProcessor(QTcpSocket *socket, QMap<QString, QString> map)
{
    clients->insert(map["login"], socket);
    messages->insert(map["login"], new QVector<QVector<QString>>());


    QVector<QString> keys = clients->keys().toVector();
    QMap<QString, QString> map1;
    map1.insert("login", map["login"]);
    map1.insert("type", "notification_connect");
    QString key_;
    foreach(key_, keys) {
        if ((*clients)[key_] != NULL) {
            emit sendToClient((*clients)[key_], map1);
        }
    }
}

void ServerLogic::checkMessagesProcessor(QTcpSocket *socket, QMap<QString, QString> map)
{
    (*clients)[map["login"]] = socket;

    QVector<QString> keys = clients->keys().toVector();
    QMap<QString, QString> map2;
    map2.insert("login", map["login"]);
    map2.insert("type", "notification_connect");
    QString key_;
    foreach(key_, keys) {
        if ((*clients)[key_] != NULL) {
            emit sendToClient((*clients)[key_], map2);
        }
    }

    QVector<QVector<QString>> * messagesVec = (*messages)[map["login"]];
    for (int i = 0; i < messagesVec->size(); i++) {
        QMap<QString, QString> map1;
        map1.insert("type","receive_private");
        map1.insert("sender", messagesVec->at(i).at(0));
        map1.insert("text", messagesVec->at(i).at(1));
        sendToClient(socket, map1);
    }
}

void ServerLogic::sendPrivateProcessor(QTcpSocket *socket, QMap<QString, QString> map)
{
    QString receiver = map["receiver"];
    QList<QString> keys = messages->keys();
    if (!keys.contains(receiver)) {
        return;
    }
    if ((*clients)[receiver] != NULL) {
        QMap<QString, QString> map1;
        map1.insert("type", "receive_private");
        map1.insert("sender", map["login"]);
        map1.insert("text", map["text"]);
        emit sendToClient((*clients)[receiver], map1);
        emit sendToClient((*clients)[map["login"]], map1);
    } else {
        QVector<QString> vec;
        vec.push_back(map["login"]);
        vec.push_back(map["text"]);
        (*messages)[map["receiver"]]->push_back(vec);
    }
}

void ServerLogic::sendToAllProcessor(QTcpSocket *socket, QMap<QString, QString> map)
{
    QVector<QString> keys = clients->keys().toVector();
    QString key;
    foreach (key, keys) {
        if ((*clients)[key] != NULL) {
            QMap<QString, QString> map1;
            map1.insert("type", "receive_to_all");
            map1.insert("sender", map["login"]);
            map1.insert("text", map["text"]);
            emit sendToClient((*clients)[key], map1);
        } else {
//            QVector<QString> vec;
//            vec.push_back(map["login"]);
//            vec.push_back(map["text"]);
//            (*messages)[key]->push_back(vec);
        }
    }
}

void ServerLogic::processNameProcessor(QTcpSocket *socket, QMap<QString, QString> map)
{
    QString name = map["name"];
    QString message;
    QString type;
    QList<QString> names = clients->keys();
    if (names.contains(name)) {
        name = generateSimilarName(name);
        message = "Sorry, client, your new name is " + name + ", do you want register?";
    } else {
        message = "Welcome client, " + name + ", do you want register?";
    }

    type = "verification_name";
    QMap<QString, QString> map_answer;
    map_answer.insert("name", name);
    map_answer.insert("type", type);
    map_answer.insert("message", message);

    emit sendToClient(socket, map_answer);
}


QString ServerLogic::generateSimilarName(QString current)
{
    QList<QString> names = clients->keys();
    while (names.contains(current)) {
        current = current + "_";
    }
    return current;
}
