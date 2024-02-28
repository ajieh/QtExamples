#pragma execution_character_set("utf-8")
#ifndef ADDADMINDIALOG_H
#define ADDADMINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "studentsql.h"
namespace Ui {
class AddAdminDialog;
}

class AddAdminDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAdminDialog(QWidget *parent = nullptr);
    ~AddAdminDialog();
    void setType(bool type, AdminInfo info = {});


private slots:
    void on_saveAdminBtn_clicked();
    void on_cancelAdminBtn_clicked();

private:
    Ui::AddAdminDialog *ui;
    bool m_isAdd;
    quint8 m_id;

};

#endif // ADDADMINDIALOG_H
