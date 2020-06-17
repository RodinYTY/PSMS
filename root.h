#ifndef ROOT_H
#define ROOT_H

#include <QMainWindow>
#include <sql.h>

namespace Ui {
class Root;
}

class Root : public QMainWindow
{
    Q_OBJECT

public:
    explicit Root(QWidget *parent = nullptr);
    ~Root();
    void setDBLink(QSqlDatabase);

private slots:
    void on_search_btn_teacher_clicked();

    void on_search_btn_student_clicked();

    void on_search_btn_course_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void after_view_loaded();

private:
    Ui::Root *ui;
    QSqlDatabase db;
};

#endif // ROOT_H
