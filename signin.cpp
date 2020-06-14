#include "signin.h"

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
    u = new SignUp();
    u->setWindowTitle("注册窗口");
    u->show();
}

void SignIn::on_setting_clicked()
{
    s = new Setting();
    s->setWindowTitle("设置窗口");
    s->setModal(true);
    QObject::connect(this,SIGNAL(sendLinkInfo(QString, QString)),s,SLOT(getLinkInfo(QString, QString)));
    QObject::connect(s,SIGNAL(fixLinkInfo(QString, QString)),this,SLOT(getLinkInfo(QString, QString)));
    emit sendLinkInfo(linkname, dbname);
    s->show();
}

void SignIn::getLinkInfo(QString _linkname, QString _dbname){
    linkname = _linkname;
    dbname = _dbname;
}

void SignIn::on_auto_2_clicked(bool checked)
{
    if(checked)
        ui->rem->setChecked(true);
}
