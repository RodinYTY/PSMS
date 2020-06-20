#include "root.h"
#include "ui_root.h"

Root::Root(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Root)
{
    ui->setupUi(this);
    ui->week->setVisible(false);
    ui->rid_c->setVisible(false);
    ui->le_course->setVisible(false);
    ui->fix->setEnabled(false);
    ui->area->setEnabled(false);
    ui->rent->setEnabled(false);
    ui->tv_room->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    ui->tv_teacher->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv_student->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv_course->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tv_room->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行
    QTimer::singleShot(100, this, SLOT(after_view_loaded())); //页面加载后

    connect(ui->gohome, SIGNAL(triggered()), this, SLOT(gohome()));
    connect(ui->changepwd, SIGNAL(triggered()), this, SLOT(changepwd()));

}

void Root::after_view_loaded(){
    //加载rno到comb上
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    //生成琴房号的combox item
    query.exec("select rno from room;");
    QString room_id;
    while(query.next()){
        room_id = query.value(0).toString();
        ui->rid->addItem(room_id);
        ui->rid_c->addItem(room_id);
    }
    load_tables_to_tv();

}

void Root::load_tables_to_tv(){
    //刷新表显示
    ui->fix->setEnabled(false);
    /*----------------课程----------------*/
    model_c = new QSqlTableModel(this, db);
    model_c->setTable("course");
    model_c->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置filter
    switch(ui->ccomb->currentIndex()){
    case 0://date: mon day
        if(ui->mon->currentIndex()!=0 && ui->day->currentIndex()==0){//某一月
            model_c->setFilter(QString("month(stime) = %1").arg(ui->mon->currentIndex()));
        }
        else if(ui->mon->currentIndex()==0 && ui->day->currentIndex()!=0){//某一天
            model_c->setFilter(QString("day(stime) = %1").arg(ui->day->currentIndex()));
        }
        else if(ui->mon->currentIndex()!=0 && ui->day->currentIndex()!=0){//具体一天
            model_c->setFilter(QString("month(stime) = %1 and day(stime) = %2").\
                               arg(QString::number(ui->mon->currentIndex()), QString::number(ui->day->currentIndex())));
        }
        break;
    case 1://weekday: week
        model_c->setFilter(QString("weekday(stime) = %1").arg(ui->week->currentIndex()));
        break;
    case 2://teacher: le_course
        if(!ui->le_course->text().isEmpty()){
            QString state;
            if(sql.isDigitStr(ui->le_course->text())){
                model_c->setFilter(sql.searchTeacher.arg(ui->le_course->text()));
            }
            else{
                model_c->setFilter(QString("tno in (select tno from teacher where tname like \'%%1%\' or uname like \'%%1%\')").arg(ui->le_course->text()));
            }
        }
        break;
    case 3://student: le_course
        if(!ui->le_course->text().isEmpty()){
            QString state;
            if(sql.isDigitStr(ui->le_course->text())){
                model_c->setFilter(sql.searchStudent.arg(ui->le_course->text()));
//                qDebug() << sql.searchStudent.arg(ui->le_course->text());
            }
            else{
                model_c->setFilter(QString("sno in (select sno from student where sname like \'%%1%\' or uname like \'%%1%\')").arg(ui->le_course->text()));
//                qDebug() << QString("sno in (select sno from student where sname like \'%%1%\' or uname like \'%%1%\')").arg(ui->le_course->text());
            }
        }
        break;
    case 4://room: rid_c
        model_c->setFilter(QString("rno = %1").arg(ui->rid_c->currentText()));
        break;
    }

    model_c->select();
    model_c->setHeaderData(0, Qt::Horizontal, tr("学生号"));
    model_c->setHeaderData(1, Qt::Horizontal, tr("老师号"));
    model_c->setHeaderData(2, Qt::Horizontal, tr("琴房号"));
    model_c->setHeaderData(3, Qt::Horizontal, tr("开始时间"));
    model_c->setHeaderData(4, Qt::Horizontal, tr("结束时间"));
    model_c->setHeaderData(5, Qt::Horizontal, tr("费用"));
    ui->tv_course->setModel(model_c);


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

    /*----------------老师----------------*/
    model_t = new QSqlTableModel(this, db);
    model_t->setTable("teacher");
    model_t->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_t->select();
    model_t->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model_t->setHeaderData(2, Qt::Horizontal, tr("身份证号"));
    model_t->setHeaderData(3, Qt::Horizontal, tr("性别"));
    model_t->setHeaderData(4, Qt::Horizontal, tr("年龄"));
    model_t->setHeaderData(5, Qt::Horizontal, tr("手机号"));
    model_t->setHeaderData(6, Qt::Horizontal, tr("专业"));
    model_t->setHeaderData(7, Qt::Horizontal, tr("账号"));
    if(!ui->le_teacher->text().isEmpty()){
        QString filter;
        if(ui->tcomb->currentIndex() == 0)//姓名
        {
            filter = "tname like \'%%1%\'";
            filter = filter.arg(ui->le_teacher->text());
            model_t->setFilter(filter);
        }
        else if(ui->tcomb->currentIndex() == 1)//手机号
        {
            filter = "tphone like \'%%1%\'";
            filter = filter.arg(ui->le_teacher->text());
            model_t->setFilter(filter);
        }
        else if(ui->tcomb->currentIndex() == 2)//身份证
        {
            filter = "tid like \'%%1%\'";
            filter = filter.arg(ui->le_teacher->text());
            model_t->setFilter(filter);
        }
        else{
            filter = "uname like \'%%1%\'";
            filter = filter.arg(ui->le_teacher->text());
            model_t->setFilter(filter);
        }
    }
    ui->tv_teacher->setModel(model_t);
    ui->tv_teacher->hideColumn(0);

    /*----------------学生----------------*/
    model_s = new QSqlTableModel(this, db);
    model_s->setTable("student");
    model_s->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_s->select();
    model_s->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model_s->setHeaderData(2, Qt::Horizontal, tr("身份证号"));
    model_s->setHeaderData(3, Qt::Horizontal, tr("性别"));
    model_s->setHeaderData(4, Qt::Horizontal, tr("年龄"));
    model_s->setHeaderData(5, Qt::Horizontal, tr("手机号"));
    model_s->setHeaderData(6, Qt::Horizontal, tr("账号"));
    if(!ui->le_student->text().isEmpty()){
        QString filter;
        if(ui->scomb->currentIndex() == 0)//姓名
        {
            filter = "sname like \'%%1%\'";
            filter = filter.arg(ui->le_student->text());
            model_s->setFilter(filter);
        }
        else if(ui->scomb->currentIndex() == 1)//手机号
        {
            filter = "sphone like \'%%1%\'";
            filter = filter.arg(ui->le_student->text());
            model_s->setFilter(filter);
        }
        else if(ui->scomb->currentIndex() == 2)//身份证
        {
            filter = "sid like \'%%1%\'";
            filter = filter.arg(ui->le_student->text());
            model_s->setFilter(filter);
        }
        else{
            filter = "uname like \'%%1%\'";
            filter = filter.arg(ui->le_student->text());
            model_s->setFilter(filter);
        }
    }
    ui->tv_student->setModel(model_s);
    ui->tv_student->hideColumn(0);
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

void Root::on_ccomb_currentIndexChanged(int index)
{
    switch (index) {
    case 0: //日期
        ui->mon->setVisible(true);
        ui->day->setVisible(true);
        ui->week->setVisible(false);
        ui->rid_c->setVisible(false);
        ui->le_course->setVisible(false);
        break;
    case 1: //星期
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(true);
        ui->rid_c->setVisible(false);
        ui->le_course->setVisible(false);
        break;
    case 2: case 3:
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(false);
        ui->rid_c->setVisible(false);
        ui->le_course->setVisible(true);
        break;
    case 4:
        ui->mon->setVisible(false);
        ui->day->setVisible(false);
        ui->week->setVisible(false);
        ui->rid_c->setVisible(true);
        ui->le_course->setVisible(false);
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
    msgBox.setText("邀请码为：\n" + code + "\n有效期为7天");
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
    QTimer::singleShot(8, this, SLOT(disableFix())); //页面加载后
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
    if(0) QString a = arg1;
    ui->fix->setEnabled(true);
}

void Root::on_rent_textChanged(const QString &arg1)
{
    if(0) QString a = arg1;
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
        if(db.lastError().isValid()){
            QMessageBox::warning(this, tr("room表"),
                                 tr("数据库错误: %1")
                                 .arg(db.lastError().text()));
        }
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
        if(db.lastError().isValid()){
            QMessageBox::warning(this, tr("room表"),
                                 tr("数据库错误: %1")
                                 .arg(db.lastError().text()));
        }
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
        QMessageBox::warning(this, tr(""), "room表",
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
    int curRow = ui->tv_room->currentIndex().row();
    //获取选中的行
    model->removeRow(curRow);
    //删除该行
    int ok = QMessageBox::warning(this,tr(""), tr("确定删除当前乐器吗？\n该操作不能撤回！"),
                         QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
       model->revertAll(); //如果不删除，则撤销
    }
    else{
        model->submitAll(); //否则提交，在数据库中删除该行
        statusBar()->showMessage(tr("移除成功"),2000);
    }
}

void Root::on_add_instr_clicked()
{
    QSqlRecord record = model->record();//获取空记录行
    int row = model->rowCount(); //获取当前的行号

    model->insertRecord(row,record);  //  插入行
    model->setData(model->index(row, 1), ui->rid->currentText());
}

void Root::on_add_room_clicked()
{
    bool ok;
    QMessageBox msgBox;
    QString new_area = QInputDialog::getText(this, "新琴房","面积(m²)", QLineEdit::Normal, "", &ok);
    if(!ok)
        return;
    if(new_area.isEmpty()){
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("文本框不能为空！");
        msgBox.exec();
        return;
    }
    QString new_rent = QInputDialog::getText(this, "新琴房","租金(￥/月)", QLineEdit::Normal, "", &ok);
    if(!ok)
        return;
    if(new_rent.isEmpty()){
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("文本框不能为空！");
        msgBox.exec();
        return;
    }
    QString state = "select min(rno) + 1 from room where rno\
 in (select rno from room x where not exists (select rno from \
room y where y.rno - x.rno = 1));";
    QSqlQuery query(db);
    query.exec(state);
    query.next();
    //待插入的rno
    int len = query.value(0).toInt();
    state = "insert into room\
            values (%1, %2, %3);";
    state = state.arg(QString::number(len), new_area, new_rent);
    db.exec(state);
    if(db.lastError().isValid()){
        QMessageBox::warning(this, tr("room表"),
                             tr("数据库错误: %1")
                             .arg(db.lastError().text()));
        return;
    }

    ui->rid->addItem(query.value(0).toString());
}

void Root::on_delete_room_clicked()
{
    int index = ui->rid->currentIndex();
    QString state = "delete from room where rno = %1";
    state = state.arg(ui->rid->currentText());
    db.exec(state);
    if(db.lastError().isValid()){
        QMessageBox::warning(this, tr("room表"),
                             tr("数据库错误: %1")
                             .arg(db.lastError().text()));
        return;
    }
    int ok = QMessageBox::warning(this,tr(""), tr("确定删除当前琴房吗？之中的乐器也将被删除\n该操作不能撤回！"),
                         QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
        return;
    //重新排琴房号
    ui->rid->clear();
    ui->rid->addItem("琴房号");
    QSqlQuery query(db);
    db.exec("USE " + config._dbname);
    //生成琴房号的combox item
    query.exec("select rno from room;");
    QString room_id;
    while(query.next()){
        room_id = query.value(0).toString();
        ui->rid->addItem(room_id);
    }
    ui->rid->setCurrentIndex(index - 1);
}

void Root::on_submit_teacher_clicked()
{
    model_t->database().transaction(); //开始事务操作
    if (model_t->submitAll()) {
        model_t->database().commit(); //提交
    } else {
        model_t->database().rollback(); //回滚
        QMessageBox::warning(this, tr(""), "teacher表",
                             tr("数据库错误: %1")
                             .arg(model_t->lastError().text()));
        return;
    }
    statusBar()->showMessage(tr("提交成功"),2000);
}

void Root::on_revert_teacher_clicked()
{
    model_t->revertAll();
    statusBar()->showMessage(tr("撤销成功"),2000);
}

void Root::on_minus_teacher_clicked()
{
    int curRow = ui->tv_teacher->currentIndex().row();

    //获取选中的行
    QAbstractItemModel *model = ui->tv_teacher->model();
    QModelIndex index = model->index(curRow, 7);
    QString uname = model->data(index).toString(), state;
    model_t->removeRow(curRow);

    /*--------------drop user--------------*/
    state = QString("drop user %1@%2").arg(uname, config._linkname);
    db.exec(state);
    if(db.lastError().isValid()){
        qDebug() << "未删除mysql用户";
        QMessageBox msgBox(this);
        msgBox.setText("MySQL中没有该用户的记录\n是否删除琴行系统中的记录？");
        msgBox.setIcon(QMessageBox::Information);//设置图标类型
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox:: Cancel);//对话框上包含的按钮

        if(msgBox.exec() == QMessageBox::Cancel)//模态调用
           return;

        else{
            model_t->submitAll();
            statusBar()->showMessage(tr("移除成功"),2000);
            return;
        }
    }
    //删除该行
    int ok = QMessageBox::warning(this,tr(""), tr("确定将该老师移除吗？\n此操作不能撤回！"),
                         QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
       model_t->revertAll(); //如果不删除，则撤销
    else{
        model_t->submitAll(); //否则提交，在数据库中删除该行
        statusBar()->showMessage(tr("移除成功"),2000);
    }
}

void Root::on_search_teacher_clicked()
{
    load_tables_to_tv();
}


void Root::on_submit_student_clicked()
{
    model_s->database().transaction(); //开始事务操作
    if (model_s->submitAll()) {
        model_s->database().commit(); //提交
    } else {
        model_s->database().rollback(); //回滚
        QMessageBox::warning(this, tr(""), "student表",
                             tr("数据库错误: %1")
                             .arg(model_s->lastError().text()));
        return;
    }
    statusBar()->showMessage(tr("提交成功"),2000);
}

void Root::on_revert_student_clicked()
{
    model_s->revertAll();
    statusBar()->showMessage(tr("撤销成功"),2000);
}


void Root::on_minus_student_clicked()
{
    int curRow = ui->tv_student->currentIndex().row();

    //获取选中的行
    QAbstractItemModel *model = ui->tv_student->model();
    QModelIndex index = model->index(curRow, 6);
    QString uname = model->data(index).toString(), state;
    model_s->removeRow(curRow);

    /*--------------drop user--------------*/
    state = QString("drop user %1@%2").arg(uname, config._linkname);
    db.exec(state);
    if(db.lastError().isValid()){
        qDebug() << "未删除mysql用户";
        QMessageBox msgBox(this);
        msgBox.setText("MySQL中没有该用户的记录\n是否删除琴行系统中的记录？");
        msgBox.setIcon(QMessageBox::Information);//设置图标类型
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox:: Cancel);//对话框上包含的按钮

        if(msgBox.exec() == QMessageBox::Cancel)//模态调用
           return;

        else{
            model_s->submitAll();
            statusBar()->showMessage(tr("移除成功"),2000);
            return;
        }
    }
    //删除该行
    int ok = QMessageBox::warning(this,tr(""), tr("确定将该学生移除吗？\n此操作不能撤回！"),
                         QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
       model_s->revertAll(); //如果不删除，则撤销
    else{
        model_s->submitAll(); //否则提交，在数据库中删除该行
        statusBar()->showMessage(tr("移除成功"),2000);
    }
}

void Root::on_search_student_clicked()
{
    load_tables_to_tv();
}

void Root::on_submit_course_clicked()
{
    model_c->database().transaction(); //开始事务操作
    if (model_c->submitAll()) {
        model_c->database().commit(); //提交
    } else {
        model_c->database().rollback(); //回滚
        QMessageBox::warning(this, tr(""), "course表",
                             tr("数据库错误: %1")
                             .arg(model_c->lastError().text()));
        return;
    }
    statusBar()->showMessage(tr("提交成功"),2000);
}

void Root::on_revert_course_clicked()
{
    model_c->revertAll();
    statusBar()->showMessage(tr("撤销成功"),2000);
}

void Root::on_plus_course_clicked()
{
    QSqlRecord record = model_c->record();//获取空记录行
    int row = model_c->rowCount(); //获取当前的行号

    model_c->insertRecord(row,record);  //  插入行
    model_c->setData(model->index(row, 1), ui->rid->currentText());
}

void Root::on_minus_course_clicked()
{
    int curRow = ui->tv_course->currentIndex().row();
    //获取选中的行
    model_c->removeRow(curRow);
    //删除该行
    int ok = QMessageBox::warning(this,tr(""), tr("确定删除当前课程吗？\n该操作不能撤回！"),
                         QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
       model_c->revertAll();
    }
    else{
        model_c->submitAll();
        statusBar()->showMessage(tr("移除成功"),2000);
    }
}

void Root::on_search_course_clicked()
{
    load_tables_to_tv();
}

void Root::gohome(){
    emit windowsClosed();
    this->close();
}

void Root::changepwd(){
    bool ok;
    QString rootpwd = QInputDialog::getText(this, "更改MySQL密码","请输入新的密码", QLineEdit::Password, "", &ok);
    if(rootpwd.isEmpty()){
        QMessageBox::critical(this,"","输入框为空，更改失败！");
        return;
    }
    if(ok == false){
        return;
    }
    QFile file(".rootpwd");
    if(!file.open(QIODevice::ReadWrite | QFile::Truncate)){
        qDebug() << "root密码配置文件打开失败";
        QMessageBox::critical(this,"错误","root密码配置文件打开失败");
        return;
    }
    //加密
    int i = 89;
    QString encryption;
    for(auto c:rootpwd){
        char x = c.unicode() + (i += 2);
        encryption.append(x);
    }
    file.write(encryption.toUtf8());
    file.close();
    QMessageBox msgBox;
    msgBox.setText("更改成功，需要重启程序以生效");
    msgBox.exec();
}
