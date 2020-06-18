#ifndef TEACHER_H
#define TEACHER_H

#include <QMainWindow>
#include <sql.h>

namespace Ui {
class Teacher;
}

class Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacher(QWidget *parent = nullptr);
    ~Teacher();
    void setDBLinkandUname(QSqlDatabase, QString);

private:
    Ui::Teacher *ui;
    QSqlDatabase db;
    QString usrname;
};

#endif // TEACHER_H
