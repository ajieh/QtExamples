#include "textwidget.h"

TextWidget::TextWidget(QWidget *parent) : QWidget(parent)
{
    textEdit = new QTextEdit(this);
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(textEdit);
    vlayout->setMargin(0);
}



