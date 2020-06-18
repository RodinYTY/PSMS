#include "student.h"
#include "ui_student.h"

Student::Student(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
    QTimer::singleShot(20, this, SLOT(after_view_loaded())); //页面加载后
    /*-------设置le的约束正则式-------*/
    //电话
    QRegExp phone_rx("[0-9]{0,11}");
    QValidator *validator0 = new QRegExpValidator(phone_rx, this);
    ui->le_phone->setValidator(validator0);
    //名字
    QRegExp name_rx("[\u4e00-\u9fa5a-zA-Z]{0,20}");
    QValidator *validator2 = new QRegExpValidator(name_rx, this);
    ui->le_name->setValidator(validator2);
    //年龄
    QRegExp age_rx("[0-9]{0,4}");
    QValidator *validator3 = new QRegExpValidator(age_rx, this);
    ui->le_age->setValidator(validator3);
    //身份证号
    QRegExp id_rx("[0-9X]{0,18}");
    QValidator *validator4 = new QRegExpValidator(id_rx, this);
    ui->le_id->setValidator(validator4);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可选中

    connect(ui->gohome, SIGNAL(triggered()), this, SLOT(gohome()));
}

void Student::after_view_loaded(){
    updateInfo();
    updateTableView();
}

Student::~Student()
{
    delete ui;
}

void Student::setDBLinkandUname(QSqlDatabase _db, QString _uname){
    this->db = _db;
    this->usrname = _uname;
}

void Student::setConfig(struct SQL::Configuration _config){
    this->config = _config;
}

void Student::updateInfo(){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    if(!db.open()){
        msgBox.setText("数据库未连接！");
        msgBox.exec();
        return;
    }
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    query.exec(QString("select * from student where uname = \'%1\'").arg(usrname));
//    qDebug() << QString("select * from student where uname = \'%1\'").arg(usrname);
    query.next();

    ui->le_name->setText(query.value(1).toString());

    ui->le_age->setText(query.value(4).toString());
    ui->le_phone->setText(query.value(5).toString());
    ui->le_id->setText(query.value(2).toString());
    if(query.value(3).toString() == "男"){
        ui->sex->setCurrentIndex(0);
    }
    else{
        ui->sex->setCurrentIndex(1);
    }
    //存入列表
    oldInfo.clear();
    oldInfo.append(query.value(1).toString());
    oldInfo.append(query.value(2).toString());
    oldInfo.append(query.value(3).toString());
    oldInfo.append(query.value(4).toString());
    oldInfo.append(query.value(5).toString());
//    qDebug() << oldInfo;

    query.exec(QString("SELECT\
                       count( * )\
                   FROM\
                       course \
                   WHERE\
                       MONTH ( stime ) = MONTH ( now( ) ) \
                       AND sno IN (\
                       SELECT\
                           sno \
                       FROM\
                           student \
                   WHERE\
                       uname = \'%1\');").arg(usrname));

    query.next();
    ui->num1->setText(query.value(0).toString());

    query.exec(QString("SELECT\
                       count( * )\
                   FROM\
                       course \
                   WHERE\
                       sno IN (\
                       SELECT\
                           sno \
                       FROM\
                           student \
                       WHERE\
                           uname = \'%1\');").arg(usrname));
    query.next();
    ui->num2->setText(query.value(0).toString());


}

void Student::on_cancel_clicked()
{
    //恢复个人资料
    ui->le_name->setText(oldInfo[0]);
    ui->le_id->setText(oldInfo[1]);
    ui->le_age->setText(oldInfo[3]);
    ui->le_phone->setText(oldInfo[4]);
    if(oldInfo[2] == "男")
        ui->sex->setCurrentIndex(0);
    else
        ui->sex->setCurrentIndex(1);
}

void Student::on_save_clicked()
{
    QString update_name, update_age, update_sex, update_phone, update_id, update_major;
    update_name = "update student set sname = \'%1\' where uname = \'%2\'";
    update_age = "update student set sage = %1 where uname = \'%2\'";
    update_sex = "update student set ssex = \'%1\' where uname = \'%2\'";
    update_phone = "update student set sphone = \'%1\' where uname = \'%2\'";
    update_id = "update student set sid = \'%1\' where uname = \'%2\'";

    QMessageBox msgBox, msgBox1;
    msgBox.setIcon(QMessageBox::Critical);
    if(ui->le_name->text().length() > 10){
        ui->le_name->setFocus();
        msgBox.setText("姓名过长！");
        msgBox.exec();
        ui->le_name->clear();
        return;
    }
    if(ui->le_age->text().toInt() > 130 || ui->le_age->text().toInt() <= 0){
        ui->le_age->setFocus();
        msgBox.setText("年龄不正确！");
        msgBox.exec();
        ui->le_age->clear();
        return;
    }
    if(ui->le_id->text().length() < 18){
        ui->le_id->setFocus();
        msgBox.setText("身份证号码不足18位！");
        msgBox.exec();
        return;
    }

    db.exec("USE " + config._dbname);
    db.exec(update_name.arg(ui->le_name->text(), usrname));
    db.exec(update_age.arg(ui->le_age->text(), usrname));
    db.exec(update_phone.arg(ui->le_phone->text(), usrname));
    db.exec(update_id.arg(ui->le_id->text(), usrname));

    QString s;
    if(ui->sex->currentIndex() == 0)
        s = "男";
    else
        s = "女";
    db.exec(update_sex.arg(s, usrname));
    updateInfo();
    msgBox1.setText("修改完成");
    msgBox1.exec();
}

void Student::updateTableView(){
    QSqlQuery query(db);
    model = new QSqlTableModel(this, db);
    model->setTable("course");
    QString state1 = QString("sno in (select sno from student where uname = \'%1\')").arg(usrname), state2;
    model->setFilter(state1);//只能看本人的
    state2 = "datediff(stime, now()) <= %1";
    switch(ui->comboBox->currentIndex()){
    case 1://今天
        model->setFilter(state1 + " and " + state2.arg(0));
        break;
    case 2://本周
        model->setFilter(state1 + " and " + state2.arg(7));
        break;
    case 3://本月
        model->setFilter(state1 + " and " + state2.arg(30));
        break;
    }
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("老师"));
    model->setHeaderData(2, Qt::Horizontal, tr("房号"));
    model->setHeaderData(3, Qt::Horizontal, tr("上课时间"));
    model->setHeaderData(4, Qt::Horizontal, tr("下课时间"));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(5);

    //替换老师号为姓名
    QString content;
    QAbstractItemModel *abstract_model;
    QModelIndex index;
    for(int r = 0; r < model->rowCount(); r++){
        abstract_model = ui->tableView->model();
        index = abstract_model->index(r, 1);
        content = model->data(index).toString();
        model->setData(model->index(r, 1), tname_from_tno(content));
    }
}

void Student::on_comboBox_currentIndexChanged(int index)
{
    if(0) index = 0;
    updateTableView();
}

QString Student::tname_from_tno(QString tno){
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    query.exec(QString("select tname from teacher where tno = %1;").arg(tno));
    query.next();
    return query.value(0).toString();
}

void Student::gohome(){
    emit windowsClosed();
    this->close();
}
