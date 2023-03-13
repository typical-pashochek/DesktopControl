#include "networkapi.h"

#include <QNetworkInterface>

NetworkApi::NetworkApi(QObject *parent)
    : QObject (parent)
{

}

bool NetworkApi::isLocalAddress(const QHostAddress &address)
{
#ifndef Q_OS_ANDROID
    auto interfaces = QNetworkInterface::allInterfaces();
    QList<QNetworkAddressEntry> entry;
    for (const auto& interface : interfaces) {
        if (interface.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
            entry = interface.addressEntries();
            QHostAddress ip = entry.at(0).ip();
            if (ip.toIPv4Address() == address.toIPv4Address())
                return true;
            entry.clear();
        }
    }
#endif
    return false;
}

QString NetworkApi::getLocalIpAddress()
{
    QString localIp = "0.0.0.0";
#ifdef Q_OS_WIN
    auto interfaces = QNetworkInterface::allInterfaces();
    QList<QNetworkAddressEntry> entries;
    for (const auto& interface : interfaces) {
        if (interface.flags() & QNetworkInterface::IsLoopBack)
            continue;

        if (interface.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
            entries = interface.addressEntries();
            for (const auto& entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    if (interface.name().indexOf("wireless") != -1) {
                        localIp = entry.ip().toString();
                        //qDebug() << interface.humanReadableName() << interface.name() << " WiFi IP: " << localIp;
                    } else if (interface.name().indexOf("ethernet") != -1) {
                        //qDebug() << interface.humanReadableName() << interface.name() << " Ethernet IP: " << entry.ip().toString();
                    }
                }
            }
            entries.clear();
        }
    }
#elif defined(Q_OS_LINUX) && not defined(Q_OS_ANDROID)
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET && ifa->ifa_flags & (IFF_UP | IFF_RUNNING) && !(ifa->ifa_flags & IFF_LOOPBACK)) {
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            localIp = addressBuffer;
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
#endif
    return localIp;
}
