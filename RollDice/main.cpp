#include "dicedialog.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiceDialog w;
    QFile qssFile(":/beautify.qss");
    if(qssFile.open(QFile::ReadOnly))
    {
        a.setStyleSheet(qssFile.readAll());
    }
    qssFile.close();

    w.show();
    return a.exec();
}
