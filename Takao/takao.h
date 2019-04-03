#ifndef TAKAO_H
#define TAKAO_H

#include <QMainWindow>
#include "gamemanager.h"

namespace Ui {
class Takao;
}

class Takao : public QMainWindow
{
    Q_OBJECT

public:
    explicit Takao(QWidget *parent = nullptr);
    ~Takao();

private:
    Ui::Takao *ui;
};

#endif // TAKAO_H
