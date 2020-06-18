#include "teacher.h"
#include "ui_teacher.h"

Teacher::Teacher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Teacher)
{
    ui->setupUi(this);
}

Teacher::~Teacher()
{
    delete ui;
}

void Teacher::setDBLinkandUname(QSqlDatabase _db, QString _uname){
    this->db = _db;
    this->usrname = _uname;
}
