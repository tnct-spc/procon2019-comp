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

private slots:
    void loadCsvField();

private:
    Ui::Takao *ui;

    ButtleAPI api;

    std::vector<int> match_ids;

    std::shared_ptr<AlgorithmWrapper> algorithm;
    std::vector<std::shared_ptr<procon::Field>> fields;
    std::vector<std::shared_ptr<SimpleVisualizer>> visualizers;

    void getFromButtleDataApi();
    void updateField(int buttle_index, std::string csv_str);
};

#endif // TAKAO_H
