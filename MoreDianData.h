#ifndef MOREDIANDATA_H
#define MOREDIANDATA_H

#include <QObject>

namespace MoreDian {

class APPToken : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appToken MEMBER appToken)
    Q_PROPERTY(long expires MEMBER expires)
public:
    Q_INVOKABLE APPToken(QObject *parent = nullptr);
    APPToken &operator =(const APPToken &obj);

    QString appToken;
    long expires;
};

class CreateOrgInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString orgName MEMBER orgName)
    Q_PROPERTY(QString address MEMBER address)
    Q_PROPERTY(QString contact MEMBER contact)
    Q_PROPERTY(QString phone MEMBER phone)
public:
    Q_INVOKABLE CreateOrgInfo(QObject *parent = nullptr);
    CreateOrgInfo &operator =(const CreateOrgInfo &obj);

    QString orgName;
    QString address;
    QString contact;
    QString phone;
};

class OrgInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString orgId MEMBER orgId)
    Q_PROPERTY(QString orgAuthKey MEMBER orgAuthKey)
public:
    Q_INVOKABLE OrgInfo(QObject *parent = nullptr);
    OrgInfo &operator =(const OrgInfo &obj);

    QString orgId;
    QString orgAuthKey;
};

class AccessToken : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString accessToken MEMBER accessToken)
    Q_PROPERTY(long expires MEMBER expires)
public:
    Q_INVOKABLE AccessToken(QObject *parent = nullptr);
    AccessToken &operator =(const AccessToken &obj);

    QString accessToken;
    long expires;
};

class ActivationResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message MEMBER message)
    Q_PROPERTY(QString sn MEMBER sn)
    Q_PROPERTY(int statusCode MEMBER statusCode)
public:
    Q_INVOKABLE ActivationResult(QObject *parent = nullptr);
    ActivationResult &operator =(const ActivationResult &obj);

    QString message;
    int statusCode;
    QString sn;
};

class UpdateControlInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceId MEMBER deviceId)
    Q_PROPERTY(int multiPerson MEMBER multiPerson)
    Q_PROPERTY(int controlDoorTime MEMBER controlDoorTime)
public:
    Q_INVOKABLE UpdateControlInfo(QObject *parent = nullptr);
    UpdateControlInfo &operator =(const UpdateControlInfo &obj);

    QString deviceId;
    int multiPerson;
    int controlDoorTime;
};

class DynamicPwdInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceId MEMBER deviceId)
    Q_PROPERTY(qint64 timestamp MEMBER timestamp)
public:
    Q_INVOKABLE DynamicPwdInfo(QObject *parent = nullptr);

    QString deviceId;
    qint64 timestamp;
};

class GroupInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tpGroupId MEMBER tpGroupId)
    Q_PROPERTY(QString groupName MEMBER groupName)
public:
    Q_INVOKABLE GroupInfo(QObject *parent = nullptr);

    QString tpGroupId;
    QString groupName;
};

class BindGroupInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString memberId MEMBER memberId)
    Q_PROPERTY(QString groupId MEMBER groupId)
public:
    Q_INVOKABLE BindGroupInfo(QObject *parent = nullptr);

    QString memberId;
    QString groupId;
};

class DeviceBindGroupInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceId MEMBER deviceId)
    Q_PROPERTY(QList<QString> groupIdList MEMBER groupIdList)
public:
    Q_INVOKABLE DeviceBindGroupInfo(QObject *parent = nullptr);

    QString deviceId;
    QList<QString> groupIdList;
};

}



#endif // MOREDIANDATA_H
