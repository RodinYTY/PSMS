#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private:
    Ui::Setting *ui;

private slots:
    //父窗口传值用
    void getLinkInfo(QString, QString);

    void on_buttonBox_accepted();

signals:
    void fixLinkInfo(QString, QString);
};

#endif // SETTING_H
