#include "MoreDianData.h"

namespace MoreDian {

APPToken::APPToken(QObject *parent) : QObject(parent)
{

}

APPToken &APPToken::operator =(const APPToken &obj)
{
    appToken    = obj.appToken;
    expires     = obj.expires;
    return *this;
}

OrgInfo::OrgInfo(QObject *parent) : QObject(parent)
{

}

OrgInfo &OrgInfo::operator =(const OrgInfo &obj)
{
    orgId   = obj.orgId;
    orgAuthKey = obj.orgAuthKey;
    return *this;
}

AccessToken::AccessToken(QObject *parent) : QObject(parent)
{

}

AccessToken &AccessToken::operator =(const AccessToken &obj)
{
    accessToken = obj.accessToken;
    expires = obj.expires;
    return *this;
}

CreateOrgInfo::CreateOrgInfo(QObject *parent) : QObject(parent)
{

}

CreateOrgInfo &CreateOrgInfo::operator =(const CreateOrgInfo &obj)
{
    orgName = obj.orgName;
    address = obj.address;
    contact = obj.contact;
    phone   = obj.phone;
    return *this;
}

ActivationResult::ActivationResult(QObject *parent) : QObject(parent)
{

}

ActivationResult &ActivationResult::operator =(const ActivationResult &obj)
{
    message     = obj.message;
    statusCode  = obj.statusCode;
    sn          = obj.sn;
    return *this;
}

UpdateControlInfo::UpdateControlInfo(QObject *parent) : QObject(parent)
{

}

UpdateControlInfo &UpdateControlInfo::operator =(const UpdateControlInfo &obj)
{
    deviceId    = obj.deviceId;
    multiPerson = obj.multiPerson;
    controlDoorTime = obj.controlDoorTime;
    return *this;
}

DynamicPwdInfo::DynamicPwdInfo(QObject *parent) : QObject (parent)
{
}

GroupInfo::GroupInfo(QObject *parent) : QObject (parent)
{

}

BindGroupInfo::BindGroupInfo(QObject *parent) : QObject (parent)
{

}

DeviceBindGroupInfo::DeviceBindGroupInfo(QObject *parent) : QObject (parent)
{

}

}
