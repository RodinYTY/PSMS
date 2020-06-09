#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->lbl5->setVisible(false);
    ui->code->setVisible(false);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_role_currentIndexChanged(int index)
{
    if(index == 0){
        ui->lbl5->setVisible(false);
        ui->code->setVisible(false);
    }
    else{
        ui->lbl5->setVisible(true);
        ui->code->setVisible(true);
    }
}
