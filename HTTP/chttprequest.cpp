#include "chttprequest.h"

#include <time.h>
#include <QDebug>
#include <QCoreApplication>

const int TIMEOUT = 45*1000;
const int HTTP_GET      = 0;
const int HTTP_POST     = 1;

CHTTP_Method::CHTTP_Method(QObject *parent) : QObject(parent)
{
//    qDebug() << QString(u8"构造CHttp_Method. this:%1").arg((long)this);
}

CHTTP_Method::~CHTTP_Method()
{
//    qDebug() << QString(u8"析构CHttp_Method. this:%1").arg((long)this);
}

bool CHTTP_Method::Get(const QString &url,
                       QMap<QString,
                       QString> &getParams,
                       std::function<void (QJsonValue)> succeeded,
                       std::function<void (QString)> failed)
{
    QVariantMap varParams = strMap2varMap(getParams);
    QVariant params = QVariant(varParams);
    return Method(HTTP_GET, url, params, succeeded, failed);
}

bool CHTTP_Method::Post(const QString &url,
                        QMap<QString, QString> &getParams,
                        std::function<void (QJsonValue)> succeeded,
                        std::function<void (QString)> failed)
{
    QVariantMap varParams = strMap2varMap(getParams);
    QVariant params = QVariant(varParams);
    return Method(HTTP_POST, url, params, succeeded, failed);
}

bool CHTTP_Method::Post(const QString &url, QByteArray &postDatas, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed)
{
    QVariant params = QVariant(postDatas);
    return Method(HTTP_POST, url, params, succeeded, failed);
}

bool CHTTP_Method::Post(const QString &url, QHttpMultiPart *multipart, std::function<void (QJsonValue)> succeeded, std::function<void (QString)> failed)
{
    QVariant params = QVariant::fromValue(multipart);
    return Method(HTTP_POST, url, params, succeeded, failed);
}


bool CHTTP_Method::Method(int type,
                          const QString &url,
                          QVariant &params,
                          std::function<void (QJsonValue)> succeeded,
                          std::function<void (QString)> failed)
{
    QTime tTimeOut;
    bool bFinished = false, isSucceed = false;
    QString strError;
    QJsonValue vData;
    mRequest.setHandlerFunc([&] (QNetworkReply *reply) {
        bFinished = true;
        if (reply->error() == QNetworkReply::NoError) { //成功
            QByteArray ba = reply->readAll();

            qDebug() << QString::fromUtf8(ba);
            isSucceed = ParseReplyResult(ba, vData, strError);
            /*
            QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);
            if (jsonDoc.isObject())
            {
                QJsonValue vResult = jsonDoc.object().value("success");
                if (vResult.toBool())
                {
                    isSucceed = true;
                    vData = jsonDoc.object().value("data");
                }
                else
                {
                    QString strMsg = jsonDoc.object().value("msg").toString();
                    strError = QString(u8"CHTTP_Method对象:%1, ").arg((long)this) + strMsg;
                }
            }
        */
        }
        else
        {
            QString strMsg = reply->errorString();
            strError = QString(u8"CHTTP_Method对象:%1, ").arg(long(this)) + strMsg;
        }
    });

    tTimeOut.start();

    bool bTimeouted = false;
    switch (type) {
    case HTTP_POST:
    {
        if (params.canConvert(QMetaType::QVariantMap))
        {
            QMap<QString, QString> strParams = varMap2strMap(params.toMap());
            mRequest.post(url, strParams);
        }
        else if (params.canConvert(QMetaType::QByteArray))
        {
            mRequest.post(url, params.toByteArray());
        }
        else if (params.canConvert(QMetaType::QObjectStar))
        {
            QHttpMultiPart *multipart = params.value<QHttpMultiPart *>();
            if (multipart)
            {
                mRequest.post(url, multipart);
            }
        }
        else
        {
            qDebug() << "ERROR";
        }
    }
        break;
    default:
    {
        QMap<QString, QString> strParams = varMap2strMap(params.toMap());
        mRequest.get(url, strParams);
    }
        break;
    }

    while (!bFinished) {
        qApp->processEvents();
        if (tTimeOut.elapsed() >= TIMEOUT) {
            bTimeouted = true;
            strError = QString(u8"操作超时. CHTTP_Method对象:%1.").arg(long(this));
            break;
        }
    }

    if (isSucceed)
        succeeded(vData);
    else
        failed(strError);

    this->deleteLater();

    return isSucceed;
}

QVariantMap CHTTP_Method::strMap2varMap(QMap<QString, QString> params)
{
    QVariantMap varMapParams;
    QMap<QString, QString>::const_iterator i = params.constBegin();
    while (i != params.constEnd()) {
        varMapParams.insert(i.key(), i.value());
        ++i;
    }
    return varMapParams;
}

QMap<QString, QString> CHTTP_Method::varMap2strMap(QVariantMap params)
{
    QMap<QString, QString> strParams;
    QVariantMap::const_iterator i = params.constBegin();
    while (i != params.constEnd()) {
        strParams.insert(i.key(), i.value().toString());
        ++i;
    }
    return strParams;
}


