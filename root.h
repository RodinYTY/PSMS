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
    void on_comboBox_currentIndexChanged(int index);

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

private:
    SQL sql;
    Ui::Root *ui;
    QSqlDatabase db;
    SQL::Configuration config;
    QSqlTableModel *model;
    QSqlTableModel *model_course, *model_room, *model_teacher, *model_student;
    void load_tables_to_tv();
};

#endif // ROOT_H
