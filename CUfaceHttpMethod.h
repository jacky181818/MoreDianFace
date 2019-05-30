#ifndef CUFACEHTTPMETHOD_H
#define CUFACEHTTPMETHOD_H

#include "chttprequest.h"
#include "mylogger.h"

class CUfaceHttpMethod : public CHTTP_Method
{
    Q_OBJECT
public:
    explicit CUfaceHttpMethod(CHTTP_Method *parent = 0);
    virtual ~CUfaceHttpMethod();

    bool ParseReplyResult(const QByteArray &reply, QJsonValue &value, QString &errMsg) override;

signals:

public slots:
};

#endif // CUFACEHTTPMETHOD_H
