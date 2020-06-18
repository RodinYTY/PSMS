#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>
#include <sql.h>

namespace Ui {
class Student;
}

class Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    ~Student();
    void setDBLinkandUname(QSqlDatabase, QString);

private:
    Ui::Student *ui;
    QSqlDatabase db;
    QString usrname;
};

#endif // STUDENT_H
