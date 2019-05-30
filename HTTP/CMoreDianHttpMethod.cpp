#include "CMoreDianHttpMethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CMoreDianHttpMethod::CMoreDianHttpMethod(CHTTP_Method *parent) : CHTTP_Method(parent)
{
//    qDebug() << QString(u8"构造CMoreDianHttpMethod. this:%1").arg((long)this);
}

CMoreDianHttpMethod::~CMoreDianHttpMethod()
{
//    qDebug() << QString(u8"析构CMoreDianHttpMethod. this:%1").arg((long)this);
}

bool CMoreDianHttpMethod::ParseReplyResult(const QByteArray &reply, QJsonValue &value, QString &errMsg)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply);
    if (jsonDoc.isObject())
    {
        QJsonValue vResult = jsonDoc.object().value("result");
        if (vResult.toString().toInt() == 0)
        {
            value = jsonDoc.object().value("data");
            return true;
        }
        else
        {
            QString strMsg = jsonDoc.object().value("message").toString();
            errMsg = QString(u8"CMoreDianHttpMethod对象:%1, ").arg(long(this)) + strMsg;
        }
    }
    return false;
}

void CMoreDianHttpMethod::setRequestHeader(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers)
{
    mRequest.setRequestHeader(headers);
}
