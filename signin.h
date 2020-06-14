#ifndef SIGNIN_H
#define SIGNIN_H

#include <QMainWindow>
#include <QMessageBox>
#include "signup.h"
#include "ui_signin.h"
#include "setting.h"

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
    void getLinkInfo(QString, QString);

    void on_auto_2_clicked(bool checked);

private:
    Ui::SignIn *ui;
    SignUp *u;
    Setting *s;
    QString linkname = "localhost"; // 默认本地连接
    QString dbname = "MySQLDB"; // 默认数据库名称

signals:
    void sendLinkInfo(QString, QString);
};
#endif // SIGNIN_H
