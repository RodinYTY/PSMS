#ifndef SIGNIN_H
#define SIGNIN_H

#include "signup.h"
#include "ui_signin.h"
#include "setting.h"
#include "root.h"
#include "sql.h"
#include "student.h"
#include "teacher.h"
#include <QMainWindow>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class SignIn; }
QT_END_NAMESPACE

class SignIn : public QMainWindow
{
    Q_OBJECT

public:
    SignIn(QWidget *parent = nullptr);
    ~SignIn();

private slots:
    void on_signup_clicked();
    void on_setting_clicked();
    //接受子窗口传值用
    void getLinkInfo(QString, QString, QString);

    void on_auto_2_clicked(bool checked);

    void on_signin_clicked();

    void after_view_loaded();

private:
    void init_database();

    Ui::SignIn *ui;
    SignUp *u;
    Root *r;
    Setting *s;
    Student *stu;
    Teacher *t;
    SQL sql;

    struct SQL::Configuration config = {"psms", "localhost", "3306", QString("x"), QString("x"), 0, 0};

    QStringList load_from_config();
    void save_to_config(QString, QString, QString, QString, QString, int ,int);
    int link_database(QSqlDatabase &); //-1连接失败，0创表失败，1成功
    int link_database(QSqlDatabase &, QString, QString);//重载，非root连接

signals:
    void sendLinkInfo(QString, QString, QString);
};
#endif // SIGNIN_H
