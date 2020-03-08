﻿#ifndef REQUEST_H
#define REQUEST_H
#include <QByteArray>
#include <memory>
#include <QString>
#include <QNetworkReply>


class QNetworkRequest;
class QNetworkAccessManager;

class Request
{
public:
    Request();
    virtual ~Request() = default;
    virtual std::unique_ptr<QNetworkReply> sendRequest(QNetworkAccessManager *manager) = 0;
    virtual QString requestName() const = 0;
    virtual QUrl destinationUrl() const;
    virtual bool canSendRequest(QNetworkAccessManager *manager) const;
    QString address() const;
    void setAddress(const QString &address);
    int port() const;
    void setPort(int port);
private:
    QString mAddress;
    int mPort;
};

#endif // REQUEST_H
