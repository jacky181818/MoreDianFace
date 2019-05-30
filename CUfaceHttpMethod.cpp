#include "CUfaceHttpMethod.h"
#include <QJsonDocument>
#include <QJsonObject>

CUfaceHttpMethod::CUfaceHttpMethod(CHTTP_Method *parent) : CHTTP_Method(parent)
{
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    headers.insert(QNetworkRequest::ContentDispositionHeader, QVariant(QString("application/x-www-form-urlencoded")));
    mRequest.setRequestHeader(headers);
}

CUfaceHttpMethod::~CUfaceHttpMethod()
{

}

bool CUfaceHttpMethod::ParseReplyResult(const QByteArray &reply, QJsonValue &value, QString &errMsg)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply);
    if (jsonDoc.isObject())
    {
        QJsonValue vResult = jsonDoc.object().value("success");
        if (vResult.toBool())
        {
            value = jsonDoc.object().value("data");
            return true;
        }
        else
        {
            QString strMsg = jsonDoc.object().value("msg").toString();
            errMsg = QString(u8"CHTTP_Method∂‘œÛ:%1, ").arg((long)this) + strMsg;
        }
    }
    return false;
}
