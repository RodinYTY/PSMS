#include "signin.h"

SignIn::SignIn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignIn)
{
    ui->setupUi(this);
    QTimer::singleShot(100, this, SLOT(after_view_loaded())); //页面加载后
}

void SignIn::after_view_loaded(){
    init_database();
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::init_database(){
    QStringList strList = load_from_config();
    if(strList.length() == 0)
        qDebug() << "配置文件不存在";
    else{
        config._dbname = strList[0];
        config._linkname = strList[1];
        config._port = strList[2];
        config._usrname = strList[3];
        config._pwd = strList[4];
        config._rem = strList[5].toInt();
        config._auto = strList[6].toInt();
    }
    if(config._rem)
        ui->rem->setChecked(true);
    else
        ui->rem->setChecked(false);
    if(config._auto){
        ui->auto_2->setChecked(true);
        ui->rem->setChecked(true);
    }
    else
        ui->auto_2->setChecked(false);
    //自动填充帐密
    if(config._usrname != "x")
        ui->usrname->setText(config._usrname);
    if(config._pwd != "x")
        ui->pwd->setText(config._pwd);
    if(config._auto)
        on_signin_clicked();
}

void SignIn::on_signin_clicked()//登录
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.addButton("确定", QMessageBox::AcceptRole);
    int ret = link_database();
    if(ui->usrname->text().isEmpty() || ui->pwd->text().isEmpty()){
        msgBox.setText("输入框不能为空！");
        msgBox.exec();
        return;
    }
    //管理员账号
    if(ui->usrname->text().toLower() == "root"){
        if(ui->pwd->text() != sql.rootpwd){
            msgBox.setText("连接MySQL失败！");
            ui->pwd->clear();
            msgBox.exec();
            return;
        }
        else{
            //密码正确
            switch(ret){
            case -1:
                msgBox.setText("连接MySQL失败！");
                msgBox.exec();
                return;
            case 0:
                msgBox.setText("创建数据库失败！");
                msgBox.exec();
                return;
            case 1:
                if(ui->auto_2->isChecked())
                    config._auto = 1;
                else
                    config._auto = 0;
                if(ui->rem->isChecked()){
                    config._rem = 1;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), ui->pwd->text(), config._rem, config._auto);
                }
                else{
                    config._rem = 0;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), "x", config._rem, config._auto);
                }
                if(ui->usrname->text().toLower() == "root"){
                    r = new Root();
                    r->setWindowTitle("琴行管理系统(root)");
                    r->setDBLink(db);//将root连接传递过去
                    this->hide();
                    r->show();
                }
            }
        }
    }
    //非root账号
    else{
        if(!db.open())
            return;
        db.exec("use psms;");
        QSqlQuery query(db);
        query.exec(sql.acountInStudent.arg(ui->usrname->text()));
        query.next();
        if(query.value(0).toInt() == 1)//学生号
        {
            switch(link_database(ui->usrname->text(), ui->pwd->text())){
            case -1:
                msgBox.setText("连接MySQL失败！");
                msgBox.exec();
                return;
            case 1:
                //存信息到配置文件
                if(ui->auto_2->isChecked())
                    config._auto = 1;
                else
                    config._auto = 0;
                if(ui->rem->isChecked()){
                    config._rem = 1;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), ui->pwd->text(), config._rem, config._auto);
                }
                else{
                    config._rem = 0;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), "x", config._rem, config._auto);
                }
                qDebug() << "学生号";
                /*----------------*/

                break;
            }
        }
        else//不是学生号
        {
            query.exec(sql.acountInTeacher.arg(ui->usrname->text()));
            query.next();
            if(query.value(0).toInt() != 1)
            {
                msgBox.setText("账号不存在！");
                msgBox.exec();
                return;
            }
            //老师号
            switch(link_database(ui->usrname->text(), ui->pwd->text())){
            case -1:
                msgBox.setText("连接MySQL失败！");
                msgBox.exec();
                return;
            case 1:
                //存信息到配置文件
                if(ui->auto_2->isChecked())
                    config._auto = 1;
                else
                    config._auto = 0;
                if(ui->rem->isChecked()){
                    config._rem = 1;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), ui->pwd->text(), config._rem, config._auto);
                }
                else{
                    config._rem = 0;
                    save_to_config(config._dbname, config._linkname, config._port, ui->usrname->text(), "x", config._rem, config._auto);
                }
                qDebug() << "老师号";
                /*----------------*/

                break;
            }
        }
    }
}

int SignIn::link_database(){
    /*-------------连接数据库(root)-------------*/
    if (QSqlDatabase::contains("root_link"))
        db = QSqlDatabase::database("QMYSQL", "root_link");
    else
        db = QSqlDatabase::addDatabase("QMYSQL", "root_link");
    db.setHostName(config._linkname);
    db.setPort(config._port.toInt());
    db.setUserName("root");
    db.setPassword(sql.rootpwd);
    bool ok = db.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else{
        qDebug() << "数据库连接失败";
        return -1;
    }
    /*---------------创建数据库---------------*/
    QString query = "CREATE DATABASE IF NOT EXISTS " + config._dbname;
    db.exec(query);
    if (db.lastError().isValid())
    {
        qDebug() <<db.lastError();
        qDebug() << "创建数据库失败";
        return 0;
    }
    else
        qDebug() << "创建数据库成功";
    //使用数据库
    query = "USE PSMS;";
    db.exec(query);
    //建表
    db.exec(sql.createTables);
    if (db.lastError().isValid())
    {
        qDebug() <<db.lastError();
        qDebug() << "创建表和函数失败";
        return 0;
    }
    else
        qDebug() << "创建表和函数成功";
    //使用表
    query = "USE PSMS";
    db.exec(query);
    //创建角色
    db.exec(sql.createRoles);
    return 1;
}

int SignIn::link_database(QString usrname, QString pwd){
    QSqlDatabase db;
    if (QSqlDatabase::contains("non_root_link")) {
        db = QSqlDatabase::database("non_root_link");
    }
    else
        db = QSqlDatabase::addDatabase("QMYSQL", "non_root_link");
    /*-------------连接数据库-------------*/
    db.setHostName(config._linkname);
    db.setPort(config._port.toInt());
    db.setUserName(usrname);
    db.setPassword(pwd);
    bool ok = db.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else{
        qDebug() << "数据库连接失败";
        return -1;
    }
    //使用数据库
    db.close();
    //非root不用建表
    return 1;
}

void SignIn::on_signup_clicked()//注册
{
    u = new SignUp();
    u->setWindowTitle("注册窗口");
    u->setConfig(config);//传递连接参数
    u->show();
}

void SignIn::on_setting_clicked()
{
    s = new Setting();
    s->setWindowTitle("设置窗口");
    s->setModal(true);
    QObject::connect(this,SIGNAL(sendLinkInfo(QString, QString, QString)),s,SLOT(getLinkInfo(QString, QString, QString)));
    QObject::connect(s,SIGNAL(fixLinkInfo(QString, QString, QString)),this,SLOT(getLinkInfo(QString, QString, QString)));
    emit sendLinkInfo(config._dbname, config._linkname, config._port);
    s->show();
}

void SignIn::getLinkInfo(QString _dbname, QString _linkname, QString _port){ //来自子窗口
    config._linkname = _linkname;
    config._dbname = _dbname;
    config._port = _port;
    //更新配置struct
    config._linkname = _linkname;
    config._dbname = _dbname;
    config._port = _port;
    save_to_config(config._dbname, config._linkname, config._port, config._usrname, config._pwd, config._rem, config._auto);
}

void SignIn::on_auto_2_clicked(bool checked)//自动登录
{
    if(checked)
        ui->rem->setChecked(true);
}

QStringList SignIn::load_from_config(){
    QStringList strList;
    QFileInfo path("config");
    if(path.exists()==false){
        qDebug() << "配置文件不存在";
        return strList;
    }
    QFile file("config");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug() << "配置文件打开失败";
        return strList;
    }
    QTextStream in(&file);
    strList =  in.readLine().split(" ");
    return strList;
}

void SignIn::save_to_config(QString _dbname, QString _linkname, QString _port, QString _usrname, QString _pwd, int _rem,int _auto){
    QString in = "%1 %2 %3 %4 %5 %6 %7";
    QFile file("config");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QFile::Truncate)){
        qDebug() << "配置文件不存在";
        return;
    }
    in = in.arg(_dbname, _linkname, _port, _usrname, _pwd, QString::number(_rem), QString::number(_auto)).toUtf8();
    qDebug() << "已存储配置到本地：";
    qDebug() << in;
    file.write(in.toUtf8());
    file.close();
}
