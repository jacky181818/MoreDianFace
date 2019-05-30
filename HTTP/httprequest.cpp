#include "httprequest.h"
#include <QStringList>
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpMultiPart>

HTTPRequest::HTTPRequest(QObject *parent, QNetworkAccessManager *networkManager) : QObject(parent)
{
    if (!networkManager) {
        m_manager = new QNetworkAccessManager(this);
    }
    else {
        m_manager = networkManager;
    }
}

HTTPRequest::~HTTPRequest()
{

}

void HTTPRequest::setRequestHeader(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers)
{
    mRequestHeaders = headers;
}

void HTTPRequest::post(const QString &url, QHttpMultiPart *multipart)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    setHeaders(request);

    QNetworkReply *reply = m_manager->post(request, multipart);
    multipart->setParent(reply);

    connect(reply, SIGNAL(finished()), this, SLOT(onRequestCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestFailed(QNetworkReply::NetworkError)));
}

QHttpPart part_parameter(QString key, QString value) {
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
    part.setBody(value.toLatin1());
    return part;
}

void HTTPRequest::post(const QString &url, QMap<QString, QString> &postData)
{       
    QList<QByteArray> paramLst;
    foreach (QString key, postData.keys()) {
        QByteArray key_value;
        key_value.append(key + "=" + postData[key]);
        paramLst.push_back(key_value);
    }
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    setHeaders(request);
    QNetworkReply *reply = m_manager->post(request, paramLst.join('&'));

    connect(reply, SIGNAL(finished()), this, SLOT(onRequestCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestFailed(QNetworkReply::NetworkError)));
}

void HTTPRequest::post(const QString &url, const QByteArray &baData)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    setHeaders(request);

    QNetworkReply *reply = m_manager->post(request, baData);

    connect(reply, SIGNAL(finished()), this, SLOT(onRequestCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestFailed(QNetworkReply::NetworkError)));

}

void HTTPRequest::get(const QString &url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    setHeaders(request);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(onRequestCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestFailed(QNetworkReply::NetworkError)));
}

void HTTPRequest::get(const QString &url, QMap<QString, QString> &getParams)
{
    QNetworkRequest request;

    QString strUrl = url;
    strUrl.append("?");
    QList<QByteArray> paramLst;
    foreach (QString key, getParams.keys()) {
        QByteArray key_value;
        key_value.append(key + "=" + getParams[key]);
        paramLst.push_back(key_value);
    }
    strUrl.append(paramLst.join('&'));
    request.setUrl(QUrl(strUrl));
    setHeaders(request);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(onRequestCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestFailed(QNetworkReply::NetworkError)));
}

void HTTPRequest::onRequestCompleted() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) //³É¹¦
    {
#if 0
        QVariant header = reply->request().header(QNetworkRequest::ContentDispositionHeader);
        if (header.isValid())
            qDebug() << "--- header --- : " << "ContentDispositionHeader" << header.toString();
        header = reply->request().header(QNetworkRequest::ContentTypeHeader);
        if (header.isValid())
            qDebug() << "--- header --- : " << "ContentTypeHeader" << header.toString();

        QList<QByteArray> rawHeaders = reply->request().rawHeaderList();
        foreach(QByteArray hd, rawHeaders)
        {
            qDebug() << "--- raw_header --- : " << hd << reply->request().rawHeader(hd);
        }
#endif
        if ( _handler_func)
        {
            _handler_func(reply);
        }

        reply->deleteLater();
    }
}

void HTTPRequest::onRequestFailed(QNetworkReply::NetworkError error) {
    Q_UNUSED(error);
    if (_handler_func)
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
        qDebug(qPrintable(reply->errorString()));
        _handler_func(reply);
    }
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
}

void HTTPRequest::setHeaders(QNetworkRequest &request)
{
    QMap<QNetworkRequest::KnownHeaders, QVariant>::const_iterator it = mRequestHeaders.constBegin();
    while (it != mRequestHeaders.constEnd()) {
        request.setHeader(it.key(), it.value());
        ++it;
    }
}
