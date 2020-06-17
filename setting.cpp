#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::getLinkInfo(QString dbname, QString linkname, QString port){
    ui->old_linkname->setText(linkname);
    ui->old_dbname->setText(dbname);
    ui->old_port->setText(port);
}

void Setting::on_buttonBox_accepted()
{
    QMessageBox msgBox;
    if(ui->dbname->text().isEmpty() && ui->linkname->text().isEmpty() && ui->port->text().isEmpty()){
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("输入框不能为空！");
        msgBox.addButton("确定", QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    if(!ui->port->text().isEmpty() && !ui->port->text().contains(QRegExp("^\\d+$"))){
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("端口号必须为数字！");
        msgBox.addButton("确定", QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    emit fixLinkInfo(ui->dbname->text().isEmpty()?ui->old_dbname->text():ui->dbname->text(),\
                     ui->linkname->text().isEmpty()?ui->old_linkname->text():ui->linkname->text(),\
                     ui->port->text().isEmpty()?ui->old_port->text():ui->port->text());
}
