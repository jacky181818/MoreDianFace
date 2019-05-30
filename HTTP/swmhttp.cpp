#include "swmhttp.h"
#include "CMoreDianHttpMethod.h"

bool HTTPGet(const QString &url, QMap<QString, QString> &getParams, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType)
{
    CMoreDianHttpMethod *http = new CMoreDianHttpMethod();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    if (headerType == HTTP_CONTENT_TYPE_HEADER::X_WWW_FORM_URLENCODED)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/x-www-form-urlencoded")));
        http->setRequestHeader(headers);
    }
    else if (headerType == HTTP_CONTENT_TYPE_HEADER::JSON)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json;charset=utf-8")));
        http->setRequestHeader(headers);
    }
    return http->Get(url, getParams, succeeded, failed);

}

bool HTTPPost(const QString &url, QMap<QString, QString> &postParams, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType)
{
    CMoreDianHttpMethod *http = new CMoreDianHttpMethod();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    if (headerType == HTTP_CONTENT_TYPE_HEADER::X_WWW_FORM_URLENCODED)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/x-www-form-urlencoded")));
        http->setRequestHeader(headers);
    }
    else if (headerType == HTTP_CONTENT_TYPE_HEADER::JSON)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json;charset=utf-8")));
        http->setRequestHeader(headers);
    }
    return http->Post(url, postParams, succeeded, failed);
}

bool HTTPPostRaw(const QString &url, QByteArray &postDatas, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType)
{
    CMoreDianHttpMethod *http = new CMoreDianHttpMethod();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    if (headerType == HTTP_CONTENT_TYPE_HEADER::X_WWW_FORM_URLENCODED)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/x-www-form-urlencoded")));
        http->setRequestHeader(headers);
    }
    else if (headerType == HTTP_CONTENT_TYPE_HEADER::JSON)
    {
        headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json;charset=utf-8")));
        http->setRequestHeader(headers);
    }
    return http->Post(url, postDatas, succeeded, failed);
}

bool HTTPPostMultiPart(const QString &url, QHttpMultiPart *multipart, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed)
{
    CMoreDianHttpMethod *http = new CMoreDianHttpMethod();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    QString strHeader = QString("multipart/form-data;boundary=%1").arg(QString(multipart->boundary()));
    qDebug() << "Boundary: " << strHeader;
    headers.insert(QNetworkRequest::ContentTypeHeader, QVariant(strHeader));
    http->setRequestHeader(headers);
    return http->Post(url, multipart, succeeded, failed);
}
