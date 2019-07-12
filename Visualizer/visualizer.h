#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QFileDialog>
#include <memory>
#include "csvtranslate.h"

namespace Ui {
class Visualizer;
}

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Visualizer(std::shared_ptr<const procon::Field> field = nullptr, QWidget *parent = nullptr);
    ~Visualizer();

    void setFieldPtr(std::shared_ptr<const procon::Field> field);
    bool isInputEnded();

signals:
    void signalResetField();
    void signalRunSimulator();
    void signalRunFullSimulation();
    void signalSimulateNextTurn();
    void signalReverseField();
    void signalMoveAgents();

private:

    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);

    Ui::Visualizer *ui;
    std::shared_ptr<const procon::Field> field;
    //↓初期化できてるか怪しい
    std::vector<std::vector<procon::Point>> move_agent {2,std::vector<procon::Point>(field->getAgentCount(),{-1,-1})};
    bool auto_mode = false;

    const double margin = 2;

    const QColor font_color = QColor(0, 0, 0, 64);
    const QColor background_color = QColor(245, 245, 220);
    const QColor grid_color = QColor(220, 220, 180);
    const QColor score_color = QColor(250, 80, 80, 120);
    const QColor automode_color = score_color;
    const std::array<QColor, 2> team_colors = {QColor(255, 0, 0), QColor(0, 0, 255)};
};

#endif // VISUALIZER_H
