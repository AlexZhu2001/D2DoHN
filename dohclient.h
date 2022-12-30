#ifndef DOHCLIENT_H
#define DOHCLIENT_H

#include <QNetworkAccessManager>
#include <QObject>

class DoHClient : public QObject
{
    Q_OBJECT
public:
    explicit DoHClient(QObject* parent = nullptr);

private:
    QNetworkAccessManager manager;
    QMap<QNetworkReply*, QByteArray> idRec;

public slots:
    void SendDoHRequest(uint16_t id, const QByteArray& query);

private slots:
    void onDoHResponse();

signals:
    void DoHAnswerReceived(uint16_t id, const QByteArray& ans);
    void DoHErrorOccurred(uint16_t id, const QByteArray& query);
};

#endif // DOHCLIENT_H
