#ifndef ROOT_H
#define ROOT_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRecord>
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
    void setConfig(struct SQL::Configuration);

private slots:
    void on_ccomb_currentIndexChanged(int index);

    void after_view_loaded();

    void on_code_btn_clicked();

    void on_rid_currentIndexChanged(const QString &arg1);

    void on_area_textChanged(const QString &arg1);

    void on_rent_textChanged(const QString &arg1);

    void on_fix_clicked();

    void disableFix();

    void on_submit_room_clicked();

    void on_revert_room_clicked();

    void on_minus_instr_clicked();

    void on_add_instr_clicked();

    void on_add_room_clicked();

    void on_delete_room_clicked();

    void on_submit_teacher_clicked();

    void on_revert_teacher_clicked();

    void on_minus_teacher_clicked();

    void on_search_teacher_clicked();

    void on_submit_student_clicked();

    void on_revert_student_clicked();

    void on_minus_student_clicked();

    void on_search_student_clicked();

private:
    SQL sql;
    Ui::Root *ui;
    QSqlDatabase db;
    SQL::Configuration config;
    QSqlTableModel *model, *model_t, *model_s, *model_c;
    void load_tables_to_tv();
};

#endif // ROOT_H
