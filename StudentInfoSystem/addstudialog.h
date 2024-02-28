#pragma execution_character_set("utf-8")
#ifndef ADDSTUDIALOG_H
#define ADDSTUDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QMap>
#include "studentsql.h"
namespace Ui {
class AddStuDialog;
}

class AddStuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStuDialog(QWidget *parent = nullptr);
    ~AddStuDialog();
    void setType(bool type, StudentInfo info = {});

private:
    Ui::AddStuDialog *ui;
    bool m_isAdd;
    //StudentInfo m_stuInfo;
    QMap<QString, QStringList> academySpecialityMap;
    QMap<QString, QStringList> specialityClassMap;
    void initialMap();

private slots:
    void on_saveBtn_clicked();
    void on_cancelBtn_clicked();
    void onAcademyChanged(const QString& academy);
    void onSpecialityChanged(const QString& specialty);
};

#endif // ADDSTUDIALOG_H
