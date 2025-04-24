#ifndef SOCKET_H
#define SOCKET_H
#include <QTcpSocket>
#include <QSharedPointer>
#include <QWidget>
class Socket : public QTcpSocket {
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    QSharedPointer<QTcpSocket> getSocket() const;
private:
    QSharedPointer<QTcpSocket> socket;
};


#endif // SOCKET_H
