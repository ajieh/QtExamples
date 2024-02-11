#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,4);
    setWindowTitle(tr("串口助手"));
    bOpenUSARTFlag = false;
    bReceiveFlag = true;//可以接收
    scanSerialPort();
    mTimerInterval = ui->sendingCycleLine->text().toInt();
    mTimer = new QTimer(this);
    mTimer->setInterval(mTimerInterval);// 设置默认定时时长1000ms
    connect(mTimer, &QTimer::timeout, this, [=](){
        on_sendFileBtn_clicked();
    });
    connect(ui->clearSendingBtn, &QPushButton::clicked, ui->sendTextEdit, &QPlainTextEdit::clear);
    connect(ui->clearReceiveBtm, &QPushButton::clicked, ui->receiveTextBrowser, &QTextBrowser::clear);
}

//扫描可用串口
void MainWindow::scanSerialPort()
{
    ui->portNumBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portNumBox->addItem(info.portName());
    }
}

//打开串口
void MainWindow::on_openUSARTBtn_clicked(bool checked)
{
    if(!bOpenUSARTFlag)
    {
        mSerialPort = new QSerialPort(this);
        //设置串口号
        mSerialPort->setPortName(ui->portNumBox->currentText());
        if(mSerialPort->open(QIODevice::ReadWrite))
        {
            //设置波特率
            mSerialPort->setBaudRate(ui->baudRateBox->currentText().toInt());
            //设置数据位
            mSerialPort->setDataBits(QSerialPort::Data8);
            //设置校验位
            switch (ui->parityBox->currentIndex())
            {
                case 0:
                    mSerialPort->setParity(QSerialPort::NoParity);
                    break;
                case 1:
                    mSerialPort->setParity(QSerialPort::OddParity);
                case 2:
                    mSerialPort->setParity(QSerialPort::EvenParity);
                default:
                    break;
            }
            //设置停止位
            switch (ui->stopBitsBox->currentIndex())
            {
                case 0:
                    mSerialPort->setStopBits(QSerialPort::OneStop);
                    break;
                case 1:
                    mSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
                    break;
                case 2:
                    mSerialPort->setStopBits(QSerialPort::TwoStop);
                    break;
                default:
                    break;
            }
            mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
            connect(mSerialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        }
        else
        {
            QMessageBox::information(this, "提示", "串口无法打开\r\n不存在或 被占用");
            return;
        }
        bOpenUSARTFlag = true;
        ui->ledLight->setStyleSheet("QLabel { background-color: green; border-radius: 10px;}");
    }
}

//读取串口数据并显示在接收区
void MainWindow::readSerialData()
{
    QByteArray readComData = mSerialPort->readAll();
    if (bReceiveFlag)
    {
        if (!readComData.isEmpty())
        {
            //是否显示时间
            if (ui->displayTimeCheckBox->checkState())
            {
                QString timeString = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] "); // 获取当前日期和时间，并将其转换为字符串
                ui->receiveTextBrowser->insertPlainText(timeString); // 追加接收时间
            }
            ui->receiveTextBrowser->insertPlainText(QString::fromLocal8Bit(readComData));
        }
        readComData.clear();
    }
}

//关闭串口
void MainWindow::on_closeUSARTBtn_clicked(bool checked)
{
    if(bOpenUSARTFlag)
    {
        mSerialPort->clear();
        mSerialPort->close();
        mSerialPort->deleteLater();
        bOpenUSARTFlag = false;
        ui->ledLight->setStyleSheet("QLabel { background-color: gray; border-radius: 10px; }");
    }
    if(mTimer->isActive())
    {
        mTimer->stop();
    }
}

//发送文件
void MainWindow::on_sendFileBtn_clicked()
{
    if(bOpenUSARTFlag)
    {
        if(mSerialPort->isOpen())
        {
            QString sendText = ui->sendTextEdit->toPlainText();
            QByteArray bytes = sendText.toLocal8Bit();
            mSerialPort->write(bytes);


        }
    }
}

//自动发送
void MainWindow::on_autoSendCheckBox_stateChanged(int arg1)
{
    if(ui->autoSendCheckBox->checkState())
    {
        mTimerInterval = ui->sendingCycleLine->text().toInt();
        if(mTimerInterval > 0)
        {
            mTimer->start(mTimerInterval);
        }
    }
    else
    {
        mTimer->stop();
    }
}

//自动发送周期
void MainWindow::on_sendingCycleLine_textChanged(const QString &arg1)
{
    mTimerInterval = ui->sendingCycleLine->text().toInt();
    if (mTimerInterval > 0 && mTimer->isActive()) {
        mTimer->setInterval(mTimerInterval);
    }
}

//保存数据
void MainWindow::on_saveDataBtn_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        "保存数据", QDir::currentPath(), "Text Files (*.txt)");
    QString data = ui->receiveTextBrowser->toPlainText();
    if (!data.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << data;
            file.close();
        }
    }
    else
    {
        QMessageBox::information(this, "提示", "没有数据");
        return;
    }

}

//打开文件
void MainWindow::on_openFileBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString data = in.readAll();
            ui->sendTextEdit->clear();
            ui->sendTextEdit->setPlainText(data);
            file.close();
        }
    }
}

//暂停/继续接收
void MainWindow::on_pauseBtn_clicked()
{
    bReceiveFlag = !bReceiveFlag;//切换接收状态
    if(bReceiveFlag)
    {
        ui->pauseBtn->setText(tr("暂停接收"));
    }
    else
    {
        ui->pauseBtn->setText(tr("继续接收"));
    }
}

//是否HEX显示
void MainWindow::on_hexDisplayCheckBox_toggled(bool checked)
{
    if(checked)
    {
        displayHex();
    }
    else
    {
        displayText();
    }
}

//hex显示
void MainWindow::displayHex()
{
    QString receiveData = ui->receiveTextBrowser->toPlainText();
    QString hexData = receiveData.toLocal8Bit().toHex(' ').toUpper();
    ui->receiveTextBrowser->setPlainText(hexData);
}

//文本显示
void MainWindow::displayText()
{
    QString receiveData = ui->receiveTextBrowser->toPlainText();
    QString textData = QString::fromLocal8Bit(receiveData.toLocal8Bit());
    ui->receiveTextBrowser->setPlainText(textData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_hexSendCheckBox_toggled(bool checked)
{
    //十六进制
    if(checked)
    {
        QString sendText = ui->sendTextEdit->toPlainText();
        QByteArray hexData = QByteArray::fromHex(sendText.toUtf8());
        QString hexString = hexData.toHex(' ').toUpper(); // 转换为大写，并以空格分隔
        ui->sendTextEdit->setPlainText(hexString);
    }
}

