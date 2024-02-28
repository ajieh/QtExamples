#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "studentsql.h"
#include "mainwindow.h"
#include "addadmindialog.h"
class MainWindow;
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    static QString getCurrentUserName();
    static QString curUsername;

private:
    Ui::LoginDialog *ui;
    MainWindow*         mainwindow;
    AddAdminDialog*     m_adminDialog;
    StudentSQL*         m_ptrStuSQL;

    //记录鼠标，窗口位置
    QPoint dialogPos;
    QPoint mousePos;
    QPoint newPos;

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void sendLoginSuccess();

private slots:
    void on_loginBtn_clicked();
    void on_quitBtn_clicked();
    void on_registerBtn_clicked();
    void on_showPasswordBtn_clicked();
};

#endif // LOGINDIALOG_H
