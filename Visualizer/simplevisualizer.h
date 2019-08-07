#ifndef SIMPLEVISUALIZER_H
#define SIMPLEVISUALIZER_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QFileDialog>
#include <memory>
#include "csvtranslate.h"

namespace Ui {
class SimpleVisualizer;
}

class SimpleVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleVisualizer(std::shared_ptr<const procon::Field> field = nullptr, QWidget *parent = nullptr);
    ~SimpleVisualizer();

    void setFieldPtr(std::shared_ptr<const procon::Field> field);

private:

    void paintEvent(QPaintEvent* event);

    Ui::SimpleVisualizer *ui;
    std::shared_ptr<const procon::Field> field;

    const double margin = 2;

    const QColor font_color = QColor(0, 0, 0, 64);
    const QColor background_color = QColor(245, 245, 220);
    const QColor grid_color = QColor(220, 220, 180);
    const QColor score_color = QColor(250, 80, 80, 120);
    const std::array<QColor, 2> team_colors = {QColor(255, 0, 0), QColor(0, 0, 255)};
};

#endif // SIMPLEVISUALIZER_H
