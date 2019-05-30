#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    mResults["id"] = ui->edt1->text();
    mResults["name"] = ui->edt2->text();
    mResults["mobile"] = ui->edt3->text();
    mResults["picture"] = ui->edt4->text();
    mResults["avatar"] = ui->edtAvata->text();
    mResults["idNumber"] = ui->edtIdCard->text();

    setResult(QDialog::Accepted);
    this->accept();
}

void Dialog::on_buttonBox_rejected()
{
    setResult(QDialog::Rejected);
    this->reject();
}


QVariantMap Dialog::results() const
{
    return mResults;
}

void Dialog::on_toolButton_clicked()
{
    const QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                u8"Ñ¡ÔñÍ¼Æ¬",
                                "",
                                tr("Images (*.png *.jpg)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty())
    {
        ui->edt4->setText(fileName);
    }

}

void Dialog::on_toolButton_2_clicked()
{
    const QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                u8"Ñ¡ÔñÍ¼Æ¬",
                                "",
                                tr("Images (*.png *.jpg)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty())
    {
        ui->edtAvata->setText(fileName);
    }
}
