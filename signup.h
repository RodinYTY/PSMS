#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QInputDialog>

namespace Ui {
class SignUp;
}

class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_role_currentIndexChanged(int index);

private:
    Ui::SignUp *ui;
};

#endif // SIGNUP_H
