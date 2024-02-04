#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    setDialogInterface();
    connect(modalButton, &QPushButton::clicked, this, &Widget::handle_modalButton_clicked);
    connect(modelessButton, &QPushButton::clicked, this, &Widget::handle_modelessButton_clicked);
    connect(halfModalButton, &QPushButton::clicked, this, &Widget::handle_halfModalButton_clicked);
    connect(progressButton, &QPushButton::clicked, this, &Widget::handle_progressButton_clicked);
    connect(warningButton, &QPushButton::clicked, this, &Widget::handle_warningButton_clicked);
    connect(criticalButton, &QPushButton::clicked, this, &Widget::handle_criticalButton_clicked);
    connect(infoButton, &QPushButton::clicked, this, &Widget::handle_infoButton_clicked);
    connect(questionButton, &QPushButton::clicked, this, &Widget::handle_questionButton_clicked);
    connect(aboutButton, &QPushButton::clicked, this, &Widget::handle_aboutButton_clicked);
    connect(inputButton, &QPushButton::clicked, this, &Widget::handle_inputButton_clicked);
    connect(colorButton, &QPushButton::clicked, this, &Widget::handle_colorButton_clicked);
    connect(fontButton, &QPushButton::clicked, this, &Widget::handle_fontButton_clicked);
    connect(fileButton, &QPushButton::clicked, this, &Widget::handle_fileButton_clicked);
}

void Widget::setDialogInterface()
{
    dockWidget = new QDockWidget(this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);

    QWidget* contentWidget = new QWidget(dockWidget);
    dockWidget->setWidget(contentWidget);

    modalButton = new QPushButton("模态对话框", dockWidget);
    modelessButton = new QPushButton("非模态对话框", dockWidget);
    halfModalButton = new QPushButton("半模态对话框", dockWidget);
    progressButton = new QPushButton("进度条对话框", dockWidget);
    warningButton = new QPushButton("警告对话框", dockWidget);
    infoButton = new QPushButton("信息对话框", dockWidget);
    questionButton = new QPushButton("问题对话框", dockWidget);
    criticalButton = new QPushButton("错误对话框", dockWidget);
    aboutButton = new QPushButton("关于对话框", dockWidget);

    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(modalButton);
    buttonLayout->addWidget(modelessButton);
    buttonLayout->addWidget(halfModalButton);
    buttonLayout->addWidget(progressButton);
    buttonLayout->addWidget(warningButton);
    buttonLayout->addWidget(criticalButton);
    buttonLayout->addWidget(infoButton);
    buttonLayout->addWidget(questionButton);
    buttonLayout->addWidget(aboutButton);
    contentWidget->setLayout(buttonLayout);

    inputButton = new QPushButton("输入对话框", this);
    inputButton->setGeometry(200, 70, 200, 40);
    numberLine = new QLineEdit(this);
    numberLine->setGeometry(450, 70, 250, 40);

    colorButton = new QPushButton("颜色对话框", this);
    colorButton->setGeometry(200,150, 200, 40);
    colorLabel = new QLabel(this);
    colorLabel->setGeometry(450, 150, 250, 40);
    colorLabel->setStyleSheet("background-color: white;");

    fontButton = new QPushButton("字体对话框", this);
    fontButton->setGeometry(200, 230, 200, 40);
    fontLabel = new QLabel("字体",this);
    fontLabel->setGeometry(450, 230, 250, 80);
    fontLabel->setAlignment(Qt::AlignCenter);
    fontLabel->setText("字体测试");

    fileButton = new QPushButton("文件对话框", this);
    fileButton->setGeometry(200, 350, 200, 40);
    filePathEdit = new QTextEdit(this);
    filePathEdit->setGeometry(450, 350, 250,200);


}

//模态对话框
void Widget::handle_modalButton_clicked()
{
    QDialog dlg(this);
    dlg.resize(400,200);
    dlg.setWindowTitle("模态对话框");

    QVBoxLayout* layout = new QVBoxLayout(&dlg);
    QLabel* label = new QLabel("这是一个模态对话框", &dlg);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    dlg.exec();
}

//非模态对话框
void Widget::handle_modelessButton_clicked()
{
    QDialog *dlg = new QDialog(this);//必须放在堆上，不然会一闪而过
    dlg->resize(400,200);
    dlg->setAttribute(Qt::WA_DeleteOnClose);//在对话框关闭后自动释放对话框对象的内存。
    dlg->setWindowTitle("非模态对话框");

    QVBoxLayout* layout = new QVBoxLayout(dlg);
    QLabel* label = new QLabel("这是一个非模态对话框", dlg);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    dlg->show();
}

//半模态对话框
void Widget::handle_halfModalButton_clicked()
{
    QDialog *dlg = new QDialog(this);
    dlg->resize(400,200);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowTitle("半模态对话框");

    QVBoxLayout* layout = new QVBoxLayout(dlg);
    QLabel* label = new QLabel("这是一个半模态对话框", dlg);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    dlg->setModal(true);
    dlg->show();
}

//进度条对话框
void Widget::handle_progressButton_clicked()
{
    QProgressDialog progressDialog("进度对话框示例", "取消", 0, 50000, this);
    progressDialog.setWindowTitle("请等待");
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(5);
    progressDialog.show();
    for(int i = 0; i < 50000; i++)
    {
        progressDialog.setValue(i);
        if(progressDialog.wasCanceled())
        {
            return;
        }
    }
}

//输入对话框
void Widget::handle_inputButton_clicked()
{
    bool ok;
    int valueInt = QInputDialog::getInt(this, "输入对话框", "请输入一个整数", 0, INT32_MIN, INT32_MAX, 1, &ok);
    if(ok)
    {
        numberLine->setText(QString::number(valueInt));
    }
}

//警告对话框
void Widget::handle_warningButton_clicked()
{
    QMessageBox::warning(this,"警告", "这是一个警告！");
}

//错误对话框
void Widget::handle_criticalButton_clicked()
{
    QMessageBox::critical(this, "错误", "发生了严重错误！");
}

//信息对话框
void Widget::handle_infoButton_clicked()
{
    QMessageBox::information(this, "信息", "这是信息提示对话框");
}

//问题对话框
void Widget::handle_questionButton_clicked()
{
    QMessageBox::question(this, "保存", "文件未保存，是否保存文件？",
                          QMessageBox::Save | QMessageBox::Cancel,
                          QMessageBox::Save);
}

//关于对话框
void Widget::handle_aboutButton_clicked()
{
    QMessageBox::about(this, "关于", "这是一个关于消息框示例");
}

//颜色对话框
void Widget::handle_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor(255,0,0));    
    if(color.isValid())
    {
        QString styleSheet = QString("QLabel { background-color: %1; }").arg(color.name());
        colorLabel->setStyleSheet(styleSheet);
    }

}

//字体对话框
void Widget::handle_fontButton_clicked()
{
    bool flag;
    QFont font = QFontDialog::getFont(&flag,QFont("宋体",12));
    if(flag)
    {
        fontLabel->setFont(font);
    }
}

//文件对话框
void Widget::handle_fileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"文件打开按钮","D:/");
    filePathEdit->setText(path);
}

Widget::~Widget()
{
    delete ui;
}

