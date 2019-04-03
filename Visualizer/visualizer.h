#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include "field.h"

namespace Ui {
class Visualizer;
}

class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(const procon::Field& field, QWidget *parent = nullptr);
    ~Visualizer();

private:

    void paintEvent(QPaintEvent* event);


    Ui::Visualizer *ui;
    const procon::Field& field;

    const double margin = 2;

    const QColor font_color = QColor(0, 0, 0, 64);
    const QColor background_color = QColor(245, 245, 220);
    const QColor grid_color = QColor(220, 220, 180);
    const std::array<QColor, 2> team_colors = {QColor(255, 0, 0), QColor(0, 0, 255)};
};

#endif // VISUALIZER_H
