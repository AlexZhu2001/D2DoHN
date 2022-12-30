#ifndef DNSCLIENT_H
#define DNSCLIENT_H

#include <QObject>
#include <QUdpSocket>

class DNSClient : public QObject
{
    Q_OBJECT
public:
    explicit DNSClient(QObject* parent = nullptr);

private:
    QUdpSocket socket;

public slots:
    void sendDNSQuery(uint16_t id, const QByteArray& query);

private slots:
    void DNSAnswerProc();

signals:
    void DNSAnswerReceived(uint16_t id, const QByteArray& ans);
};

#endif // DNSCLIENT_H
