#include "signin.h"

SignIn::SignIn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignIn)
{
    ui->setupUi(this);
    //读取root密码
    QFileInfo path(".rootpwd");
    if(path.exists()==false){//配置文件不存在则需初始化
        qDebug() << "root密码配置文件不存在";
        bool ok;
        while(sql.rootpwd.isEmpty()){
            sql.rootpwd = QInputDialog::getText(this, "初始化系统","请输入MySQL的root账号密码", QLineEdit::Password, "", &ok);
            if(ok == false){
                exit(0);
            }
        }
        QFile file(".rootpwd");
        if(!file.open(QIODevice::WriteOnly | QFile::Truncate)){
            qDebug() << "root密码配置文件打开失败";
            QMessageBox::critical(this,"错误",file.fileName() + "打开失败");
            exit(-1);
        }
        //加密
        int i = 89;
        QString encryption;
        for(auto c:sql.rootpwd){
            char x = c.unicode() + (i += 2);
            encryption.append(x);
        }
        file.write(encryption.toUtf8());
        file.close();
    }
    else{
        QFile file(".rootpwd");
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "配置文件打开失败";
            QMessageBox::critical(this,"错误",file.fileName() + "打开失败");
            exit(-1);
        }
        QTextStream in(&file);
        QString origin = in.readLine();
        //解密
        int i = 89;
        QString deciphering;
        for(auto c:origin){
            char x = c.unicode() - (i += 2);
            deciphering.append(x);
        }
        sql.rootpwd = deciphering;
        file.close();
    }
    QTimer::singleShot(100, this, SLOT(after_view_loaded())); //页面加载后

}

void SignIn::after_view_loaded(){
    init_database();
}

void SignIn::redisplay(){
    this->show();
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::init_database(){
    QSqlDatabase db;
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
    QSqlDatabase db;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.addButton("确定", QMessageBox::AcceptRole);

    if(ui->usrname->text().isEmpty() || ui->pwd->text().isEmpty()){
        msgBox.setText("输入框不能为空！");
        msgBox.exec();
        return;
    }
    //管理员账号
    if(ui->usrname->text().toLower() == "root"){
        int ret = link_database(db);
        switch(ret){
        case -1:
            msgBox.setText("连接MySQL(root)失败！");
            msgBox.exec();
            ui->auto_2->setChecked(false);
            return;
        case 0:
            msgBox.setText("创建数据库失败！");
            msgBox.exec();
            ui->auto_2->setChecked(false);
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
            r = new Root();
            r->setWindowTitle("琴行管理系统[root]");
            //将root和config连接传递过去
            r->setDBLink(db);
            r->setConfig(config);
            connect(r, SIGNAL(windowsClosed()), this, SLOT(redisplay()));
            this->hide();
            r->show();
        }
    }
    //非root账号
    else{
        link_database(db);
        QSqlDatabase db1;
        if(!db.open()){
            msgBox.setText("数据库连接断开！");
            msgBox.exec();
            ui->auto_2->setChecked(false);
            return;
        }
        db.exec("USE " + config._dbname);
        QSqlQuery query(db);
        query.exec(sql.acountInStudent.arg(ui->usrname->text()));
        query.next();
        if(query.value(0).toInt() == 1)//学生号
        {
            switch(link_database(db1, ui->usrname->text(), ui->pwd->text())){
            case -1:
                msgBox.setText("连接MySQL失败！\n请检查数据库配置或登录账号");
                msgBox.exec();
                db1.close();
                ui->auto_2->setChecked(false);
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
                stu = new Student();
                stu->setDBLinkandUname(db, ui->usrname->text());
                stu->setConfig(config);
                stu->setWindowTitle("琴行管理系统[学生]");
                connect(stu, SIGNAL(windowsClosed()), this, SLOT(redisplay()));
                stu->show();
                this->hide();
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
                ui->auto_2->setChecked(false);
                return;
            }
            //老师号
            switch(link_database(db1, ui->usrname->text(), ui->pwd->text())){
            case -1:
                msgBox.setText("连接MySQL失败！\n请检查数据库配置或登录账号");
                msgBox.exec();
                ui->auto_2->setChecked(false);
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
                t = new Teacher();
                t->setDBLinkandUname(db, ui->usrname->text());
                t->setConfig(config);
                t->setWindowTitle("琴行管理系统[教师]");
                connect(t, SIGNAL(windowsClosed()), this, SLOT(redisplay()));
                t->show();
                this->hide();
                break;
            }
        }
    }
}

int SignIn::link_database(QSqlDatabase &db){
    /*-------------连接数据库[root]-------------*/
    if (QSqlDatabase::contains("root_link") && db.open())
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
        if(db.lastError().isValid()){
            QMessageBox::warning(this, tr(""),
                                 tr("数据库语句错误: %1")
                                 .arg(db.lastError().text()));
        }
        return 0;
    }
    else
        qDebug() << "创建数据库成功";
    //使用数据库
    query = "USE " + config._dbname;
    db.exec(query);
    //建表
    db.exec(sql.createTables);
    if (db.lastError().isValid())
    {
        qDebug() << db.lastError();
        qDebug() << "创建表和函数失败";
        if(db.lastError().isValid()){
            QMessageBox::warning(this, tr(""),
                                 tr("数据库语句错误: %1")
                                 .arg(db.lastError().text()));
        }
        return 0;
    }
    else
        qDebug() << "创建表和函数成功";
    //使用表
    query = "USE " + config._dbname;
    db.exec(query);
    //创建角色
    db.exec(sql.createRoles);
    //插入触发器，插入学生自动删除过期邀请码
    db.exec(sql.createTrigger);
    return 1;
}

int SignIn::link_database(QSqlDatabase &db1, QString usrname, QString pwd){
    if (QSqlDatabase::contains("non_root_link")) {
        db1 = QSqlDatabase::database("non_root_link");
    }
    else
        db1 = QSqlDatabase::addDatabase("QMYSQL", "non_root_link");
    /*-------------连接数据库-------------*/
    db1.setHostName(config._linkname);
    //db1.setDatabaseName(config._dbname);
    db1.setPort(config._port.toInt());
    db1.setUserName(usrname);
    db1.setPassword(pwd);
    bool ok = db1.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else{
        qDebug() << db1.lastError().text();
        qDebug() << "数据库连接失败";
        return -1;
    }
    //非root不用建表
    return 1;
}

void SignIn::on_signup_clicked()//注册
{
    u = new SignUp();
    u->setWindowTitle("注册窗口");
    u->setConfig(config);//传递连接参数
    u->sql.rootpwd = sql.rootpwd;
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
    QFileInfo path(".config");
    if(path.exists()==false){
        qDebug() << "配置文件不存在";
        return strList;
    }
    QFile file(".config");
    if(!file.open(QIODevice::ReadWrite)){
        qDebug() << "配置文件打开失败";
        return strList;
    }
    QTextStream in(&file);
    strList =  in.readLine().split(" ");
    //解密usrname
    QString usrname, pwd;
    int i = 99;

    if(strList[3] != "x"){
        qDebug() << "解码usrname";
        for(auto c:strList[3]){
            char x = c.unicode() - i++;
            usrname.append(x);
        }
        qDebug() << usrname;
        strList[3] = usrname;
    }
    //解密pwd
    if(strList[4] != 'x'){
        qDebug() << "解码pwd";
        for(auto c:strList[4]){
            char x = c.unicode() + i++;
            pwd.append(x);
        }
        qDebug() << pwd;
        strList[4] = pwd;
    }
    return strList;
}

void SignIn::save_to_config(QString _dbname, QString _linkname, QString _port, QString _usrname, QString _pwd, int _rem,int _auto){
    QString in = "%1 %2 %3 %4 %5 %6 %7";
    QFile file(".config");
    if(!file.open(QIODevice::ReadWrite | QFile::Truncate)){
        qDebug() << "配置文件打开失败";
        return;
    }
    QString usrname, pwd;
    int i = 99;
    if(_usrname == "x")
        usrname = "x";
    else{
        //加密usrname
        qDebug() << "usrname加密前后";
        qDebug() << _usrname;
        for(auto c:_usrname){
            char x = c.unicode() + i++;
            usrname.append(x);
        }
        qDebug() << usrname;
    }
    if(_pwd == "x")
        pwd = "x";
    else{
        //加密pwd
        qDebug() << "pwd加密前后";
        qDebug() << _pwd;
        for(auto c:_pwd){
            char x = c.unicode() - i++;
            pwd.append(x);
        }
        qDebug() << pwd;
    }
    in = in.arg(_dbname, _linkname, _port, usrname, pwd, QString::number(_rem), QString::number(_auto)).toUtf8();
    qDebug() << "已存储配置到本地：";
    qDebug() << in;
    file.write(in.toUtf8());
    file.close();
}
