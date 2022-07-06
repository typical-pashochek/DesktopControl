#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#ifdef Q_OS_LINUX
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
#endif

#include <QObject>

class QHostAddress;
class NetworkApi : public QObject
{
    Q_OBJECT

public:
    NetworkApi(QObject *parent = nullptr);

    static bool isLocalAddress(const QHostAddress &address);
    static QString getLocalIpAddress();
};

#endif // NETWORKAPI_H
