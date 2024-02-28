#include "logindialog.h".h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qssFile(":/style.qss");
    if(qssFile.open(QFile::ReadOnly))
    {
        a.setStyleSheet(qssFile.readAll());
    }
    qssFile.close();

    LoginDialog login;
    login.show();
    return a.exec();
}
