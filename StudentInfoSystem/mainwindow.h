
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "logindialog.h"
#include "addstudialog.h"
#include "addadmindialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class LoginDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow();

private:
    Ui::MainWindow *ui;
    const int STU_COLUMN_COUNT = 11;
    const int ADM_COLUMN_COUNT = 4;
    AddStuDialog*       m_addStuDialog;
    AddAdminDialog*     m_addAdminDialog;
    StudentSQL*         m_ptrStuSql;
    QString             currentUsername;
    QString             permissionLevel;

signals:
    void currentUserInformationChanged();

private slots:
    void onCurrentUserInformationChanged();
    void on_treeWidget_itemSelectionChanged();
    void on_queryBtn_clicked();
    void on_addBtn_clicked();
    void updateStudentTableWidget();
    void on_clearStuTableBtn_clicked();
    void on_deleteBtn_clicked();
    void on_modifyBtn_clicked();
    void on_selectAllCheckBox_stateChanged(int arg1);
    void on_queryAdminBtn_clicked();
    void on_deleteAdminBtn_clicked();
    void on_modifyAdminBtn_clicked();
    void updateAdminTableWidget();
    void on_exitBtn_clicked();
};
#endif // MAINWINDOW_H
