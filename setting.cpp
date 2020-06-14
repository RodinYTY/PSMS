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

void Setting::getLinkInfo(QString linkname, QString dbname){
    ui->old_linkname->setText(linkname);
    ui->old_dbname->setText(dbname);
}

void Setting::on_buttonBox_accepted()
{
    emit fixLinkInfo(ui->linkname->text().isEmpty()?ui->old_linkname->text():ui->linkname->text(),\
                     ui->dbname->text().isEmpty()?ui->old_dbname->text():ui->dbname->text());
}
