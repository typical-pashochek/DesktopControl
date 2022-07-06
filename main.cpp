#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    qRegisterMetaType<DataBlock>("DataBlock");
    qRegisterMetaType<QAbstractSocket::SocketState>("SocketState");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<RemoteEvent>("RemoteEvent");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
