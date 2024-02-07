#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QTextEdit>

class TextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextWidget(QWidget *parent = nullptr);
    QTextEdit*      textEdit;
};

#endif // TEXTWIDGET_H
