#include "dicedialog.h"
#include "ui_dicedialog.h"

DiceDialog::DiceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DiceDialog)
{
    ui->setupUi(this);
    setWindowTitle("掷骰子");
    m_diceThread = new DiceThread();
    m_timer = new QTimer(this);
    connect(m_diceThread, &DiceThread::started, this, &DiceDialog::handleThreadStarted);
    connect(m_diceThread, &DiceThread::finished, this, &DiceDialog::handleThreadFinished);
    connect(m_timer, &QTimer::timeout, this, &DiceDialog::handleTimerOut);
    showDicePicture(0);

}

DiceDialog::~DiceDialog()
{
    delete ui;
}

//dialog关闭事件
void DiceDialog::closeEvent(QCloseEvent *event)
{
    if(m_diceThread->isRunning())
    {
        m_diceThread->stopThread();
        m_diceThread->wait();
    }
    event->accept();
}

//显示骰子图片
void DiceDialog::showDicePicture(const quint8 &value)
{
    QPixmap pixmap;
    switch(value) {
    case 0:
        pixmap.load(":/dice/dice_init.png");
        break;
    case 1:
        pixmap.load(":/dice/dice_one.png");
        break;
    case 2:
        pixmap.load(":/dice/dice_two.png");
        break;
    case 3:
        pixmap.load(":/dice/dice_three.png");
        break;
    case 4:
        pixmap.load(":/dice/dice_four.png");
        break;
    case 5:
        pixmap.load(":/dice/dice_five.png");
        break;
    case 6:
        pixmap.load(":/dice/dice_six.png");
        break;
    default:
        break;
    }
    ui->displayDiceLabel->setPixmap(pixmap);
}

//线程启动的槽函数
void DiceDialog::handleThreadStarted()
{
    ui->threadStatusLabel->setText("状态：线程启动");
}

//线程结束的槽函数
void DiceDialog::handleThreadFinished()
{
    ui->threadStatusLabel->setText("状态：线程结束");
}

//时间溢出响应槽函数
void DiceDialog::handleTimerOut()
{
    quint8 tempIndex = 0;
    quint8 tempPoint = 0;
    if(m_diceThread->readPointValue(&tempIndex, &tempPoint) && (tempIndex != m_index))
    {
        m_index = tempIndex;
        m_pointValue = tempPoint;
        QString message = QString("第%1次投掷骰子，点数为:%2\n").arg(m_index).arg(m_pointValue);
        ui->resultTextEdit->appendPlainText(message);
        showDicePicture(m_pointValue);
    }
}


//开始线程
void DiceDialog::on_startThreadBtn_clicked()
{
    m_index = 0;
    m_diceThread->start();
    ui->startThreadBtn->setEnabled(false);
    ui->finishThreadBtn->setEnabled(true);
    ui->beginRollDiceBtn->setEnabled(true);
    ui->pauseBtn->setEnabled(false);
}

//结束线程
void DiceDialog::on_finishThreadBtn_clicked()
{
    m_diceThread->stopThread();
    m_diceThread->wait();
    ui->startThreadBtn->setEnabled(true);
    ui->finishThreadBtn->setEnabled(false);
    ui->beginRollDiceBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
}

//开始掷骰子
void DiceDialog::on_beginRollDiceBtn_clicked()
{
    m_diceThread->diceBegin();
    m_timer->start(200);
    ui->beginRollDiceBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(true);
}

//暂停
void DiceDialog::on_pauseBtn_clicked()
{
    m_diceThread->dicePause();
    m_timer->stop();
    ui->beginRollDiceBtn->setEnabled(true);
    ui->pauseBtn->setEnabled(false);
}

//清空
void DiceDialog::on_clearBtn_clicked()
{
    ui->resultTextEdit->clear();
    showDicePicture(0);
}





