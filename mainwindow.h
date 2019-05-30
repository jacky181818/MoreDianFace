#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mylogger.h"
#include "QJsonSerializer"

#include "MoreDianData.h"

namespace Ui {
class MainWindow;
}

using namespace MoreDian;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DEFINE_TW_LOG()
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnGetAppToken_clicked();

    void on_btnCreateOrg_clicked();

    void on_btnOrgAccessToken_clicked();

    void on_btnOrgUpdate_clicked();

    void on_btnDeviceActivation_clicked();

    void on_btnGetDeviceId_clicked();

    void on_btnUpdateControlInfo_clicked();

    void on_btnUnbind_clicked();

    void on_btnGetDynamicPwd_clicked();

    void on_btnMemberCreate_clicked();

    void on_btnMemberUpdate_clicked();

    void on_btnMemberDelete_clicked();

    void on_btnUpdateVerifyImg_clicked();

    void on_btnUpdateShowFace_clicked();

    void on_btnGroupCreate_clicked();

    void on_btnGroupUpdate_clicked();

    void on_btnBindGroup_clicked();

    void on_btnUnbindGroup_clicked();

    void on_btnBinding_clicked();

    void on_btnConfiguration_clicked();

private:
    Ui::MainWindow *ui;

    QJsonSerializer *mSerializer;

    APPToken mAppToken;
    OrgInfo mOrgInfo;
    AccessToken mAccessToken;
    ActivationResult mActivationResult;
    QString mStrDeviceId;

public:
    static void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // MAINWINDOW_H
