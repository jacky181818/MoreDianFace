#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "httprequest_global.h"
#include <QObject>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <functional>
#include <iostream>
using namespace std;

class HTTPRequest : public QObject
{
    Q_OBJECT
public:
    explicit HTTPRequest(QObject *parent = nullptr, QNetworkAccessManager *networkManager = nullptr);

    ~HTTPRequest();

    void setRequestHeader(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers);

    void post(const QString &url, QHttpMultiPart *multipart);
    void post(const QString &url, QMap<QString, QString> &postData);
    void post(const QString &url, const QByteArray &baData);
    void get(const QString &url);
    void get(const QString &url, QMap<QString, QString> &getParams);

    void setHandlerFunc (std::function<void (QNetworkReply *reply)> handler_func)
    {
        _handler_func = handler_func;
    }
signals:

public slots:

private slots:
    void onRequestCompleted();
    void onRequestFailed(QNetworkReply::NetworkError error);

private:
    std::function<void (QNetworkReply *reply)> _handler_func;
    QNetworkAccessManager *m_manager;
    QMap<QNetworkRequest::KnownHeaders, QVariant> mRequestHeaders;

private:
    inline void setHeaders(QNetworkRequest &request);
};


#endif // HTTPREQUEST_H
