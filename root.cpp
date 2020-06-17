#include "root.h"
#include "ui_root.h"

Root::Root(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Root)
{
    ui->setupUi(this);
    ui->week->setVisible(false);
    ui->le->setVisible(false);
    ui->fix->setEnabled(false);
    ui->area->setEnabled(false);
    ui->rent->setEnabled(false);
    ui->tv_room->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    //ui->tv_room->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行

    QTimer::singleShot(100, this, SLOT(after_view_loaded())); //页面加载后
}

void Root::load_tables_to_tv(){
    /*----------------课程----------------*/




    /*----------------琴房----------------*/
    QString roomid = ui->rid->currentText();
    model = new QSqlTableModel(this, db);
    model->setTable("instrument");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setFilter(QString("rno = %1").arg(roomid));
    model->select();
    model->setHeaderData(2, Qt::Horizontal, tr("型号"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    ui->tv_room->setModel(model);
    ui->tv_room->hideColumn(0);
    ui->tv_room->hideColumn(1);
    ui->fix->setEnabled(false);


    /*----------------学生----------------*/





    /*----------------老师----------------*/







}

void Root::after_view_loaded(){
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    //生成琴房号的combox item
    query.exec("select rno from room;");
    QString room_id;
    while(query.next()){
        room_id = query.value(0).toString();
        ui->rid->addItem(room_id);
    }
    load_tables_to_tv();




}

Root::~Root()
{
    delete ui;
}

void Root::setConfig(struct SQL::Configuration _config){
    this->config = _config;
}

void Root::setDBLink(QSqlDatabase _db){
    this->db = _db;
}

void Root::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0: //日期
        ui->mon->setVisible(true);
        ui->day->setVisible(true);
        ui->week->setVisible(false);
        ui->le->setVisible(false);
        break;
    case 1: //星期
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(true);
        ui->le->setVisible(false);
        break;
    case 2: case 3: case 4:
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(false);
        ui->le->setVisible(true);
        break;
    }
}

void Root::on_code_btn_clicked()
{
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    query.exec("select genCode(6);");
    query.next();
    QString code = query.value(0).toString();
    db.exec(sql.insertCodeOf.arg(code));
    QMessageBox msgBox;
    msgBox.setText("邀请码为：" + code + "\n有效期为7天");
    msgBox.exec();
}

void Root::on_rid_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "琴房号"){
        ui->area->clear();
        ui->area->setEnabled(false);
        ui->rent->clear();
        ui->rent->setEnabled(false);
        ui->fix->setEnabled(false);
        QSqlTableModel *model1 = new QSqlTableModel();
        ui->tv_room->setModel(model1);
        return;
    }
    ui->area->setEnabled(true);
    ui->rent->setEnabled(true);
    QTimer::singleShot(1, this, SLOT(disableFix())); //页面加载后
    load_tables_to_tv();
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    query.exec("select rarea, rrent from room where rno = " + arg1);
    query.next();
    ui->area->setText(query.value(0).toString());
    ui->rent->setText(query.value(1).toString());
}

void Root::disableFix(){
    ui->fix->setEnabled(false);
}

void Root::on_area_textChanged(const QString &arg1)
{
    ui->fix->setEnabled(true);
}

void Root::on_rent_textChanged(const QString &arg1)
{
    ui->fix->setEnabled(true);
}

void Root::on_fix_clicked()//修改房间信息
{
    QMessageBox msgBox;
    msgBox.addButton("确定", QMessageBox::AcceptRole);
    if(ui->area->text().isEmpty() || ui->rent->text().isEmpty()){
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("文本框不能为空！");
        msgBox.exec();
        return;
    }
    QString state1 = "update room set rarea = %1 where rno = %2";
    state1 = state1.arg(ui->area->text(), ui->rid->currentText());
    QString state2 = "update room set rrent = %1 where rno = %2";
    state2 = state2.arg(ui->rent->text(), ui->rid->currentText());
    db.exec(state1);
    if (db.lastError().isValid())
    {
        qDebug() <<db.lastError();
        qDebug() << "area修改失败";
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("数据修改失败！");
        msgBox.exec();
        return;
    }
    db.exec(state2);
    if (db.lastError().isValid())
    {
        qDebug() <<db.lastError();
        qDebug() << "rent修改失败";
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("数据修改失败！");
        msgBox.exec();
        return;
    }
    msgBox.setText("数据修改成功！");
    msgBox.exec();
}

void Root::on_submit_room_clicked()
{
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
        model->database().commit(); //提交
    } else {
        model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1")
                             .arg(model->lastError().text()));
        return;
    }
    statusBar()->showMessage(tr("提交成功"),2000);
}

void Root::on_revert_room_clicked()
{
    model->revertAll();
    statusBar()->showMessage(tr("撤销成功"),2000);
}

void Root::on_minus_instr_clicked()
{

}
