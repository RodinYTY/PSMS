#include "signin.h"
#include "signup.h"
#include "ui_signin.h"

SignIn::SignIn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignIn)
{
    ui->setupUi(this);
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::on_signup_clicked()
{
    SignUp* u = new SignUp();
    u->setWindowTitle("注册窗口");
    u->show();
}

void SignIn::on_setting_clicked()
{
    QString tempt;
    bool isOK;
    tempt = QInputDialog::getText(this, "连接设置", "更改当前连接名" + linkname + "为：", QLineEdit::Normal, "", &isOK);
    if(tempt.size()){
        linkname = tempt;
        QMessageBox msgBox;
        //msgBox.setIcon(QMessageBox::AcceptRole);
        msgBox.setText("修改完成");
        msgBox.addButton("确定", QMessageBox::AcceptRole);
        msgBox.exec();
    }
   else if(isOK){
       QMessageBox::information(this, "", "连接名不能为空");
   }
}
