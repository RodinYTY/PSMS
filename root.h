#ifndef ROOT_H
#define ROOT_H

#include <QMainWindow>

namespace Ui {
class Root;
}

class Root : public QMainWindow
{
    Q_OBJECT

public:
    explicit Root(QWidget *parent = nullptr);
    ~Root();

private:
    Ui::Root *ui;
};

#endif // ROOT_H
