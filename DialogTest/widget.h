﻿#ifndef WIDGET_H
#define WIDGET_H
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextEdit>
#include <QInputDialog>
#include <QProgressDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;    
    QDockWidget*    dockWidget;
    QPushButton*    modalButton;
    QPushButton*    modelessButton;
    QPushButton*    halfModalButton;
    QPushButton*    progressButton;
    QPushButton*    warningButton;
    QPushButton*    infoButton;
    QPushButton*    questionButton;
    QPushButton*    criticalButton;
    QPushButton*    aboutButton;
    QPushButton*    inputButton;
    QPushButton*    colorButton;
    QPushButton*    fontButton;
    QPushButton*    fileButton;
    QLineEdit*      numberLine;
    QLabel*         fontLabel;
    QLabel*         colorLabel;
    QTextEdit*      filePathEdit;
    void setDialogInterface();

private slots:
    void handle_modalButton_clicked();
    void handle_modelessButton_clicked();
    void handle_halfModalButton_clicked();
    void handle_progressButton_clicked();
    void handle_warningButton_clicked();
    void handle_criticalButton_clicked();
    void handle_infoButton_clicked();
    void handle_questionButton_clicked();
    void handle_aboutButton_clicked();
    void handle_inputButton_clicked();
    void handle_colorButton_clicked();
    void handle_fontButton_clicked();
    void handle_fileButton_clicked();

};
#endif // WIDGET_H
