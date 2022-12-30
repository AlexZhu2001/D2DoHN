#include "container.h"
#include <QObject>

Container::Container()
{
    server = new DNServer;
    dohClient = new DoHClient;
    dnsClient = new DNSClient;
    QObject::connect(server,
                     &DNServer::DNSQuestionReceived,
                     dohClient,
                     &DoHClient::SendDoHRequest);
    QObject::connect(dohClient,
                     &DoHClient::DoHAnswerReceived,
                     server,
                     &DNServer::sendDNSAnwser);
    QObject::connect(dohClient,
                     &DoHClient::DoHErrorOccurred,
                     dnsClient,
                     &DNSClient::sendDNSQuery);
    QObject::connect(dnsClient,
                     &DNSClient::DNSAnswerReceived,
                     server,
                     &DNServer::sendDNSAnwser);
}

Container::~Container()
{
    delete dnsClient;
    delete dohClient;
    delete server;
}
