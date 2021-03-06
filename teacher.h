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

    void on_comboBox_currentIndexChanged(int index);

    void gohome();

private:
    Ui::Teacher *ui;
    QSqlDatabase db;
    QString usrname;
    QStringList oldInfo;
    SQL::Configuration config;
    QSqlTableModel *model;
    void updateInfo();
    void updateTableView();
    QString sname_from_sno(QString);

signals:
    void windowsClosed();
};

#endif // TEACHER_H
