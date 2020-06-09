#ifndef SIGNIN_H
#define SIGNIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SignIn; }
QT_END_NAMESPACE

class SignIn : public QMainWindow
{
    Q_OBJECT

public:
    SignIn(QWidget *parent = nullptr);
    ~SignIn();

private:
    Ui::SignIn *ui;
};
#endif // SIGNIN_H
