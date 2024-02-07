#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFont font("Microsoft YaHei",10);
    w.setFont(font);
    w.show();
    return a.exec();
}
