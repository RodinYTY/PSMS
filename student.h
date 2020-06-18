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
    void setConfig(struct SQL::Configuration);
    void setDBLinkandUname(QSqlDatabase, QString);

private slots:
    void on_cancel_clicked();

    void on_save_clicked();

    void after_view_loaded();

    void on_comboBox_currentIndexChanged(int index);

    void gohome();

private:
    Ui::Student *ui;
    QSqlDatabase db;
    QString usrname;
    QStringList oldInfo;
    SQL::Configuration config;
    QSqlTableModel *model;
    void updateInfo();
    void updateTableView();
    QString tname_from_tno(QString tno);

signals:
    void windowsClosed();
};

#endif // STUDENT_H
