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

signals:
    void signalLoadField(procon::Field field);
    void signalMatchID(procon::Field field);

private slots:
    void loadCsvField();
    void loadMatchID();

private:
    Ui::Takao *ui;

    std::shared_ptr<GameManager> manager;
};

#endif // TAKAO_H
