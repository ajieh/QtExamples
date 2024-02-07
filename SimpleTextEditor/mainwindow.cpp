#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("文本编辑器"));
    textWidget = new TextWidget(this);
    setCentralWidget(textWidget);

    fontLabel = new QLabel(tr("字体: "));
    fontComboBox = new QFontComboBox;
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    connect(fontComboBox, &QComboBox::currentTextChanged, this, &MainWindow::handle_fontComboBox_currentTextChanged);

    wordSizeLabel = new QLabel(tr("字号: "));
    fontSizeBox = new QComboBox;
    connect(fontSizeBox, &QComboBox::currentTextChanged, this, &MainWindow::handle_fontSizeBox_currentTextChanged);

    QFontDatabase fontData;
    foreach(int size, fontData.standardSizes())
    {
        fontSizeBox->addItem(QString::number(size));
    }
    fontSizeBox->setCurrentIndex(4);

    boldBtn = new QToolButton;
    boldBtn->setIcon(QIcon(":/icon/font-bold.png"));
    boldBtn->setCheckable(true);
    connect(boldBtn, &QToolButton::clicked, this, &MainWindow::handle_boldBtn_clicked);

    italicBtn = new QToolButton;
    italicBtn->setIcon(QIcon(":/icon/font-Italics.png"));
    italicBtn->setCheckable(true);
    connect(italicBtn, &QToolButton::clicked, this, &MainWindow::handle_italicBtn_clicked);

    underlineBtn = new QToolButton;
    underlineBtn->setIcon(QIcon(":/icon/font_underline.png"));
    underlineBtn->setCheckable(true);
    connect(underlineBtn, &QToolButton::clicked, this, &MainWindow::handle_underlineBtn_clicked);

    colorBtn = new QToolButton;
    colorBtn->setIcon(QIcon(":/icon/font_color.png"));
    colorBtn->setCheckable(true);
    connect(colorBtn, &QToolButton::clicked, this, &MainWindow::handle_colorBtn_clicked);
    connect(textWidget->textEdit, &QTextEdit::currentCharFormatChanged, this, &MainWindow::handle_textEdit_currentFormatChanged);

    listLabel = new QLabel(tr("排序: "),this);
    listCombobox = new QComboBox;
    listCombobox->addItem("Standard");
    listCombobox->addItem("ListDisc");
    listCombobox->addItem("ListCircle");
    listCombobox->addItem("ListSquare");
    listCombobox->addItem("ListDecimal");
    listCombobox->addItem("ListLowerAlpha");
    listCombobox->addItem("ListUpperAlpha");
    listCombobox->addItem("ListLowerRoman");
    listCombobox->addItem("ListUpperRoman");
    connect(listCombobox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::handle_listComboBox_activated);
    connect(textWidget->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::handle_textEdit_cursorPositionChanged);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
}

//创建actions
void MainWindow::createActions()
{
    openAction = new QAction(tr("打开(O)"), this);
    openAction->setIcon(QIcon(":/icon/open.png"));
    openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    connect(openAction, &QAction::triggered, this, &MainWindow::handle_openAction_triggered);

    saveAction = new QAction(tr("保存(S)"),this);
    saveAction->setIcon(QIcon(":/icon/save.png"));
    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(saveAction, &QAction::triggered, this, &MainWindow::handle_saveAction_triggered);

    exitAction = new QAction(tr("退出(Q)"), this);
    exitAction->setIcon(QIcon(":/icon/quit.png"));
    exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    newAction = new QAction(tr("新建(N)"), this);
    newAction->setIcon(QIcon(":/icon/new.png"));
    newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(newAction, &QAction::triggered, this, &MainWindow::handle_newAction_triggered);

    readOnlyAction = new QAction("只读", this);
    readOnlyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    readOnlyAction->setCheckable(true);
    readOnlyAction->setChecked(false);
    connect(readOnlyAction, &QAction::triggered, this, &MainWindow::toggleReadOnlyMode);

    copyAction = new QAction(tr("复制(C)"), this);
    copyAction->setIcon(QIcon(":/icon/copy.png"));
    copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(copyAction, &QAction::triggered, textWidget->textEdit, &QTextEdit::copy);

    cutAction = new QAction(tr("剪切(X)"), this);
    cutAction->setIcon(QIcon(":/icon/cut.png"));
    cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    connect(cutAction, &QAction::triggered, textWidget->textEdit, &QTextEdit::cut);

    pasteAction = new QAction(tr("粘贴(P)"), this);
    pasteAction->setIcon(QIcon(":/icon/paste.png"));
    pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    connect(pasteAction, &QAction::triggered, textWidget->textEdit, &QTextEdit::paste);

    aboutAction = new QAction(tr("关于(A)"), this);
    aboutAction->setIcon(QIcon(":/icon/about.png"));
    aboutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::handle_aboutAction_triggered);

    printAction = new QAction(tr("打印(P)"), this);
    printAction->setIcon(QIcon(":/icon/printer.png"));
    printAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    connect(printAction, &QAction::triggered, this, &MainWindow::handle_printAction_triggered);

    undoAction = new QAction(tr("撤销(Z)"),this);
    undoAction->setIcon(QIcon(":/icon/undo.png"));
    undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    connect(undoAction, &QAction::triggered, textWidget->textEdit, &QTextEdit::undo);

    redoAction = new QAction(tr("重做(Y)"), this);
    redoAction->setIcon(QIcon(":/icon/redo.png"));
    redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    connect(redoAction, &QAction::triggered, textWidget->textEdit, &QTextEdit::redo);

    actGroup = new QActionGroup(this);
    leftAction = new QAction(tr("左对齐"), actGroup);
    leftAction->setIcon(QIcon(":/icon/left.png"));
    leftAction->setCheckable(true);

    rightAction = new QAction(tr("右对齐"), actGroup);
    rightAction->setIcon(QIcon(":/icon/right.png"));
    rightAction->setCheckable(true);

    centerAction = new QAction(tr("居中"), actGroup);
    centerAction->setIcon(QIcon(":/icon/center.png"));
    centerAction->setCheckable(true);

    justifyAction = new QAction(tr("两端对齐"), actGroup);
    justifyAction->setIcon(QIcon(":/icon/justify.png"));
    justifyAction->setCheckable(true);
    connect(actGroup, &QActionGroup::triggered, this, &MainWindow::handle_alignmentActionGroup_triggered);
}

//创建菜单
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(newAction);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->addAction(readOnlyAction);
    editMenu->addSeparator();
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(cutAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(aboutAction);
}

//创建工具栏
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar("File");
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(printAction);
    fileToolBar->addAction(exitAction);
    fileToolBar->setMovable(false);

    editToolBar = addToolBar("Edit");
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->setMovable(false);

    fontToolBar = addToolBar("Font");
    fontToolBar->addWidget(fontLabel);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(wordSizeLabel);
    fontToolBar->addWidget(fontSizeBox);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addWidget(colorBtn);
    fontToolBar->setMovable(false);

    listToolBar = addToolBar("List");
    listToolBar->addWidget(listLabel);
    listToolBar->addWidget(listCombobox);
    listToolBar->addSeparator();
    listToolBar->addActions(actGroup->actions());
    listToolBar->setMovable(false);
}

//创建状态栏
void MainWindow::createStatusBar()
{
    rowsColsLabel = new QLabel(this);
    rowsColsLabel->setFixedSize(300, 40);
    rowsColsLabel->setText(tr("行: ") + tr("  列: "));

    modeLabel = new QLabel(this);
    modeLabel->setFixedSize(300, 40);
    modeLabel->setText(tr("编辑模式"));

    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->addWidget(rowsColsLabel);
    statusBar->addWidget(modeLabel);
    connect(textWidget->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);
}

//更新状态栏
void MainWindow::updateStatusBar()
{
    int line = textWidget->textEdit->textCursor().blockNumber() + 1;
    int column = textWidget->textEdit->textCursor().columnNumber() + 1;

    rowsColsLabel->setText(tr("行: ") + QString::number(line) + tr("  列: ")  + QString::number(column));
}

//打开文件
void MainWindow::handle_openAction_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("选择文件"));
    if(!fileName.isEmpty())
    {
        if(textWidget->textEdit->document()->isEmpty())
        {
            loadText(fileName);
        }
    }
}

//导入文本
void MainWindow::loadText(QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        while(!stream.atEnd())
        {
            textWidget->textEdit->append(stream.readLine());
        }
    }
}

//保存文件
void MainWindow::handle_saveAction_triggered()
{
    // 创建文件对话框，获取保存的文件名和类型
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文本"), ".", tr("文本文件(*.txt)"));
    if (fileName.isEmpty()) {
        return;
    }

    // 打开文件，写入文本
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("错误"), tr("无法保存文件"));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << textWidget->textEdit->toPlainText();
    file.close();

    //显示保存成功提示
    QMessageBox::information(this, tr("提示"), tr("文件保存成功"));
}

//新建文件
void MainWindow::handle_newAction_triggered()
{
    MainWindow* newFile = new MainWindow(this);
    newFile->show();
}

//打印文件
void MainWindow::handle_printAction_triggered()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec())
    {
        QTextDocument* doc = textWidget->textEdit->document();
        doc->print(&printer);
    }
}

//关于
void MainWindow::handle_aboutAction_triggered()
{
    QString aboutText = "<h2>About TextEditor</h2>"
                            "<p>This is a simple text editor designed with Qt.</p>"
                            "<p>Version: 1.0.0</p>"
                            "<p>Developed by: zhao weijie</p>"
                            "<p>&copy; 2024 All rights reserved.</p>";
     QMessageBox::about(this, tr("关于"), aboutText);
}

//只读模式
void MainWindow::toggleReadOnlyMode()
{
    bool readOnly = readOnlyAction->isChecked();
    textWidget->textEdit->setReadOnly(readOnly);
    if (readOnly)
    {
        modeLabel->setText(tr("只读模式"));
    }
    else
    {
        modeLabel->setText(tr("编辑模式"));
    }
}

//设置字体
void MainWindow::handle_fontComboBox_currentTextChanged(const QString& fontStr)
{
    QTextCharFormat format;
    format.setFontFamily(fontStr);
    mergeFontFormat(format);
}

//合并字体格式
void MainWindow::mergeFontFormat(QTextCharFormat format)
{
    QTextCursor cursor = textWidget->textEdit->textCursor();
    //如果cursor当前有选定的文本内容，那么将当前光标所在的单词选中
    if(cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    //传入的字体格式format与光标所在位置的文本格式进行合并。
    cursor.mergeCharFormat(format);
    //将传入的字体格式format应用到当前文本编辑器中的光标位置的文本格式上
    textWidget->textEdit->mergeCurrentCharFormat(format);
}


//设置字号
void MainWindow::handle_fontSizeBox_currentTextChanged(const QString& spinValue)
{
    QTextCharFormat format;
    format.setFontPointSize(spinValue.toInt());
    textWidget->textEdit->mergeCurrentCharFormat(format);
}

//加粗
void MainWindow::handle_boldBtn_clicked()
{
    QTextCharFormat format;
    format.setFontWeight(boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
    textWidget->textEdit->mergeCurrentCharFormat(format);
}

//倾斜
void MainWindow::handle_italicBtn_clicked()
{
    QTextCharFormat format;
    format.setFontItalic(italicBtn->isChecked());
    textWidget->textEdit->mergeCurrentCharFormat(format);
}

//下划线
void MainWindow::handle_underlineBtn_clicked()
{
    QTextCharFormat format;
    format.setFontUnderline(underlineBtn->isChecked());
    textWidget->textEdit->mergeCurrentCharFormat(format);
}

//字体颜色
void MainWindow::handle_colorBtn_clicked()
{
    QColor fontColor = QColorDialog::getColor(Qt::red,this);
    if(fontColor.isValid())
    {
        QTextCharFormat format;
        format.setForeground(fontColor);
        textWidget->textEdit->mergeCurrentCharFormat(format);
    }
}


//光标所在处的字符格式发生变化时调用此函数
void MainWindow::handle_textEdit_currentFormatChanged(const QTextCharFormat& fmt)
{

    fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
    fontSizeBox->setCurrentIndex(fontSizeBox->findText(QString::number(fmt.fontPointSize())));
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.font().italic());
    underlineBtn->setChecked(fmt.font().underline());
}

//段落对齐
void MainWindow::handle_alignmentActionGroup_triggered(QAction* act)
{
    if(act == leftAction)
    {
        textWidget->textEdit->setAlignment(Qt::AlignLeft);
    }
    else if(act == rightAction)
    {
        textWidget->textEdit->setAlignment(Qt::AlignRight);
    }
    else if(act == centerAction)
    {
        textWidget->textEdit->setAlignment(Qt::AlignCenter);
    }
    else if(act == justifyAction)
    {
        textWidget->textEdit->setAlignment(Qt::AlignJustify);
    }
}


void MainWindow::handle_textEdit_cursorPositionChanged()
{
    if(textWidget->textEdit->alignment() == Qt::AlignLeft)
    {
        leftAction->setChecked(true);
    }
    else if(textWidget->textEdit->alignment() == Qt::AlignRight)
    {
        rightAction->setChecked(true);
    }
    else if(textWidget->textEdit->alignment() == Qt::AlignCenter)
    {
        centerAction->setChecked(true);
    }
    else if(textWidget->textEdit->alignment() == Qt::AlignJustify)
    {
        justifyAction->setChecked(true);
    }
}

//文本排序
void MainWindow::handle_listComboBox_activated(const int& index)
{
    QTextCursor cursor = textWidget->textEdit->textCursor();
    if(index)
    {
        QTextListFormat::Style style = QTextListFormat::ListDisc;
        switch (index)
        {
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case 7:
                style = QTextListFormat::ListLowerRoman;
                break;
            case 8:
                style = QTextListFormat::ListUpperRoman;
                break;
        }
        //设置缩进值
        //开始编辑块，记录编辑操作
        cursor.beginEditBlock();
        QTextBlockFormat blockFormat = cursor.blockFormat();
        QTextListFormat listFormat;
        //判断当前光标所在文本块是否属于列表
        if(cursor.currentList())
        {
            listFormat = cursor.currentList()->format();
        }
        else
        {
            //如果不是列表，则设置缩进和格式
            listFormat.setIndent(blockFormat.indent() + 1);
            blockFormat.setIndent(0);
            cursor.setBlockFormat(blockFormat);
        }
        listFormat.setStyle(style);
        cursor.createList(listFormat);
        //结束编辑块，应用之前记录的编辑操作
        cursor.endEditBlock();
    }
    else
    {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

