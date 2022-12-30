#include "dohclient.h"

#include <QNetworkReply>
#include <QSettings>
#include <spdlog/spdlog.h>

DoHClient::DoHClient(QObject* parent)
    : QObject{ parent }
{
}

void
DoHClient::SendDoHRequest(uint16_t id, const QByteArray& query)
{
    Q_UNUSED(id);
    QSettings options;
    auto url = options.value("DoH_URL").toString();
    QNetworkRequest req((QUrl(url)));
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  "application/dns-message");
    auto rep = manager.post(req, query);
    connect(rep, &QNetworkReply::finished, this, &DoHClient::onDoHResponse);
    idRec.insert(rep, query);
    qDebug() << "DoH req" << query;
    spdlog::get("Logger")->debug("DoH Request Send");
}

void
DoHClient::onDoHResponse()
{
    auto rep = static_cast<QNetworkReply*>(sender());
    uint16_t id = idRec[rep].at(0);
    id <<= 8;
    id |= (idRec[rep].at(1) & 0xFF);
    auto code =
        rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    if (rep->error() != QNetworkReply::NetworkError::NoError || code != "200") {
        emit DoHErrorOccurred(id, idRec[rep]);
        qDebug() << "DoH ERROR OCCURRED";
        spdlog::get("Logger")->error("!!!DoH Error Occurred!!!");
        idRec.remove(rep);
    } else {
        auto ans = rep->readAll();
        emit DoHAnswerReceived(id, ans);
        qDebug() << "DoH ANSWER RECV" << ans;
        spdlog::get("Logger")->debug("DoH Answer Received");
        idRec.remove(rep);
    }
    rep->deleteLater();
    return;
}
