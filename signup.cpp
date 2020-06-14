#include "signup.h"
#include "ui_signup.h"
#define CODE_MAX_LEN 6

SignUp::SignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->lbl5->setVisible(false);
    ui->code->setVisible(false);
    ui->major->setVisible(false);
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
        ui->major->setVisible(false);
    }
    else{
        ui->lbl5->setVisible(true);
        ui->code->setVisible(true);
        ui->major->setVisible(true);
    }
}

void SignUp::on_code_textChanged(const QString &arg1)
{
    if(arg1.length() > CODE_MAX_LEN)
        ui->code->setText(arg1.left(CODE_MAX_LEN));
}
