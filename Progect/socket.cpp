#include "socket.h"

Socket::Socket(QObject *parent) : QTcpSocket(parent) {
    connectToHost("127.0.0.1", 1234);
}

QSharedPointer<QTcpSocket> Socket::getSocket() const {
    // Возвращать sharedPointer на сам объект (если нужно)
    // Но лучше переписать архитектуру, чтобы не путать указатели.
    // Например, можно возвращать `this` в sharedPointer:
    return QSharedPointer<QTcpSocket>(const_cast<Socket*>(this));
    // Либо возвращать `this` как сырой указатель, чтобы не создавать лишние sharedPointer.
}
