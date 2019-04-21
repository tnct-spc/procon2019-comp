#include "visualizer.h"
#include "ui_visualizer.h"

Visualizer::Visualizer(std::shared_ptr<const procon::Field> field, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Visualizer),
    field(field)
{
    ui->setupUi(this);
}

Visualizer::~Visualizer()
{
    delete ui;
}

void Visualizer::setFieldPtr(std::shared_ptr<const procon::Field> field){
    this->field = field;
    this->update();
    this->repaint();
}

void Visualizer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);

    int window_width = this->width();
    int window_height = this->height();

    procon::Point size = field ? field->getSize() : procon::Point(10, 10);

    double grid_size = std::min(1.0 * window_width / ( (margin * 2) + size.x), 1.0 * window_height / ( (margin * 2) + size.y));
    double horizontal_margin = (window_width - grid_size * size.x) / 2;
    double vertical_margin = (window_height - grid_size * size.y) / 2;

    painter.setBrush(QBrush(background_color));
    painter.drawRect(0, 0, window_width, window_height);

    auto drawGrid = [&]{
        painter.setPen(QPen(QBrush(Qt::black), 0.5));

        for(int count = 1; count < size.x; ++count)
            painter.drawLine(horizontal_margin + grid_size * count, vertical_margin, horizontal_margin + grid_size * count, window_height - vertical_margin);

        for(int count = 1; count < size.y; ++count)
            painter.drawLine(horizontal_margin, vertical_margin + grid_size * count, window_width - horizontal_margin, vertical_margin + grid_size * count);
    };

    auto drawTiles = [&]{
        painter.setPen(QPen(QBrush(Qt::black), 0.3));
        for(int x_pos = 0; x_pos < size.x; ++x_pos)
            for(int y_pos = 0; y_pos < size.y; ++y_pos){

                if(field->getState(x_pos, y_pos).isEmpty() == false){
                    QColor paint_color = team_colors.at(field->getState(x_pos, y_pos).getDecrementedSide());
                    paint_color.setAlpha(64);

                    painter.setBrush(QBrush(paint_color));
                    painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
                }else{
                    QColor paint_color = grid_color;

                    painter.setBrush(QBrush(paint_color));
                    painter.drawRect(horizontal_margin + grid_size * x_pos, vertical_margin + grid_size * y_pos, grid_size, grid_size);
                }
            }
    };

    auto drawValues = [&]{

        QFont text_font;
        text_font.setPixelSize(grid_size * 0.5);
        painter.setFont(text_font);
        painter.setPen(QPen(font_color));

        for(int x_pos = 0; x_pos < size.x; ++x_pos)
            for(int y_pos = 0; y_pos < size.y; ++y_pos){
                int value = field->getState(x_pos, y_pos).value;

                QString text = QString::number(value);
                painter.drawText(horizontal_margin + grid_size * x_pos + (grid_size * 0.2), vertical_margin + grid_size * y_pos + ( grid_size * 0.7 ) , text);
            }

    };

    auto drawAgents = [&]{
        int agent_count = field->getAgentCount();
        for(int side = 0; side < 2; ++side){
            for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                QColor paint_color = team_colors.at(side);

                paint_color.setAlpha(128);
                painter.setBrush(QBrush(paint_color));

                const procon::Point& position = field->getAgent(side, agent_index);
                painter.drawEllipse(horizontal_margin + grid_size * (0.1 + position.x), vertical_margin + grid_size * (0.1 + position.y), 0.8 * grid_size, 0.8 * grid_size);
            }
        }
    };

    auto drawTurnCount = [&]{
        QPoint text_point;
        text_point.setX(horizontal_margin + (size.x - 3) * grid_size);
        text_point.setY(vertical_margin - 0.4 * grid_size);

        painter.setPen(QPen(QBrush(score_color), 0.3));

        QFont font = painter.font();
        font.setPointSize(grid_size * 0.8);
        painter.setFont(font);

        auto turn = field->getTurn();
        QString str(QString::number(turn.now) + QString::fromStdString(" / ") + QString::number(turn.final));
        painter.drawText(text_point, str);
    };

    auto drawScores = [&]{
        auto scores = field->getScores();

        QPoint side_0_point, side_1_point;
        side_0_point.setX(horizontal_margin);
        side_1_point.setX(window_width - horizontal_margin - grid_size * 3.5);
        side_0_point.setY(window_height - vertical_margin + grid_size * 1.3);
        side_1_point.setY(window_height - vertical_margin + grid_size * 1.3);

        QColor paint_color = team_colors.at(0);
        paint_color.setAlpha(100);
        painter.setPen(QPen(QBrush(paint_color), 0.3));
        painter.drawText(side_0_point, QString::number(scores[0].tile) + QString::fromStdString(" + ") + QString::number(scores[0].region));

        paint_color = team_colors.at(1);
        paint_color.setAlpha(100);
        painter.setPen(QPen(QBrush(paint_color), 0.3));
        painter.drawText(side_1_point, QString::number(scores[1].tile) + QString::fromStdString(" + ") + QString::number(scores[1].region));
    };

    if(field){
        drawGrid();
        drawTiles();
        drawValues();
        drawAgents();
        drawTurnCount();
        drawScores();
    }
}

void Visualizer::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_R)
        emit signalResetField();
    if(event->key() == Qt::Key_S)
        emit signalRunSimulator();
    if(event->key() == Qt::Key_F)
        emit signalRunFullSimulation();
    if(event->key() == Qt::Key_N)
        emit signalSimulateNextTurn();
    if(event->key() == Qt::Key_I)
        emit signalReverseField();
    if(event->key() == Qt::Key_E){
        procon::csv::csvExport(QFileDialog::getSaveFileName(this, tr("Save CSV")).toStdString(), *field);
    }
}
