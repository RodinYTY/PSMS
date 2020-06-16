#include "root.h"
#include "ui_root.h"

Root::Root(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Root)
{
    ui->setupUi(this);
    ui->week->setVisible(false);
    ui->le->setVisible(false);
}

Root::~Root()
{
    delete ui;
}

void Root::setDBLink(QSqlDatabase _db){
    this->db = _db;
}

void Root::on_search_btn_teacher_clicked()
{
    qDebug() << "1";
}

void Root::on_search_btn_student_clicked()
{
    qDebug() << "1";
}

void Root::on_search_btn_course_clicked()
{
    qDebug() << "1";
}

void Root::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0: //日期
        ui->mon->setVisible(true);
        ui->day->setVisible(true);
        ui->week->setVisible(false);
        ui->le->setVisible(false);
        break;
    case 1: //星期
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(true);
        ui->le->setVisible(false);
        break;
    case 2: case 3: case 4:
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(false);
        ui->le->setVisible(true);
        break;
    }
}
