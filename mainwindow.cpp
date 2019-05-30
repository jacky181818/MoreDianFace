#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "swmhttp.h"
#include "qjsonserializer.h"
#include <QJsonDocument>
#include <QMap>
#include <QDebug>
#include <QInputDialog>
#include <QDateTime>
#include "dialog.h"
#include <QFile>
#include <QFileInfo>

#define APP_ID  "1632860700994961408"
#define APP_KEY  "lQfIu01V2zdhSE2YlP7BdbpMShjZAYSk"
#define URL_BASE "https://oapi.moredian.com/"
#define ORGID   "1634216433191747584"
#define ORGAUTHKEY  "4JGzAU76tjcAdOojhT1SmOB3jiq01Lm0"
#define DEVICE_SN "110201190308NC0002"

static MainWindow *ptrMainWindow = Q_NULLPTR;

IMPLEMENT_TW_LOG(MainWindow)

void MainWindow::myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString strMsg;
    switch (type) {
    case QtDebugMsg:
        strMsg = QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg("");
        break;
    case QtInfoMsg:
        strMsg = QString("Info: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg("");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg("");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg("");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg("");
        abort();
    }

    if (ptrMainWindow)
    {
        try{
            QMetaObject::invokeMethod(ptrMainWindow->ui->edtMessages, "append", Qt::AutoConnection, Q_ARG(const QString&, strMsg) );
        }
        catch(QException &e)
        {
            QString strExeption = e.what();
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ptrMainWindow = this;

    ui->setupUi(this);

    mSerializer = new QJsonSerializer(this);

    mOrgInfo.orgId = ORGID;
    mOrgInfo.orgAuthKey = ORGAUTHKEY;
    mActivationResult.sn = DEVICE_SN;

    QJsonSerializer::registerListConverters<QList<QString>>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGetAppToken_clicked()
{
    QString url = QString("%1app/getAppToken").arg(URL_BASE);
    qDebug() << "url: " << url;
    QMap<QString, QString> params;
    params["appId"] = APP_ID;
    params["appKey"] = APP_KEY;

    HTTPGet(url, params, [=](QJsonValue v){
        if (v.isObject())
        {
            QScopedPointer<APPToken>pToken(mSerializer->deserialize<APPToken *>(v.toObject()));
            if (!pToken.isNull())
            {
                mAppToken = *pToken;
                qDebug() << "app Token:" << pToken->appToken << ", expires:" << pToken->expires;
            }
        }
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });

}

void MainWindow::on_btnCreateOrg_clicked()
{
    QString url = QString("%1app/createOrg?appToken=%2").arg(URL_BASE).arg(mAppToken.appToken);
    qDebug() << "url: " << url;

    CreateOrgInfo info;
    info.orgName    = u8"易民测试";
    info.address    = u8"杭州市余杭区未来研创园";
    info.contact    = u8"阿狸";
    info.phone      = "13912345678";
    QJsonObject json = mSerializer->serialize(&info);
    QJsonDocument doc(json);
    QByteArray baParams = doc.toJson(QJsonDocument::Compact);

    HTTPPostRaw(url, baParams, [=](QJsonValue v){
        if (v.isObject())
        {
            QScopedPointer<OrgInfo>pOrgInfo(mSerializer->deserialize<OrgInfo *>(v.toObject()));
            if (!pOrgInfo.isNull())
            {
                mOrgInfo = *pOrgInfo;
                qDebug() << "orgId:" << pOrgInfo->orgId << ", orgAuthKey:" << pOrgInfo->orgAuthKey;
            }
        }
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnOrgAccessToken_clicked()
{
    QString url = QString("%1app/getOrgAccessToken").arg(URL_BASE);
    qDebug() << "url: " << url;
    QMap<QString, QString> params;
    params["appToken"]      = mAppToken.appToken;
    params["orgAuthKey"]    = mOrgInfo.orgAuthKey;
    params["orgId"]         = mOrgInfo.orgId;
    HTTPGet(url, params, [=](QJsonValue v){
        if (v.isObject())
        {
            QScopedPointer<AccessToken>pAccessToken(mSerializer->deserialize<AccessToken *>(v.toObject()));
            if (!pAccessToken.isNull())
            {
                mAccessToken = *pAccessToken;
                qDebug() << "access Token:" << pAccessToken->accessToken << ", expires:" << pAccessToken->expires;
            }
        }
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnOrgUpdate_clicked()
{
    QString url = QString("%1org/update?accessToken=%2")\
            .arg(URL_BASE).arg(mAccessToken.accessToken);
    qDebug() << "url: " << url;
    CreateOrgInfo info;
    info.orgName    = u8"易民测试";
    info.address    = u8"杭州市余杭区未来研创园";
    info.contact    = u8"阿狸";
    info.phone      = "13912345678";
    QJsonObject json = mSerializer->serialize(&info);
    QJsonDocument doc(json);
    QByteArray baParams = doc.toJson(QJsonDocument::Compact);

    HTTPPostRaw(url, baParams, [=](QJsonValue v){
        if (v.isObject())
        {
            qDebug() << u8"更新组织信息"  << (v.toBool() ? u8"：成功！" : u8"：失败！");
        }
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnDeviceActivation_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, u8"设备激活",
                                         u8"请输入魔点的设备QRCODE:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        QString url = QString("%1device/activation?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        QString strCode = text;
        QByteArray baParams = QString("{\"qrCode\":\"%1\"}").arg(strCode).toUtf8();

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            if (v.isObject())
            {
                QScopedPointer<ActivationResult>pActivationResult(mSerializer->deserialize<ActivationResult *>(v.toObject()));
                if (!pActivationResult.isNull())
                {
                    mActivationResult = *pActivationResult;
                    qDebug() << "satusCode:" << mActivationResult.statusCode << ", sn:" << mActivationResult.sn << ", message:" << mActivationResult.message;
                }
            }
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }

}

void MainWindow::on_btnGetDeviceId_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, u8"获取设备ID",
                                         u8"请输入设备SN:", QLineEdit::Normal,
                                         "110201190308NC0002", &ok);
    if (ok && !text.isEmpty())
    {
        QString url = QString("%1device/deviceId").arg(URL_BASE);

        QMap<QString, QString> params;
        params["accessToken"]   = mAccessToken.accessToken;
        params["deviceSn"]      = mActivationResult.sn;

        qDebug() << "url: " << url;

        HTTPGet(url, params, [=](QJsonValue v){
            if (v.isObject())
            {
                mStrDeviceId = v.toString();
                qDebug() << u8"设备ID:" << mStrDeviceId;
            }
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }


}

void MainWindow::on_btnUpdateControlInfo_clicked()
{
    QString url = QString("%1device/updateControlInfo?accessToken=%2")\
            .arg(URL_BASE).arg(mAccessToken.accessToken);
    qDebug() << "url: " << url;

    UpdateControlInfo info;
    info.deviceId = mStrDeviceId;
    info.multiPerson    = 0;
    info.controlDoorTime    = 5000;

    QJsonObject json = mSerializer->serialize(&info);
    QJsonDocument doc(json);
    QByteArray baParams = doc.toJson(QJsonDocument::Compact);

    HTTPPostRaw(url, baParams, [=](QJsonValue v){
        Q_UNUSED(v)
        qDebug() << "OK!" << Q_FUNC_INFO;
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnUnbind_clicked()
{
    QString url = QString("%1device/unbind?accessToken=%2")\
            .arg(URL_BASE).arg(mAccessToken.accessToken);
    qDebug() << "url: " << url;

    QByteArray baParams = QString("{\"deviceId\":\"%1\"}").arg(mStrDeviceId).toUtf8();

    HTTPPostRaw(url, baParams, [=](QJsonValue v){
        Q_UNUSED(v)
        qDebug() << "OK!" << Q_FUNC_INFO;
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnGetDynamicPwd_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, u8"获取动态密码",
                                         u8"请输入设备上的时间:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        QString url = QString("%1device/getDynamicPwd?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        DynamicPwdInfo info;
        info.deviceId = mStrDeviceId;
        info.timestamp = QDateTime::fromString(text).toMSecsSinceEpoch();

        QJsonObject json = mSerializer->serialize(&info);
        QJsonDocument doc(json);
        QByteArray baParams = doc.toJson(QJsonDocument::Compact);

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << "OK!" << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }

}

QString GetBase64Header(QString ext){
    QString strHeader = ext.toLower();
    if (strHeader.compare("jpg") == 0)
    {
        strHeader = "image/jpeg";
    }
    else if (strHeader.compare("png") == 0)
    {
        strHeader = "image/png";
    }
    else
    {
        strHeader = "";
    }
    return strHeader;
}

void MainWindow::on_btnMemberCreate_clicked()
{
    Dialog dlg;
    if (QDialog::Accepted == dlg.exec())
    {
        QVariantMap  results = dlg.results();

        QString url = QString("%1member/create?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        qDebug() << "Boundary: " << multiPart->boundary();

        QString strTemp;
        QHttpPart tpUserIdPart;
        tpUserIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"tpUserId\""));
        tpUserIdPart.setBody(results["id"].toString().toUtf8());
        multiPart->append(tpUserIdPart);

        QString strPictureFileName = results["picture"].toString();
        if (!strPictureFileName.isEmpty())
        {
            QFileInfo fi(strPictureFileName);
            QString ext = fi.suffix();

            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(GetBase64Header(ext)));
            QString strCDHeader = QString("form-data; name=\"verifyFace\"; filename=\"%1\"").arg(fi.fileName());
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(strCDHeader));
            QFile *file = new QFile(strPictureFileName);
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(imagePart);

        }
        QString strAvataFileName = results["avatar"].toString();
        if (!strAvataFileName.isEmpty())
        {
            QFileInfo fi(strAvataFileName);
            QString ext = fi.suffix();

            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(GetBase64Header(ext)));
            QString strCDHeader = QString("form-data; name=\"showFace\"; filename=\"%1\"").arg(fi.fileName());
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(strCDHeader));
            QFile *file = new QFile(strAvataFileName);
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(imagePart);
        }       

        QHttpPart memberNamePart;
        memberNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"memberName\""));
        memberNamePart.setBody(results["name"].toString().toUtf8());
        multiPart->append(memberNamePart);

        strTemp = results["idNumber"].toString();
        if (!strTemp.isEmpty())
        {
            QHttpPart certTypePart;
            certTypePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"certType\""));
            certTypePart.setBody("1");
            multiPart->append(certTypePart);

            QHttpPart certNoPart;
            certNoPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"certNo\""));
            certNoPart.setBody(strTemp.toUtf8());
            multiPart->append(certNoPart);
        }

        QHttpPart mobilePart;
        mobilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"mobile\""));
        mobilePart.setBody(results["mobile"].toString().toUtf8());
        multiPart->append(mobilePart);

        HTTPPostMultiPart(url, multiPart, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << u8"创建人员成功! 返回的魔点ID: " << v.toString();
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }
}

void MainWindow::on_btnMemberUpdate_clicked()
{
    Dialog dlg;
    if (QDialog::Accepted == dlg.exec())
    {
        QVariantMap  results = dlg.results();

        QString url = QString("%1member/update?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        qDebug() << "Boundary: " << multiPart->boundary();

        QString strTemp;
        QHttpPart tpUserIdPart;
        tpUserIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"memberId\""));
        tpUserIdPart.setBody(results["id"].toString().toUtf8());
        multiPart->append(tpUserIdPart);

        QString strPictureFileName = results["picture"].toString();
        if (!strPictureFileName.isEmpty())
        {
            QFileInfo fi(strPictureFileName);
            QString ext = fi.suffix();
            QString fileName = fi.fileName();
            qDebug() << fileName;
            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
            QString strCDHeader = QString("form-data; name=\"verifyFace\"; filename=\"%1\"").arg(fi.fileName());
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(strCDHeader));
            QFile *file = new QFile(strPictureFileName);
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(imagePart);
            connect(file, &QFile::aboutToClose, this, [=](){
                qDebug() << "file should be closed. fileName: " << strPictureFileName;
            });
        }
        QString strAvataFileName = results["avatar"].toString();
        if (!strAvataFileName.isEmpty())
        {
            QFileInfo fi(strAvataFileName);
            QString ext = fi.suffix();

            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(GetBase64Header(ext)));
            QString strCDHeader = QString("form-data; name=\"showFace\"; filename=\"%1\"").arg(fi.fileName());
            imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(strCDHeader));
            QFile *file = new QFile(strAvataFileName);
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(imagePart);
            connect(file, &QFile::aboutToClose, this, [=](){
                qDebug() << "file should be closed. fileName: " << strAvataFileName;
            });
        }

        strTemp = results["name"].toString();
        if (!strTemp.isEmpty())
        {
            QHttpPart memberNamePart;
            memberNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"memberName\""));
            memberNamePart.setBody(strTemp.toUtf8());
            multiPart->append(memberNamePart);
        }
        strTemp = results["idNumber"].toString();
        if (!strTemp.isEmpty())
        {
            QHttpPart certTypePart;
            certTypePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"certType\""));
            certTypePart.setBody("1");
            multiPart->append(certTypePart);

            QHttpPart certNoPart;
            certNoPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"certNo\""));
            certNoPart.setBody(strTemp.toUtf8());
            multiPart->append(certNoPart);
        }
        strTemp = results["mobile"].toString();
        if (!strTemp.isEmpty())
        {
            QHttpPart mobilePart;
            mobilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"mobile\""));
            mobilePart.setBody(results["mobile"].toString().toUtf8());
            multiPart->append(mobilePart);
        }
        HTTPPostMultiPart(url, multiPart, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << "OK!" << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }
}

void MainWindow::on_btnMemberDelete_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, u8"删除人员",
                                         u8"请输入在魔点公司的人员ID:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        QString url = QString("%1member/delete?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        QString content = QString("{\"memberId\":%1}").arg(text);
        QByteArray baParams = content.toUtf8();

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << "OK!" << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }
}

void MainWindow::on_btnUpdateVerifyImg_clicked()
{

}

void MainWindow::on_btnUpdateShowFace_clicked()
{

}

void MainWindow::on_btnGroupCreate_clicked()
{
    QString url = QString("%1group/create?accessToken=%2")\
            .arg(URL_BASE).arg(mAccessToken.accessToken);
    qDebug() << "url: " << url;

    GroupInfo info;
    info.tpGroupId = "eminDT";
    info.groupName = "defaultGroup";

    QJsonObject json = mSerializer->serialize(&info);
    QJsonDocument doc(json);
    QByteArray baParams = doc.toJson(QJsonDocument::Compact);

    HTTPPostRaw(url, baParams, [=](QJsonValue v){
        Q_UNUSED(v)
        qDebug() << u8"创建群组OK! 返回的魔点群组ID:" << v.toString() << Q_FUNC_INFO;
    },
    [=](QString e){
        qDebug() << e;
        TW_LOG_ERROR(qPrintable(e));
    });
}

void MainWindow::on_btnGroupUpdate_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, u8"修改群组",
                                         u8"请输入魔点的群组ID:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        QString url = QString("%1group/update?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        GroupInfo info;
        info.tpGroupId = text;
        info.groupName = "eminDTGroup";

        QJsonObject json = mSerializer->serialize(&info);
        QJsonDocument doc(json);
        QByteArray baParams = doc.toJson(QJsonDocument::Compact);
        baParams.replace("tpGroupId", "groupId");

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << u8"修改群组OK! " << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }

}

void MainWindow::on_btnBindGroup_clicked()
{
    bool ok;
    QString strMemberId, strGroupId;
    QString text = QInputDialog::getText(this, u8"绑定人员到群组",
                                         u8"请输入魔点的人员ID:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty())
    {
        return;
    }

    strMemberId = text;

    text = QInputDialog::getText(this, u8"绑定人员到群组",
                                             u8"请输入魔点的群组ID:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !text.isEmpty())
    {
        strGroupId = text;

        QString url = QString("%1member/bindGroup?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        BindGroupInfo info;
        info.memberId = strMemberId;
        info.groupId = strGroupId;

        QJsonObject json = mSerializer->serialize(&info);
        QJsonDocument doc(json);
        QByteArray baParams = doc.toJson(QJsonDocument::Compact);

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << u8"绑定人员到群组OK! " << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }
}

void MainWindow::on_btnUnbindGroup_clicked()
{
    bool ok;
    QString strMemberId, strGroupId;
    QString text = QInputDialog::getText(this, u8"解除人员和群组的绑定关系",
                                         u8"请输入魔点的人员ID:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty())
    {
        return;
    }

    strMemberId = text;

    text = QInputDialog::getText(this, u8"解除人员和群组的绑定关系",
                                             u8"请输入魔点的群组ID:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !text.isEmpty())
    {
        strGroupId = text;

        QString url = QString("%1member/unbindGroup?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        BindGroupInfo info;
        info.memberId = strMemberId;
        info.groupId = strGroupId;

        QJsonObject json = mSerializer->serialize(&info);
        QJsonDocument doc(json);
        QByteArray baParams = doc.toJson(QJsonDocument::Compact);

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << u8"解除人员和群组的绑定关系OK! " << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }
}

void MainWindow::on_btnBinding_clicked()
{
    bool ok;
    QString strDeviceId, strGroupId;
    QString text = QInputDialog::getText(this, u8"设备与群组绑定",
                                         u8"请输入魔点的设备ID:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty())
    {
        return;
    }

    strDeviceId = text;

    text = QInputDialog::getText(this, u8"设备与群组绑定",
                                             u8"请输入魔点的群组ID列表，ID之间用英文逗号(,)分割:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !text.isEmpty())
    {
        strGroupId = text;

        QString url = QString("%1device/group/binding?accessToken=%2")\
                .arg(URL_BASE).arg(mAccessToken.accessToken);
        qDebug() << "url: " << url;

        DeviceBindGroupInfo info;
        info.deviceId = strDeviceId;
        QList<QString> lstGroup;
        foreach(QString str, strGroupId.split(';'))
        {
            lstGroup.append(str);
        }
        info.groupIdList = lstGroup;

        QJsonObject json = mSerializer->serialize(&info);
        QJsonDocument doc(json);
        QByteArray baParams = doc.toJson(QJsonDocument::Compact);

        HTTPPostRaw(url, baParams, [=](QJsonValue v){
            Q_UNUSED(v)
            qDebug() << u8"设备与群组绑定OK! " << Q_FUNC_INFO;
        },
        [=](QString e){
            qDebug() << e;
            TW_LOG_ERROR(qPrintable(e));
        });
    }

}

void MainWindow::on_btnConfiguration_clicked()
{

}
