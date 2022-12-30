#ifndef CONTAINER_H
#define CONTAINER_H

#include "dnsclient.h"
#include "dnserver.h"
#include "dohclient.h"
#include <QObject>

class Container
{
public:
    explicit Container();
    ~Container();

private:
    DNServer* server = nullptr;
    DoHClient* dohClient = nullptr;
    DNSClient* dnsClient = nullptr;

signals:
};

#endif // CONTAINER_H
