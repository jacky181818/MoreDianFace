#ifndef CMOREDIANHTTPMETHOD_H
#define CMOREDIANHTTPMETHOD_H

#include "chttprequest.h"
#include "mylogger.h"

class CMoreDianHttpMethod : public CHTTP_Method
{
    Q_OBJECT
    DEFINE_TW_LOG()
public:
    explicit CMoreDianHttpMethod(CHTTP_Method *parent = nullptr);
    virtual ~CMoreDianHttpMethod() override;

    bool ParseReplyResult(const QByteArray &reply, QJsonValue &value, QString &errMsg) override;
    void setRequestHeader(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers);

signals:

public slots:
};

#endif // CMOREDIANHTTPMETHOD_H
