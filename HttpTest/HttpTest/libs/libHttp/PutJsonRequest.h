﻿#ifndef PUTJSONREQUEST_H
#define PUTJSONREQUEST_H
#include "PostJsonRequest.h"
#include <QJsonObject>

class PutJsonRequest : public PostJsonRequest
{
public:
    PutJsonRequest() = default;
    void addGetParameter(const QString &parameterName, const QString &parameterValue);
    void addPostParameter(const QString &parameterName, const QString &parameterValue);
    void addParameter(const QString &parameterName, const QString &parameterValue) = delete;
    std::unique_ptr<QNetworkReply> sendRequest(QNetworkAccessManager *manager) override;
    QString requestName() const override;
private:
    std::map<QString, QString> mGetParameters;
    std::map<QString, QString> mPostParameters;
};

#endif // PUTJSONREQUEST_H
