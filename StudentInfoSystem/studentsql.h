#ifndef STUDENTSQL_H
#define STUDENTSQL_H

#include <QObject>
#include <QtSql>
#include <QDebug>

//学生信息结构体
struct StudentInfo {
    quint32 s_serialNum;
    QString s_studentId;
    QString s_name;
    QString s_gender;
    QString s_phone;
    QString s_year;
    quint8 s_age;
    QString s_academy;
    QString s_speciality;
    QString s_class;
    double s_gpa;
};

//管理员信息结构体
struct AdminInfo {
    quint8 a_adminId;
    QString a_username;
    QString a_password;
    QString a_permissions;
};

class StudentSQL : public QObject
{
    Q_OBJECT
public:
    static StudentSQL* getInstance();
    bool connectToDatabase();
    void disconnectFromDatabase();
    bool performLoginValidation(const QString& username, const QString& password);
    //学生
    quint32 queryAllStudentsNum();
    QList<StudentInfo> queryStudentsInfoByPage(quint32 page, quint32 num);
    bool addStudent(StudentInfo info);
    bool deleteStudent(QString id);
    bool clearStudentTable();
    bool updateStudentInfo(StudentInfo info);
    //管理员
    quint32 queryAllAdminsNum();
    QList<AdminInfo> queryAllAdministratorsInfo();
    bool isAdiminExist(QString username);
    bool addAdministrator(AdminInfo info);
    bool deleteAdministrator(QString username);
    bool updateAdiminInfo(AdminInfo info);
    QString getCurrentUsername() const;
    QString getCurrentPermission(QString username) const;


private:
    explicit StudentSQL(QObject *parent = nullptr);
    StudentSQL(const StudentSQL&) = delete;
    StudentSQL& operator=(const StudentSQL&) = delete;
    QSqlDatabase   mysqlDatabase;
    static StudentSQL* ptrStuSQL;
    QString  username;

};

#endif // STUDENTSQL_H
