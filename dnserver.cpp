#include "dnserver.h"

#include <QSettings>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/spdlog.h>

DNServer::DNServer(QObject* parent)
    : QObject{ parent }
{
    QSettings options;
    auto allowLan = options.value("allowLan").toBool();
    auto bindAddrV4 =
        allowLan ? QHostAddress::AnyIPv4 : QHostAddress::LocalHost;
    auto bindAddrV6 =
        allowLan ? QHostAddress::AnyIPv6 : QHostAddress::LocalHostIPv6;
    if (options.value("enV4").toBool()) {
        v4Socket = new QUdpSocket(this);
        v4Socket->bind(bindAddrV4, 53, QUdpSocket::ReuseAddressHint);
        connect(
            v4Socket, &QUdpSocket::readyRead, this, &DNServer::DNSQuestionProc);
    }
    if (options.value("enV6").toBool()) {
        v6Socket = new QUdpSocket(this);
        v6Socket->bind(bindAddrV6, 53, QUdpSocket::ReuseAddressHint);
        connect(
            v6Socket, &QUdpSocket::readyRead, this, &DNServer::DNSQuestionProc);
    }
}

void
DNServer::DNSQuestionProc()
{
    auto socket = static_cast<QUdpSocket*>(sender());
    while (socket->hasPendingDatagrams()) {
        auto datagram = socket->receiveDatagram();
        if (datagram.data().size() < 2) {
            continue;
        }
        uint16_t id = datagram.data().at(0);
        id <<= 8;
        id |= datagram.data().at(1) & 0xFF;
        clientRec.insert(id,
                         { datagram.senderAddress(), datagram.senderPort() });
        qDebug() << "DNS QUERY RECV\n" << datagram.data();
        spdlog::get("Logger")->debug("DNS Query received");
        emit DNSQuestionReceived(id, datagram.data());
    }
}

void
DNServer::sendDNSAnwser(uint16_t id, const QByteArray& ans)
{
    if (!clientRec.contains(id)) {
        return;
    }
    auto dest = clientRec[id];
    if (dest.addr.protocol() ==
        QAbstractSocket::NetworkLayerProtocol::IPv4Protocol) {
        v4Socket->writeDatagram(ans, dest.addr, dest.port);
        v4Socket->flush();
    } else {
        v6Socket->writeDatagram(ans, dest.addr, dest.port);
        v6Socket->flush();
    }
    clientRec.remove(id);
    qDebug() << "DNS ANSWER SEND\n" << ans;
    spdlog::get("Logger")->debug("DNS Answer Send");
}
