#ifndef SWMHTTP_H
#define SWMHTTP_H

#include <QMap>
#include <functional>
#include <QJsonValue>
#include <QHttpMultiPart>

typedef enum {
    X_WWW_FORM_URLENCODED,
    JSON
}HTTP_CONTENT_TYPE_HEADER;

bool HTTPGet(const QString &url, QMap<QString, QString> &getParams, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType = HTTP_CONTENT_TYPE_HEADER::JSON);
bool HTTPPost(const QString &url, QMap<QString, QString> &postParams,  std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType = HTTP_CONTENT_TYPE_HEADER::JSON);
bool HTTPPostRaw(const QString &url, QByteArray &postDatas,  std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed, HTTP_CONTENT_TYPE_HEADER headerType = HTTP_CONTENT_TYPE_HEADER::JSON);
bool HTTPPostMultiPart(const QString &url, QHttpMultiPart *multipart,  std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);

#endif // SWMHTTP_H
