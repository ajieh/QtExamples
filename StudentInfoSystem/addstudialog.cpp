#include "addstudialog.h"
#include "ui_addstudialog.h"

AddStuDialog::AddStuDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStuDialog)
{
    ui->setupUi(this);
    initialMap();
    connect(ui->academyComboBox, QOverload<const QString &>::of(&QComboBox::activated), this, &AddStuDialog::onAcademyChanged);
    connect(ui->specialityComboBox, QOverload<const QString &>::of(&QComboBox::activated), this, &AddStuDialog::onSpecialityChanged);
}

AddStuDialog::~AddStuDialog()
{
    delete ui;
}

//设置是进行添加还是修改操作
void AddStuDialog::setType(bool type, StudentInfo info)
{
    m_isAdd = type;
    //m_stuInfo = info;
    ui->studentIDLineEdit->setText(info.s_studentId);
    ui->nameLineEdit->setText(info.s_name);
    ui->genderComboBox->setCurrentText(info.s_gender);
    ui->ageSpinBox->setValue(info.s_age);
    ui->phoneLineEdit->setText(info.s_phone);
    ui->dateEdit->setDate(QDate::fromString(info.s_year, "yyyy-MM-dd"));
    ui->academyComboBox->setCurrentText(info.s_academy);
    ui->specialityComboBox->setCurrentText(info.s_speciality);
    ui->classComboBox->setCurrentText(info.s_class);
    ui->GPASpinBox->setValue(info.s_gpa);
}

//用QMap来存储大学、专业和班级的信息
void AddStuDialog::initialMap()
{
    academySpecialityMap["计算机学院"] << "计算机科学与技术" << "软件工程" << "网络工程" << "信息安全";
    academySpecialityMap["外语学院"] << "英语" << "法语" << "德语" << "俄语";
    academySpecialityMap["建筑学院"] << "建筑学" << "土木工程" << "城市规划" << "风景园林";
    academySpecialityMap["生命学院"] << "生物工程" << "生物科学" << "生态学" << "植物学";

    specialityClassMap["计算机科学与技术"] << "计科01班" << "计科02班" << "计科03班";
    specialityClassMap["软件工程"] << "软件工程01班" << "软件工程02班" << "软件工程03班";
    specialityClassMap["网络工程"] << "网络工程01班" << "网络工程02班" << "网络工程03班";
    specialityClassMap["信息安全"] << "信息安全01班" << "信息安全02班" << "信息安全02班";

    specialityClassMap["英语"] << "英语01班" << "英语02班" << "英语03班";
    specialityClassMap["法语"] << "法语01班" << "法语02班" << "法语03班";
    specialityClassMap["德语"] << "德语01班" << "德语02班" << "德语03班";
    specialityClassMap["俄语"] << "俄语01班" << "俄语02班" << "俄语03班";

    specialityClassMap["建筑学"] << "建筑学01班" << "建筑学02班" << "建筑学03班";
    specialityClassMap["土木工程"] << "土木工程01班" << "土木工程02班" << "土木工程03班";
    specialityClassMap["城市规划"] << "城市规划01班" << "城市规划02班" << "城市规划03班";
    specialityClassMap["风景园林"] << "风景园林01班" << "风景园林02班" << "风景园林03班";

    specialityClassMap["生物工程"] << "生物工程01班" << "生物工程02班" << "生物工程03班";
    specialityClassMap["生物科学"] << "生物科学01班" << "生物科学02班" << "生物科学03班";
    specialityClassMap["生态学"] << "生态学01班" << "生态学02班" << "生态学03班";
    specialityClassMap["植物学"] << "植物学01班" << "植物学02班" << "植物学03班";
}

//保存按钮
void AddStuDialog::on_saveBtn_clicked()
{
    auto stu = StudentSQL::getInstance();
    StudentInfo info;
    info.s_studentId = ui->studentIDLineEdit->text();
    info.s_name = ui->nameLineEdit->text();
    info.s_gender = ui->genderComboBox->currentText();
    info.s_age = ui->ageSpinBox->text().toUInt();
    info.s_phone = ui->phoneLineEdit->text();
    info.s_year = ui->dateEdit->text();
    info.s_academy = ui->academyComboBox->currentText();
    info.s_speciality = ui->specialityComboBox->currentText();
    info.s_class = ui->classComboBox->currentText();
    info.s_gpa = ui->GPASpinBox->text().toDouble();

    if(m_isAdd)
    {
        stu->addStudent(info);
        QMessageBox::information(this, "信息", "保存成功");
    }
    else
    {
        stu->updateStudentInfo(info);
        QMessageBox::information(this, "信息", "更新成功");
    }

    this->hide();

}

//取消按钮
void AddStuDialog::on_cancelBtn_clicked()
{
    this->hide();
}

//当学院下拉框选项确定时，更新下属专业显示
void AddStuDialog::onAcademyChanged(const QString& academy)
{
    ui->specialityComboBox->clear();
    QStringList specialitiesList = academySpecialityMap.value(academy);
    ui->specialityComboBox->addItems(specialitiesList);
}

//当专业下拉框选项确定时，更新下属班级显示
void AddStuDialog::onSpecialityChanged(const QString& specialty)
{
    ui->classComboBox->clear();
    QStringList classesList = specialityClassMap.value(specialty);
    ui->classComboBox->addItems(classesList);
}

