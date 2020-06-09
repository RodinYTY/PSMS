#include "signin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignIn w;
    w.setWindowTitle("登录窗口");
    w.show();
    return a.exec();
}
