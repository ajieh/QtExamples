#include "studentsql.h"
StudentSQL* StudentSQL::ptrStuSQL = nullptr;

StudentSQL* StudentSQL::getInstance()
{
    if(nullptr == ptrStuSQL)
    {
        ptrStuSQL = new StudentSQL;
    }
    return ptrStuSQL;
}

StudentSQL::StudentSQL(QObject *parent)
    : QObject{parent}
{

}

//通过ODBC连接MySQL
bool StudentSQL::connectToDatabase()
{
    mysqlDatabase = QSqlDatabase::addDatabase("QODBC");
    mysqlDatabase.setHostName("127.0.0.1");
    mysqlDatabase.setPort(3306);
    mysqlDatabase.setDatabaseName("MySQL");
    mysqlDatabase.setUserName("root");
    mysqlDatabase.setPassword("258096");

    if (!mysqlDatabase.open()) {
        qDebug() << "Database connection error:" << mysqlDatabase.lastError().text();
        return false;
    }
    qDebug() << "Database connected!";
    return true;
}

//关闭数据库
void StudentSQL::disconnectFromDatabase()
{
    if(mysqlDatabase.open())
    {
        mysqlDatabase.close();
        qDebug() << "Database disconnected!";
    }
}

//进行登录验证
bool StudentSQL::performLoginValidation(const QString &username, const QString &password)
{

    QSqlQuery query(mysqlDatabase);
    query.prepare("SELECT * FROM administrators WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec())
    {
        if (query.next())
        {
            return true;  // 登录验证成功
        }
        else
        {
            qDebug() << "No administrator found with the given username and password.";
        }
    }
    else
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }

    return false;  // 登录验
}

//查询所有学生数量
quint32 StudentSQL::queryAllStudentsNum()
{
    QSqlQuery query(mysqlDatabase);
    query.exec("SELECT COUNT(serialNumber) FROM students");
    quint32 uintNum = 0;
    while(query.next())
    {
        uintNum = query.value(0).toUInt();
    }
    return uintNum;
}

//查询第几页学生的信息
QList<StudentInfo> StudentSQL::queryStudentsInfoByPage(quint32 page, quint32 num)
{
    QList<StudentInfo> list;
    QSqlQuery query(mysqlDatabase);
    query.prepare("SELECT * FROM students ORDER BY serialNumber LIMIT :num OFFSET :page");
    query.bindValue(":num", num);
    query.bindValue(":page", page);
    query.exec();

    StudentInfo info;
    while (query.next())
    {
        info.s_serialNum = query.value(0).toString().toUInt();
        info.s_studentId = query.value(1).toString();
        info.s_name = query.value(2).toString();
        info.s_gender = query.value(3).toString();
        info.s_phone = query.value(4).toString();
        info.s_year = query.value(5).toString();
        info.s_age = query.value(6).toUInt();
        info.s_academy = query.value(7).toString();
        info.s_speciality = query.value(8).toString();
        info.s_class = query.value(9).toString();
        info.s_gpa = query.value(10).toString().toDouble();
        list.push_back(info);
    }
    return list;
}

//添加学生信息
bool StudentSQL::addStudent(StudentInfo info)
{
    QSqlQuery query(mysqlDatabase);

    // 查询是否已存在相同学号的学生
    query.prepare("SELECT COUNT(*) FROM students WHERE student_id = :student_id");
    query.bindValue(":student_id", info.s_studentId);
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return false;
    }

    // 获取查询结果
    if (query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            qDebug() << "Student with ID" << info.s_studentId << "already exists.";
            return false; // 学号已存在，无法添加
        }
    }
    else
    {
        qDebug() << "Error fetching results:" << query.lastError().text();
        return false;
    }

    // 插入新学生信息
    query.prepare("INSERT INTO students VALUES (null, :student_id, :name, :gender, :phone, :time_of_enrollment,"
                  ":age, :academy, :speciality, :class, :gpa)");
    query.bindValue(":student_id", info.s_studentId);
    query.bindValue(":name", info.s_name);
    query.bindValue(":gender", info.s_gender);
    query.bindValue(":phone", info.s_phone);
    query.bindValue(":time_of_enrollment", info.s_year);
    query.bindValue(":age", info.s_age);
    query.bindValue(":academy", info.s_academy);
    query.bindValue(":speciality", info.s_speciality);
    query.bindValue(":class", info.s_class);
    query.bindValue(":gpa", info.s_gpa);
    return query.exec();
}


//根据学号删除学生
bool StudentSQL::deleteStudent(QString id)
{
    QSqlQuery query(mysqlDatabase);
    query.prepare("DELETE FROM students WHERE student_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

//清空学生表
bool StudentSQL::clearStudentTable()
{
    QSqlQuery query(mysqlDatabase);
    return query.exec("DELETE FROM students");
}

//更新学生信息
bool StudentSQL::updateStudentInfo(StudentInfo info)
{
    QSqlQuery query(mysqlDatabase);
    query.prepare("UPDATE students SET name = :name, gender = :gender, phone = :phone, age = :age, academy = :academy,"
                  "speciality = :speciality, class = :class, gpa = :gpa WHERE student_id = :student_id");
    query.bindValue(":name", info.s_name);
    query.bindValue(":gender", info.s_gender);
    query.bindValue(":phone", info.s_phone);
    query.bindValue(":age", info.s_age);
    query.bindValue(":academy", info.s_academy);
    query.bindValue(":speciality", info.s_speciality);
    query.bindValue(":class", info.s_class);
    query.bindValue(":gpa", info.s_gpa);
    query.bindValue(":student_id", info.s_studentId);
    return query.exec();
}

quint32 StudentSQL::queryAllAdminsNum()
{
    QSqlQuery query(mysqlDatabase);
    query.exec("SELECT COUNT(admin_id) FROM administrators");
    quint32 uintNum = 0;
    while(query.next())
    {
        uintNum = query.value(0).toUInt();
    }
    return uintNum;
}

//获取所有管理员信息
QList<AdminInfo> StudentSQL::queryAllAdministratorsInfo()
{
    QList<AdminInfo> list;
    QSqlQuery query(mysqlDatabase);
    query.prepare("SELECT * FROM administrators");
    query.exec();

    AdminInfo info;
    while (query.next())
    {
        info.a_adminId = query.value(0).toUInt();
        info.a_username = query.value(1).toString();
        info.a_password = query.value(2).toString();
        info.a_permissions = query.value(3).toString();
        list.push_back(info);
    }
    return list;
}

//判断用户是否存在
bool StudentSQL::isAdiminExist(QString username)
{
    QSqlQuery query(mysqlDatabase);
    query.prepare("SELECT COUNT(*) FROM administrators WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next())
    {
        return query.value(0).toInt() > 0;
    }
    else
    {
        qDebug() << "Error checking administrator existence for username" << username << ":" << query.lastError().text();
        return false;
    }
}

//添加管理员
bool StudentSQL::addAdministrator(AdminInfo info)
{
    if(isAdiminExist(info.a_username))
    {
        return false;
    }
    else
    {
        QSqlQuery query(mysqlDatabase);
        query.prepare("INSERT INTO administrators (username, password, permissions)"
                      "VALUES (:username, :password, :permissions)");
        query.bindValue(":username", info.a_username);
        query.bindValue(":password", info.a_password);
        query.bindValue(":permissions", info.a_permissions);
        return query.exec();
    }

}

//删除管理员
bool StudentSQL::deleteAdministrator(QString username)
{
    QSqlQuery query(mysqlDatabase);
    query.prepare("DELETE FROM administrators WHERE username = :username");
    query.bindValue(":username", username);
    return query.exec();
}

//更新管理员信息
bool StudentSQL::updateAdiminInfo(AdminInfo info)
{
    username = info.a_username;
    qDebug() << info.a_adminId;
    QSqlQuery query(mysqlDatabase);
    query.prepare("UPDATE administrators SET username = :username, password = :password, permissions = :permissions "
                  "WHERE admin_id = :admin_id");
    query.bindValue(":username", info.a_username);
    query.bindValue(":password", info.a_password);
    query.bindValue(":permissions", info.a_permissions);
    query.bindValue(":admin_id", info.a_adminId);
    return query.exec();
}

//获取当前用户名
QString StudentSQL::getCurrentUsername() const
{
    return username;
}

//获取当前用户权限
QString StudentSQL::getCurrentPermission(QString username) const
{
    QSqlQuery query(mysqlDatabase);
    query.prepare("SELECT permissions FROM administrators WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next())
    {
        QString permissions = query.value(0).toString();
        return permissions;
    }
    else
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return "";
    }
}

