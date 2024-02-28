#include "addadmindialog.h"
#include "ui_addadmindialog.h"

AddAdminDialog::AddAdminDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAdminDialog)
{
    ui->setupUi(this);
}

AddAdminDialog::~AddAdminDialog()
{
    delete ui;
}

void AddAdminDialog::setType(bool type, AdminInfo info)
{
    m_isAdd = type;
    m_id = info.a_adminId;
    ui->usernameLineEdit->setText(info.a_username);
    ui->adminPasswordLineEdit->setText(info.a_password);
    ui->permissionsComboBox->setCurrentText(info.a_permissions);
}

//保存
void AddAdminDialog::on_saveAdminBtn_clicked()
{
    auto admin = StudentSQL::getInstance();
    AdminInfo info;
    info.a_adminId = m_id;
    info.a_username = ui->usernameLineEdit->text();
    info.a_password = ui->adminPasswordLineEdit->text();
    info.a_permissions = ui->permissionsComboBox->currentText();
    if(m_isAdd)
    {
        if(admin->addAdministrator(info))
        {
           QMessageBox::information(this, "信息", "保存成功");
        }
        else
        {
           QMessageBox::warning(this, "警告", "用户名已存在");
        }

    }
    else
    {
        admin->updateAdiminInfo(info);
        QMessageBox::information(this, "信息", "更新成功");
    }
    this->hide();
}


void AddAdminDialog::on_cancelAdminBtn_clicked()
{
    this->hide();
}

