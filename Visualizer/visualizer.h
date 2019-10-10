#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
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
    void resetAgentAct();
    void resetStrategy(bool setState);
    void setMoves(std::vector<procon::MoveState>& moves);
    bool auto_mode = true;

signals:
    void signalResetField();
    void signalRunSimulator();
    void signalRunFullSimulation();
    void signalSimulateNextTurn();
    void signalReverseField();
    void signalMoveAgents(const std::vector<procon::Point>& inp_vec, std::vector<int> is_delete, bool manual_team);
    void signalStrategy(std::vector<std::vector<bool>> strategy);
    void signalSendMove();
    void signalGetField();
    void signalSetMove();

private:

    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void checkClickedAgent(procon::Point mass);
    void checkClickGrid(procon::Point mass, bool right_flag);

    Ui::Visualizer *ui;
    std::shared_ptr<const procon::Field> field;
    std::vector<procon::Point> move_agent;
    int window_width;
    int window_height;
    int grid_size;
    int horizontal_margin;
    int vertical_margin;
    int agent_count;
    bool selected = false;
    unsigned int confirm_count = 0;
    procon::Point clicked_grid;
    std::pair<int, int> selected_agent;
    std::vector<procon::MoveState> moves;
    procon::Point selected_agent_grid;
    bool is_moving_agent = false;
    std::vector<int> is_delete;
    bool manual_team = false;
    const double margin = 2;

    bool is_strategy = true;
    bool show_minimap = false;
    std::vector<std::vector<bool>> strategy;

    const QColor font_color = QColor(0, 0, 0, 64);
    const QColor background_color = QColor(245, 245, 220);
    const QColor grid_color = QColor(220, 220, 180);
    const QColor score_color = QColor(250, 80, 80, 120);
    const QColor strategy_color = QColor(0, 200, 170);
    const std::array<QColor, 2> strategy_grid_color = {QColor(200, 200, 200), QColor(100, 200, 170)};
    const std::array<QColor, 5> heatmap_color = {QColor(0, 0, 255), QColor(100, 150, 255), QColor(100, 255, 100), QColor(255, 255, 0), QColor(255, 0, 0)};
    const QColor checked_color_a = QColor(255,120,0);
    const QColor checked_color_b = QColor(0,120,255);
    const std::array<QColor, 2> team_colors = {QColor(255, 0, 0), QColor(0, 0, 255)};
    const std::array<QColor, 2> team_colors_deep = {QColor(100, 0, 0), QColor(0, 0, 100)};
    const std::array<QColor, 2> team_colors_pastel = {QColor(255, 100, 100), QColor(100, 100, 255)};
};

#endif // VISUALIZER_H
