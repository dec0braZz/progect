#include "socket.h"
#include <QJsonDocument>
Socket::Socket(QObject *parent) : QTcpSocket(parent) {
    connectToHost("127.0.0.1", 1234);
}
void Socket::sendJson(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    socket->write(data);
    socket->flush();
}
QSharedPointer<QTcpSocket> Socket::getSocket() const {
    // Возвращать sharedPointer на сам объект (если нужно)
    // Но лучше переписать архитектуру, чтобы не путать указатели.
    // Например, можно возвращать `this` в sharedPointer:
    return QSharedPointer<QTcpSocket>(const_cast<Socket*>(this));
    // Либо возвращать `this` как сырой указатель, чтобы не создавать лишние sharedPointer.
}
