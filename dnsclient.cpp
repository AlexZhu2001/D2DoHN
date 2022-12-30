#include "dnsclient.h"
#include <QNetworkDatagram>
#include <QSettings>

DNSClient::DNSClient(QObject* parent)
    : QObject{ parent }
{
    socket.bind(40000);
    connect(&socket, &QUdpSocket::readyRead, this, &DNSClient::DNSAnswerProc);
}

void
DNSClient::sendDNSQuery(uint16_t id, const QByteArray& query)
{
    QSettings options;
    auto dnsAddr = options.value("DNS_ADDR").toString();
    auto dnsServer = QHostAddress(dnsAddr);
    socket.writeDatagram(query, dnsServer, 53);
    socket.flush();
}

void
DNSClient::DNSAnswerProc()
{
    auto socket = static_cast<QUdpSocket*>(sender());
    while (socket->hasPendingDatagrams()) {
        auto datagram = socket->receiveDatagram();
        auto ans = datagram.data();
        if (ans.size() < 2) {
            return;
        }
        uint16_t id = ans.at(0);
        id <<= 8;
        id |= (ans.at(1) & 0xFF);
        emit DNSAnswerReceived(id, ans);
    }
}
