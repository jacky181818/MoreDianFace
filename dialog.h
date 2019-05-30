#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QVariantMap results() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:


private:
    Ui::Dialog *ui;
    QVariantMap mResults;
};

#endif // DIALOG_H
