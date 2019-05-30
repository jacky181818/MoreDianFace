#ifndef CHTTPREQUEST_H
#define CHTTPREQUEST_H

#include <QObject>
#include <functional>
#include <QJsonValue>
#include <httprequest.h>
#include "mylogger.h"
#include <QHttpMultiPart>

class CHTTP_Method : public QObject
{
    Q_OBJECT
    DEFINE_TW_LOG()
public:
    CHTTP_Method(QObject *parent = 0);
    virtual ~CHTTP_Method();
    bool Get(const QString &url, QMap<QString, QString> &getParams, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);
    bool Post(const QString &url, QMap<QString, QString> &postParams, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);
    bool Post(const QString &url, QByteArray &postDatas, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);
    bool Post(const QString &url, QHttpMultiPart *multipart, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);

    /**
     * @brief ParseReplyResult  解析服务器返回的结果
     * @param reply 服务器返回的结果
     * @param value 响应结果为成功时，解析出来到的JsonValue
     * @param errMsg 响应结果为失败时，给出的失败原因
     * @return false: 网络错误或服务器返回错误结果   true: 正确返回结果
     */
    virtual bool ParseReplyResult(const QByteArray &reply, QJsonValue &value, QString &errMsg) = 0;

protected:
    QString mUrl;
    QMap<QString, QString> mGetParams;
    std::function<void (QJsonValue)> mSucceeded;
    std::function<void (QString)> mFailed;
    HTTPRequest mRequest;
private:
    bool Method(int type, const QString &url, QVariant &params, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed);

public:
    static QVariantMap strMap2varMap(QMap<QString, QString> params);
    static QMap<QString, QString> varMap2strMap(QVariantMap params);
};



#endif // CHTTPREQUEST_H
