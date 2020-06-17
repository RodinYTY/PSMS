#ifndef SIGNIN_H
#define SIGNIN_H

#include "signup.h"
#include "ui_signin.h"
#include "setting.h"
#include "root.h"
#include "sql.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTimer>

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
    QSqlDatabase db; //数据库命令执行接口
    Ui::SignIn *ui;
    SignUp *u;
    Root *r;
    Setting *s;
    SQL sql;

    struct SQL::Configuration config = {"psms", "localhost", "3306", QString("x"), QString("x"), 0, 0};

    QStringList load_from_config();
    void save_to_config(QString, QString, QString, QString, QString, int ,int);
    int link_database(); //-1连接失败，0创表失败，1成功

signals:
    void sendLinkInfo(QString, QString, QString);
};
#endif // SIGNIN_H
