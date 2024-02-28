#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ptrStuSql(nullptr), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ptrStuSql = StudentSQL::getInstance();
    connect(ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &MainWindow::on_treeWidget_itemSelectionChanged);
    connect(ui->addBtn, &QPushButton::clicked, this, &MainWindow::updateStudentTableWidget);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::updateStudentTableWidget);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::updateStudentTableWidget);
    connect(ui->clearStuTableBtn, &QPushButton::clicked, this, &MainWindow::updateStudentTableWidget);
    connect(ui->deleteAdminBtn, &QPushButton::clicked, this, &MainWindow::updateAdminTableWidget);
    connect(ui->modifyAdminBtn, &QPushButton::clicked, this, &MainWindow::updateAdminTableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    QStringList list;
    list<< "学生信息管理系统";
    QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidget, list);
    ui->treeWidget->addTopLevelItem(top);
    list.clear();
    list<< "学生管理";
    QTreeWidgetItem* stuItem = new QTreeWidgetItem(top, list);
    list.clear();
    list<< "管理员管理";
    QTreeWidgetItem* administratorItem = new QTreeWidgetItem(top, list);
    top->addChild(stuItem);
    top->addChild(administratorItem);

    // 设置管理员表第一列宽度为固定值，而其他列自动伸展

    ui->adminTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //显示当前用户名
    currentUsername = LoginDialog::getCurrentUserName();
    ui->adminLabel->setText("当前用户：" + currentUsername);
    permissionLevel =  m_ptrStuSql->getCurrentPermission(currentUsername);

    //修改当前用户信息的时候，更新用户名显示和权限
    connect(this, &MainWindow::currentUserInformationChanged, this, [=](){
        currentUsername = m_ptrStuSql->getCurrentUsername();
        ui->adminLabel->setText("当前用户：" + currentUsername);
        permissionLevel = m_ptrStuSql->getCurrentPermission(currentUsername);
    });
    this->show();
    m_addStuDialog = new AddStuDialog(this);
    m_addAdminDialog = new AddAdminDialog(this);
}

 //修改当前用户信息时发送信号
void MainWindow::onCurrentUserInformationChanged()
{
    emit currentUserInformationChanged();
}

//切换页面
void MainWindow::on_treeWidget_itemSelectionChanged()
{
    QTreeWidgetItem *selectedItem = ui->treeWidget->currentItem();

    if (selectedItem) {
        QString itemName = selectedItem->text(0);

        if (itemName == "学生管理") {
            ui->stackedWidget->setCurrentIndex(0);
        }
        else if (itemName == "管理员管理")
        {
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
}

//查询学生
void MainWindow::on_queryBtn_clicked()
{
    QString infofilter = ui->searchLineEdit->text();

    if (infofilter.isEmpty())
    {
        QMessageBox::warning(this, "警告", "查询内容不能为空！");
        updateStudentTableWidget();
        return;
    }
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(STU_COLUMN_COUNT);
    QStringList strlist;
    strlist << "序号" << "学号" << "姓名" << "性别" << "手机号" << "入学时间" << "年龄" << "学院"
            << "专业" << "班级" << "GPA";
    ui->tableWidget->setHorizontalHeaderLabels(strlist);
    auto count = m_ptrStuSql->queryAllStudentsNum();
    QList<StudentInfo> list = m_ptrStuSql->queryStudentsInfoByPage(0,count);

    int index = 0;
    for(int i = 0; i < list.size(); i++)
    {
        if(!list[i].s_name.contains(infofilter) && !list[i].s_studentId.contains(infofilter))
        {
            continue;
        }

        ui->tableWidget->setItem(index,0,new QTableWidgetItem(QString::number(index + 1)));
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(list[i].s_studentId));
        ui->tableWidget->setItem(index,2,new QTableWidgetItem(list[i].s_name));
        ui->tableWidget->setItem(index,3,new QTableWidgetItem(list[i].s_gender));
        ui->tableWidget->setItem(index,4,new QTableWidgetItem(list[i].s_phone));
        ui->tableWidget->setItem(index,5,new QTableWidgetItem(list[i].s_year));
        ui->tableWidget->setItem(index,6,new QTableWidgetItem(QString::number(list[i].s_age)));
        ui->tableWidget->setItem(index,7,new QTableWidgetItem(list[i].s_academy));
        ui->tableWidget->setItem(index,8,new QTableWidgetItem(list[i].s_speciality));
        ui->tableWidget->setItem(index,9,new QTableWidgetItem(list[i].s_class));
        ui->tableWidget->setItem(index,10,new QTableWidgetItem(QString::number(list[i].s_gpa)));
        index++;
    }
    ui->tableWidget->setRowCount(index);
}

//增加学生
void MainWindow::on_addBtn_clicked()
{
    m_addStuDialog = new AddStuDialog(this);
    m_addStuDialog->setType(true);
    m_addStuDialog->exec();

}

//更新学生表中的数据
void MainWindow::updateStudentTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(STU_COLUMN_COUNT);
    QStringList strlist;
    strlist << "序号" << "学号" << "姓名" << "性别" << "手机号" << "入学时间" << "年龄" << "学院"
            << "专业" << "班级" << "GPA";
    ui->tableWidget->setHorizontalHeaderLabels(strlist);
    ui->tableWidget->verticalHeader()->setVisible(false);//不显示默认行号
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//只选中行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    auto count = m_ptrStuSql->queryAllStudentsNum();
    ui->statusbar->showMessage("学生总数: " + QString::number(count));

    QList<StudentInfo> list = m_ptrStuSql->queryStudentsInfoByPage(0,count);
    ui->tableWidget->setRowCount(count);
    for(int i = 0; i < list.size(); i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(list[i].s_serialNum)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(list[i].s_studentId));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(list[i].s_name));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(list[i].s_gender));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(list[i].s_phone));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(list[i].s_year));
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(list[i].s_age)));
        ui->tableWidget->setItem(i,7,new QTableWidgetItem(list[i].s_academy));
        ui->tableWidget->setItem(i,8,new QTableWidgetItem(list[i].s_speciality));
        ui->tableWidget->setItem(i,9,new QTableWidgetItem(list[i].s_class));
        ui->tableWidget->setItem(i,10,new QTableWidgetItem(QString::number(list[i].s_gpa)));
    }
}

//修改学生信息
void MainWindow::on_modifyBtn_clicked()
{
    StudentInfo info;
    int rows = ui->tableWidget->currentRow();
    if(rows >= 0)
    {
        info.s_serialNum = ui->tableWidget->item(rows, 0)->text().toUInt();
        info.s_studentId = ui->tableWidget->item(rows, 1)->text();
        info.s_name = ui->tableWidget->item(rows, 2)->text();
        info.s_gender = ui->tableWidget->item(rows, 3)->text();
        info.s_age = ui->tableWidget->item(rows, 4)->text().toUInt();
        info.s_phone = ui->tableWidget->item(rows, 5)->text();
        info.s_year = ui->tableWidget->item(rows, 6)->text();
        info.s_academy = ui->tableWidget->item(rows, 7)->text();
        info.s_speciality = ui->tableWidget->item(rows, 8)->text();
        info.s_class = ui->tableWidget->item(rows, 9)->text();
        info.s_gpa = ui->tableWidget->item(rows, 10)->text().toDouble();
        m_addStuDialog->setType(false, info);
        m_addStuDialog->exec();
    }
}


//删除学生
void MainWindow::on_deleteBtn_clicked()
{
    int rows = ui->tableWidget->currentRow();
    if(rows >= 0)
    {
        QString id = ui->tableWidget->item(rows, 1)->text();
        m_ptrStuSql->deleteStudent(id);
    }

}

//清空学生表
void MainWindow::on_clearStuTableBtn_clicked()
{
    QMessageBox msgBox(QMessageBox::Question, "确认", "您确定要清空学生表吗?", QMessageBox::Yes|QMessageBox::No, this);
    msgBox.setButtonText(QMessageBox::Yes, "是");
    msgBox.setButtonText(QMessageBox::No, "否");
    int reply = msgBox.exec();
    if (reply == QMessageBox::Yes) {
        m_ptrStuSql->clearStudentTable();
    }
}

//全选
void MainWindow::on_selectAllCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        // 选中所有行
        ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        for (int row = 0; row < ui->tableWidget->rowCount(); row++)
        {
            ui->tableWidget->selectRow(row);
        }
    }
    else
    {
        // 取消选中所有行
        ui->tableWidget->clearSelection();
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

//查询用户(通过用户名)
void MainWindow::on_queryAdminBtn_clicked()
{
    QString namefilter = ui->searchAdminLineEdit->text();
    if(namefilter.isEmpty())
    {
        QMessageBox::warning(this, "警告", "查询内容不能为空！");
        updateAdminTableWidget();
        return;
    }
    ui->adminTable->clear();
    ui->adminTable->setColumnCount(ADM_COLUMN_COUNT);
    QStringList strlist;
    strlist << "序号" << "用户名" << "密码" << "权限";
    ui->adminTable->setHorizontalHeaderLabels(strlist);
    ui->adminTable->setRowCount(1);
    QList<AdminInfo> list = m_ptrStuSql->queryAllAdministratorsInfo();
    for(int i = 0; i < list.size(); i++)
    {
        if(list[i].a_username.contains(namefilter))
        {
            ui->adminTable->setItem(i,0,new QTableWidgetItem(QString::number(list[i].a_adminId)));
            ui->adminTable->setItem(i,1,new QTableWidgetItem(list[i].a_username));
            ui->adminTable->setItem(i,2,new QTableWidgetItem(list[i].a_password));
            ui->adminTable->setItem(i,3,new QTableWidgetItem(list[i].a_permissions));
        }
    }
}

//删除用户
void MainWindow::on_deleteAdminBtn_clicked()
{

    if(permissionLevel == "普通用户")
    {

        QMessageBox::warning(this, "警告", "用户权限不够!");
        return;
    }
    int rows = ui->adminTable->currentRow();
    if(rows >= 0)
    {
        QString username = ui->adminTable->item(rows, 1)->text();
        m_ptrStuSql->deleteAdministrator(username);
    }
}

//修改用户信息
void MainWindow::on_modifyAdminBtn_clicked()
{
    AdminInfo info;
    int rows = ui->adminTable->currentRow();
    if(rows >= 0)
    {
        // 获取当前选定行的管理员信息
        info.a_adminId = ui->adminTable->item(rows, 0)->text().toUInt();
        info.a_username = ui->adminTable->item(rows, 1)->text();
        info.a_password = ui->adminTable->item(rows, 2)->text();
        info.a_permissions = ui->adminTable->item(rows, 3)->text();

        // 检查权限级别，只有 "管理员" 权限才能修改所有用户的信息
        if (permissionLevel == "管理员")
        {
            m_addAdminDialog->setType(false, info);
            m_addAdminDialog->exec();
            onCurrentUserInformationChanged();
        }
        else if (permissionLevel == "普通用户")
        {
            // 检查是否是当前用户的信息，只有当前用户才能修改自己的信息
            if (info.a_username == currentUsername)
            {
                m_addAdminDialog->setType(false, info);
                m_addAdminDialog->exec();
                onCurrentUserInformationChanged();
            }
            else
            {
                QMessageBox::warning(this, "权限不足", "您没有权限修改其他用户的信息。");
            }
        }
    }
}


//更新用户表
void MainWindow::updateAdminTableWidget()
{
    ui->adminTable->clear();
    ui->adminTable->setColumnCount(ADM_COLUMN_COUNT);
    QStringList strlist;
    strlist << "序号" << "用户名" << "密码" << "权限";
    ui->adminTable->setHorizontalHeaderLabels(strlist);
    ui->adminTable->verticalHeader()->setVisible(false);//不显示默认行号
    ui->adminTable->setSelectionBehavior(QAbstractItemView::SelectRows);//只选中行
    ui->adminTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    auto count = m_ptrStuSql->queryAllAdminsNum();
    ui->adminTable->setRowCount(count);
    ui->statusbar->showMessage("管理员总数: " + QString::number(count));

    QList<AdminInfo> list = m_ptrStuSql->queryAllAdministratorsInfo();
    for(int i = 0; i < list.size(); i++)
    {
        ui->adminTable->setItem(i,0,new QTableWidgetItem(QString::number(list[i].a_adminId)));
        ui->adminTable->setItem(i,1,new QTableWidgetItem(list[i].a_username));
        ui->adminTable->setItem(i,2,new QTableWidgetItem(list[i].a_password));
        ui->adminTable->setItem(i,3,new QTableWidgetItem(list[i].a_permissions));
    }

}


void MainWindow::on_exitBtn_clicked()
{
    QApplication::quit();
}

