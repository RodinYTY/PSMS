#include "student.h"
#include "ui_student.h"

Student::Student(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
}

Student::~Student()
{
    delete ui;
}

void Student::setDBLinkandUname(QSqlDatabase _db, QString _uname){
    this->db = _db;
    this->usrname = _uname;
}
