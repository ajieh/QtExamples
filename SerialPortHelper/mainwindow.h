#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort*    mSerialPort;
    bool            bOpenUSARTFlag;
    bool            bReceiveFlag;
    int             mTimerInterval;
    QTimer*         mTimer;

private:
    void scanSerialPort();
    void displayHex();
    void displayText();

private slots:
    void readSerialData();
    void on_openUSARTBtn_clicked(bool checked);
    void on_closeUSARTBtn_clicked(bool checked);
    void on_sendFileBtn_clicked();
    void on_autoSendCheckBox_stateChanged(int arg1);
    void on_sendingCycleLine_textChanged(const QString &arg1);
    void on_saveDataBtn_clicked();
    void on_openFileBtn_clicked();
    void on_pauseBtn_clicked();
    void on_hexDisplayCheckBox_toggled(bool checked);
    void on_hexSendCheckBox_toggled(bool checked);
};
#endif // MAINWINDOW_H
