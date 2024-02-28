#include "logindialog.h"
#include "ui_logindialog.h"
QString LoginDialog::curUsername = "";
//获取当前用户用户名
QString LoginDialog::getCurrentUserName()
{
    return curUsername;
}

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    ,  ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    m_ptrStuSQL = StudentSQL::getInstance();
    if(m_ptrStuSQL)
    {
        m_ptrStuSQL->connectToDatabase();
    }
    mainwindow = new MainWindow(this);
    connect(this, &LoginDialog::sendLoginSuccess, mainwindow, &MainWindow::initMainWindow);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}



void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    this->dialogPos = this->pos();       // 获得部件当前位置
    this->mousePos = event->globalPos(); // 获得鼠标位置
    this->newPos = mousePos - dialogPos;   // 移动后部件所在的位置
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->newPos);
}

//登录
void LoginDialog::on_loginBtn_clicked()
{
    QString username = ui->userLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    bool loginSuccess = m_ptrStuSQL->performLoginValidation(username, password);
    if (loginSuccess)
    {
        curUsername = ui->userLineEdit->text();
        emit sendLoginSuccess(); // 发出登录成功信号
        this->hide(); // 隐藏登录对话框
    }
    else
    {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重试");
    }
}

//退出
void LoginDialog::on_quitBtn_clicked()
{
    m_ptrStuSQL->disconnectFromDatabase();
    QApplication::quit();
}

//注册账号
void LoginDialog::on_registerBtn_clicked()
{
    m_adminDialog = new AddAdminDialog(this);
    m_adminDialog->exec();
}



void LoginDialog::on_showPasswordBtn_clicked()
{
    ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

