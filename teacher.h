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
    void setConfig(struct SQL::Configuration);
    void setDBLinkandUname(QSqlDatabase, QString);

private slots:
    void after_view_loaded();

    void on_cancel_clicked();

    void on_save_clicked();

    void on_delete_2_clicked();

private:
    Ui::Teacher *ui;
    QSqlDatabase db;
    QString usrname;
    QStringList oldInfo;
    SQL::Configuration config;
    QSqlTableModel *model;
    void updateInfo();
    void updateTableView();

};

#endif // TEACHER_H
