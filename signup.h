#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
#include <sql.h>

namespace Ui {
class SignUp;
}


class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();
    void setDBLink(QSqlDatabase);
    void setConfig(const struct SQL::Configuration);
    SQL sql;

private slots:
    void on_role_currentIndexChanged(int index);

    void on_code_textChanged(const QString &arg1);

    void on_signup_clicked();

    void on_pwd1_editingFinished();

    void after_view_loaded();

    void on_usrname_editingFinished();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::SignUp *ui;
    QSqlDatabase db;
    void setConstraints();
    struct SQL::Configuration config;
    void link_database(QSqlDatabase &);
};

#endif // SIGNUP_H
