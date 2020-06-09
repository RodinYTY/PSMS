#ifndef SIGNIN_H
#define SIGNIN_H

#include <QMainWindow>
#include <QMessageBox>

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

private:
    Ui::SignIn *ui;
    QString linkname = "localhost"; // 默认本地连接
};
#endif // SIGNIN_H
