#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QTextStream>
#include <QTextCharFormat>
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QTextList>
#include "textwidget.h"
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TextWidget*     textWidget;
    QString         fileName;
    QMenu*          fileMenu;
    QMenu*          editMenu;
    QMenu*          helpMenu;
    QAction*        openAction;
    QAction*        saveAction;
    QAction*        exitAction;
    QAction*        newAction;
    QAction*        copyAction;
    QAction*        cutAction;
    QAction*        pasteAction;
    QAction*        aboutAction;
    QAction*        printAction;
    QAction*        undoAction;
    QAction*        redoAction;
    QAction*        readOnlyAction;
    QToolBar*       fileToolBar;
    QToolBar*       editToolBar;
    QToolBar*       fontToolBar;
    QLabel*         fontLabel;
    QLabel*         wordSizeLabel;
    QLabel*         listLabel;
    QLabel*         rowsColsLabel;
    QLabel*         modeLabel;
    QComboBox*      fontSizeBox;
    QComboBox*      listCombobox;
    QFontComboBox*  fontComboBox;
    QToolButton*    boldBtn;
    QToolButton*    italicBtn;
    QToolButton*    underlineBtn;
    QToolButton*    colorBtn;
    QActionGroup*   actGroup;
    QAction*        leftAction;
    QAction*        rightAction;
    QAction*        centerAction;
    QAction*        justifyAction;
    QToolBar*       listToolBar;

private:
    void createMenus();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void updateStatusBar();
    void loadText(QString filename);
    void mergeFontFormat(QTextCharFormat format);

private slots:
    void toggleReadOnlyMode();
    void handle_openAction_triggered();
    void handle_saveAction_triggered();
    void handle_newAction_triggered();
    void handle_printAction_triggered();
    void handle_aboutAction_triggered();
    void handle_fontComboBox_currentTextChanged(const QString& fontStr);
    void handle_fontSizeBox_currentTextChanged(const QString& spinValue);
    void handle_boldBtn_clicked();
    void handle_italicBtn_clicked();
    void handle_underlineBtn_clicked();
    void handle_colorBtn_clicked();
    void handle_textEdit_currentFormatChanged(const QTextCharFormat& fmt);
    void handle_alignmentActionGroup_triggered(QAction* act);
    void handle_textEdit_cursorPositionChanged();
    void handle_listComboBox_activated(const int& index);
};
#endif // MAINWINDOW_H
