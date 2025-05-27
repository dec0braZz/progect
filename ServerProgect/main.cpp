#include <QCoreApplication>
#include "MyServer.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    MyServer server;
    if (!server.listen(QHostAddress::Any, 1234)) {
        qDebug() << "Unable to start the server:";
        return 1;
    }

    qDebug() << "Server started on port" << server.serverPort();
    return a.exec();
}
