#ifndef DNSERVER_H
#define DNSERVER_H

#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>

class DNServer : public QObject
{
    Q_OBJECT
public:
    explicit DNServer(QObject* parent = nullptr);

private:
    struct Dest
    {
        QHostAddress addr;
        int port;
    };
    QMap<uint16_t, Dest> clientRec;
    QUdpSocket* v4Socket = nullptr;
    QUdpSocket* v6Socket = nullptr;

private slots:
    void DNSQuestionProc();

public slots:
    void sendDNSAnwser(uint16_t id, const QByteArray& ans);

signals:
    void DNSQuestionReceived(uint16_t id, const QByteArray& question);
};

#endif // DNSERVER_H
