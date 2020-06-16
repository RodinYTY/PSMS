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
    //首先连接数据库，失败则退出
    QTimer::singleShot(100, this, SLOT(after_view_loaded())); //页面加载后
    setConstraints();
}

void SignUp::after_view_loaded(){
    link_database(db);
    if(!db.open()){
        QMessageBox msgBox;
        db.close();
        QSqlDatabase::removeDatabase("QMYSQL");
        msgBox.setText("数据库连接失败，请检查数据库配置！");
        msgBox.exec();
        return;
    }
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::setConstraints(){
    /*-------设置le的约束正则式-------*/
    //电话
    QRegExp phone_rx("[0-9]{0,11}");
    QValidator *validator0 = new QRegExpValidator(phone_rx, this);
    ui->phone->setValidator(validator0);
    //用户名
    QRegExp uname_rx("[0-9a-zA-Z]{0,25}");
    QValidator *validator1 = new QRegExpValidator(uname_rx, this);
    ui->usrname->setValidator(validator1);
    //名字
    QRegExp name_rx("[\u4e00-\u9fa5a-zA-Z]{0,20}");
    QValidator *validator2 = new QRegExpValidator(name_rx, this);
    ui->name->setValidator(validator2);
    //年龄
    QRegExp age_rx("[0-9]{0,4}");
    QValidator *validator3 = new QRegExpValidator(age_rx, this);
    ui->age->setValidator(validator3);
    //身份证号
    QRegExp id_rx("[0-9X]{0,18}");
    QValidator *validator4 = new QRegExpValidator(id_rx, this);
    ui->id->setValidator(validator4);
}

void SignUp::setDBLink(QSqlDatabase _db){
    this->db = _db;
}

void SignUp::setConfig(const struct SQL::Configuration _config){
    config = _config;
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

void SignUp::on_signup_clicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.addButton("确定", QMessageBox::AcceptRole);
    if(ui->usrname->text().length() > 20){
        ui->usrname->setFocus();
        msgBox.setText("用户名过长！");
        msgBox.exec();
        return;
    }
    if(ui->usrname->text().isEmpty()){
        ui->usrname->setFocus();
        msgBox.setText("用户名不能为空！");
        msgBox.exec();
        return;
    }
    if(ui->pwd->text().isEmpty()){
        ui->pwd->setFocus();
        msgBox.setText("密码不能不为空！");
        msgBox.exec();
        return;
    }
    if(ui->pwd->text().length() < 6){
        ui->pwd->setFocus();
        msgBox.setText("密码过短！");
        msgBox.exec();
        ui->pwd->clear();
        return;
    }
    if(ui->pwd->text().length() > 20){
        ui->pwd->setFocus();
        msgBox.setText("密码过长！");
        msgBox.exec();
        ui->pwd->clear();
        return;
    }
    if(ui->name->text().length() > 10){
        ui->name->setFocus();
        msgBox.setText("姓名过长！");
        msgBox.exec();
        ui->name->clear();
        return;
    }
    if(ui->age->text().toInt() > 130 || ui->age->text().toInt() <= 0){
        ui->age->setFocus();
        msgBox.setText("年龄不正确！");
        msgBox.exec();
        ui->age->clear();
        return;
    }
    if(ui->sex->currentIndex() == 0){
        ui->sex->setFocus();
        msgBox.setText("未设置性别！");
        msgBox.exec();
        return;
    }
    if(ui->id->text().length() < 18){
        ui->id->setFocus();
        msgBox.setText("身份证号码不足18位！");
        msgBox.exec();
        return;
    }
    if(ui->role->currentIndex() == 1) //老师
    {
        if(ui->code->text().isEmpty()){
            ui->code->setFocus();
            msgBox.setText("教师账号需要获得管理员的邀请码才能注册");
            msgBox.exec();
            return;
        }
        if(ui->code->text().length() < 6){
            ui->code->setFocus();
            msgBox.setText("邀请码为6位");
            msgBox.exec();
            return;
        }
        if(ui->major->currentIndex() == 0){
            ui->major->setFocus();
            msgBox.setText("请设置您的专业");
            msgBox.exec();
            return;
        }
    }




    QMessageBox msgBox1;
    msgBox1.setText("注册成功！");
    msgBox1.exec();
    this->close();
}

void SignUp::link_database(QSqlDatabase &db){
    /*---------连接数据库---------*/
    if (QSqlDatabase::contains("QMYSQL"))
        db = QSqlDatabase::database(config._linkname);
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(config._linkname);
    db.setPort(config._port.toInt());
    db.setUserName("root");
    db.setPassword(sql.rootpwd);
    bool ok = db.open();
    config.print();
    if (ok)
        qDebug() << "数据库连接成功";
    else
        qDebug() << "数据库连接失败";
}

void SignUp::on_pwd1_editingFinished()
{
    QMessageBox msgBox;
    if(ui->pwd->text() != ui->pwd1->text()){
        msgBox.setText("两次输入的密码不一致！");
        ui->pwd1->clear();
        msgBox.exec();
    }
}

void SignUp::closeEvent(QCloseEvent *event){
    /*--------覆写closeEvent，关闭窗口自动断开连接--------*/
    if(0) qDebug() << event;
    db.close();
    QSqlDatabase::removeDatabase("QMYSQL");
}
