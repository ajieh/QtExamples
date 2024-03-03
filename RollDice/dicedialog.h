#pragma execution_character_set("utf-8")
#ifndef DICEDIALOG_H
#define DICEDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "dicethread.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class DiceDialog;
}
QT_END_NAMESPACE

class DiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiceDialog(QWidget *parent = nullptr);
    ~DiceDialog();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::DiceDialog *ui;
    DiceThread*     m_diceThread;
    QTimer*         m_timer;
    quint8          m_index;
    quint8          m_pointValue;
    void showDicePicture(const quint8& value);

private slots:
    void handleThreadStarted();
    void handleThreadFinished();
    void handleTimerOut();
    void on_startThreadBtn_clicked();
    void on_finishThreadBtn_clicked();
    void on_beginRollDiceBtn_clicked();
    void on_pauseBtn_clicked();
    void on_clearBtn_clicked();

};
#endif // DICEDIALOG_H
