#include "visualizer.h"
#include "ui_visualizer.h"

Visualizer::Visualizer(std::shared_ptr<const procon::Field> field, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Visualizer),
    field(field)
{
    ui->setupUi(this);
    is_delete = std::vector<int>(field->getAgentCount());
    move_agent = std::vector<procon::Point>(field->getAgentCount(),{-1,-1});
    strategy = std::vector<std::vector<bool>>(field->getSize().x,std::vector<bool>(field->getSize().y));
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

void Visualizer::mousePressEvent(QMouseEvent *event){
    agent_count = field->getAgentCount();
    unsigned int grid_x = field->getSize().x;
    unsigned int grid_y = field->getSize().y;
    QPointF point = event->pos();
    if(auto_mode == false){
        if ((point.x() < horizontal_margin) || (point.x() > window_width - horizontal_margin)
                        || (point.y() < vertical_margin) || (point.y() > window_height - vertical_margin)) {
            return;
        }
        bool right_flag = (event->button() == Qt::RightButton);

        // クリックされたグリッド座標を保存
        procon::Point clicked_grid;

        // xを座標からマスへ
        clicked_grid.x = (point.x() - horizontal_margin) / grid_size;

        // yを座標からマスへ
        clicked_grid.y = (point.y() - vertical_margin) / grid_size;
        // 移動を入力するエージェントが選ばれているか
        if(selected){
            std::vector<std::vector<procon::Point>> agents (2,std::vector<procon::Point>(agent_count));
            checkClickGrid(clicked_grid,right_flag);
        }
        else {
            // クリックされたエージェントまたはマスを照合
            checkClickedAgent(clicked_grid);
        }
        bool finished_move = true;
        for(int i = 0;i < agent_count;i++){
            if(move_agent[i].x == -1 || move_agent[i].y == -1){
                finished_move = false;
            }
        }
    }
    else if(is_strategy == true){
        if ((point.x() < horizontal_margin) || (point.x() > window_width - horizontal_margin)
                        || (point.y() < vertical_margin) || (point.y() > window_height - vertical_margin)) {
            return;
        }
        bool right_flag = (event->button() == Qt::RightButton);
        procon::Point clicked_grid;
        clicked_grid.x = (point.x() - horizontal_margin) / grid_size;
        clicked_grid.y = (point.y() - vertical_margin) / grid_size;
        strategy[clicked_grid.x][clicked_grid.y] = !strategy[clicked_grid.x][clicked_grid.y];
        emit signalStrategy(strategy);
        this->update();
        this->repaint();
    }
}

// クリックされたエージェントまたはマスを照合
void Visualizer::checkClickedAgent(procon::Point mass)
{
    // Fieldから現在のエージェントの位置を取得
    std::vector<std::vector<procon::Point>> agents {2,std::vector<procon::Point>(field->getAgentCount(),{-1,-1})};
    for(int i = 0;i < 2;i++){
        for(int j = 0;j < agent_count;j++){
            agents[i][j] = field->getAgent(i, j);//ここにgetAgentが入る予定です
        }
    }

    // クリックされたマスにエージェントがいるのか確認
    for (int agent = 0; agent < agent_count; agent++) {
        // クリックされたマスとエージェントの位置が一致したら、チームとエージェントの番号を返す
        if (mass == agents.at(manual_team).at(agent)) {

            // クリックされたエージェントがすでに変更済みなら何もしない
            // if (decided_agents.at(team).at(agent)) return;

            selected_agent.first = manual_team;
            selected_agent.second = agent;

            // 移動を入力するエージェントのグリッド座標を保存
            selected_agent_grid = mass;

            // エージェントが選択されたことを記録
            selected = true;

            // 選択されたエージェントの移動可能範囲を表示
            this->update();
        }
    }
}

// エージェントの移動先を決定
void Visualizer::checkClickGrid(procon::Point mass, bool right_flag)
{
    // クリックされた場所がエージェントの移動範囲に含まれているか確認
    if (((selected_agent_grid.x - 1) > mass.x) || ((selected_agent_grid.x + 1) < mass.x)
            || ((selected_agent_grid.y - 1) > mass.y) || ((selected_agent_grid.y + 1) < mass.y)) {
        // 選択を解除
        selected = false;

        // すでに登録されている場合リセット
        if (move_agent.at(selected_agent.second).x != -1) {

            move_agent.at(selected_agent.second) = {-1, -1};
            confirm_count--;

        }

        // 初期化
        selected_agent = std::make_pair(INT_MAX, INT_MAX);
        selected_agent_grid = {INT_MAX, INT_MAX};

        this->update();

        return;
    }

    //もう既に選択されている場合は値を増やさない
    if(move_agent.at(selected_agent.second).x == -1)
        ++confirm_count;

    // 移動先を記録
    move_agent.at(selected_agent.second) = mass;
    is_delete.at(selected_agent.second) = right_flag;

    // エージェントの移動先が決定済みであることを記録
    // decided_agents.at(selected_agent.first).at(selected_agent.second) = true;

    // エージェントの選択を解除
    selected = false;

    // windowの描きかえ
    this->update();


    if(confirm_count == agent_count){

        confirm_count = 0;

        std::vector<procon::Point> return_val = std::move(move_agent);
        std::vector<int> return_delete_flag = std::move(is_delete);

        move_agent = std::vector<procon::Point>(field->getAgentCount(),{-1,-1});

        is_delete = std::vector<int>(field->getAgentCount());
        auto_mode = true;
        emit signalMoveAgents(return_val, return_delete_flag, manual_team);
    }
}

void Visualizer::resetAgentAct(){
    is_delete = std::vector<int>(field->getAgentCount());
    move_agent = std::vector<procon::Point>(field->getAgentCount(),{-1,-1});
    confirm_count = 0;
    selected = false;
    auto_mode = true;
    agent_count = field->getAgentCount();
    this->update();
    this->repaint();
}

void Visualizer::resetStrategy(bool setState){
    for(int x = 0;x < field->getSize().x;x++){
        for(int y = 0;y < field->getSize().y;y++){
            strategy[x][y] = setState;
        }
    }
}

void Visualizer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    window_width = this->width();
    window_height = this->height();

    procon::Point size = field ? field->getSize() : procon::Point(10, 10);

    grid_size = std::min(1.0 * window_width / ( (margin * 2) + size.x), 1.0 * window_height / ( (margin * 2) + size.y));
    horizontal_margin = (window_width - grid_size * size.x) / 2;
    vertical_margin = (window_height - grid_size * size.y) / 2;

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
                QColor paint_color;
                paint_color = team_colors.at(side);
                if(side == manual_team && move_agent[agent_index].x != -1){
                    paint_color = team_colors_pastel.at(side);
                }
                if(selected_agent.first == side && selected_agent.second == agent_index && selected){
                    paint_color = team_colors_deep.at(side);
                }
                paint_color.setAlpha(128);
                painter.setBrush(QBrush(paint_color));

                const procon::Point& position = field->getAgent(side, agent_index);
                painter.drawEllipse(horizontal_margin + grid_size * (0.1 + position.x), vertical_margin + grid_size * (0.1 + position.y), 0.8 * grid_size, 0.8 * grid_size);
            }
        }
    };

    auto drawAgentMove = [&]{

        painter.setPen(QPen(QBrush(Qt::black), 0.3));

        for(unsigned int index = 0; index < move_agent.size(); ++index){

            int pos_x = move_agent.at(index).x;
            int pos_y = move_agent.at(index).y;

            if(pos_x == -1)continue;

            QColor paint_color = ( manual_team == 0
                                   ? checked_color_a
                                   : checked_color_b);

            paint_color.setAlpha(120);
            if(is_delete.at(index) || (move_agent.at(index).x < field->getSize().x && move_agent.at(index).y < field->getSize().y && field->getState(move_agent.at(index)).tile == (manual_team == 0 ? 2 : 1)))
                paint_color.setAlpha(200);

            painter.setBrush(QBrush(paint_color));

            painter.drawRect(horizontal_margin + grid_size * (0.1 + pos_x), vertical_margin + grid_size * (0.1 + pos_y), 0.8 * grid_size, 0.8 * grid_size);
        }
    };

    // 選択されたエージェントの移動可能先を塗る
    auto drawAroundAgent = [&] {

        // チームごとに配色を変える
        QColor paint_color = ( selected_agent.first == 0
                               ? checked_color_a
                               : checked_color_b);
        paint_color.setAlpha(100);

        painter.setBrush(QBrush(paint_color));

        // フィールドをはみ出さない範囲で、エージェントの移動可能領域を描く
        for (int x = -1; x < 2; x++) {

            int draw_x = selected_agent_grid.x + x;

            if ((draw_x >= 0) && ((unsigned int)draw_x < field->getSize().x) && selected) {

                for (int y = -1; y < 2; y++) {

                    int draw_y = selected_agent_grid.y + y;

                    if ((draw_y >= 0) && ((unsigned int)draw_y < field->getSize().y)) {

                        painter.drawRect(horizontal_margin + draw_x * grid_size, vertical_margin + draw_y * grid_size, grid_size, grid_size);
                    }
                }
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

    auto drawAutomode = [&]{
        QPoint text_point;
        text_point.setX(horizontal_margin);
        text_point.setY(vertical_margin - 0.4 * grid_size);

        painter.setPen(QPen(QBrush(team_colors[manual_team]), 0.3));

        QFont font = painter.font();
        font.setPointSize(grid_size * 0.8);
        painter.setFont(font);

        QString str(QString::fromStdString("Manual Mode"));
        if(!auto_mode)painter.drawText(text_point, str);
    };

    auto drawStrategyGrid = [&]{
        for(int x = 0;x < field->getSize().x;x++){
            for(int y = 0;y < field->getSize().y;y++){
                QColor paint_color = ( strategy[x][y] == true
                                       ? strategy_grid_color[1]
                                       : strategy_grid_color[0]);
                paint_color.setAlpha(100);
                painter.setBrush(QBrush(paint_color));
                painter.drawRect(horizontal_margin + x * grid_size, vertical_margin + y * grid_size, grid_size, grid_size);
            }
        }
    };

    auto drawStrategymode = [&]{
        if(is_strategy){
            QPoint text_point;
            text_point.setX(horizontal_margin);
            text_point.setY(vertical_margin - 0.4 * grid_size);

            painter.setPen(QPen(QBrush(strategy_color), 0.3));

            QFont font = painter.font();
            font.setPointSize(grid_size * 0.8);
            painter.setFont(font);

            QString str(QString::fromStdString("Strategy Mode"));
            painter.drawText(text_point, str);
            drawStrategyGrid();
        }
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
        drawAgentMove();
        drawAroundAgent();
        drawTurnCount();
        drawAutomode();
        drawScores();
        drawStrategymode();
    }
}

void Visualizer::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_R){
        if(!is_strategy){
            emit signalResetField();
            resetAgentAct();//emitしてからresetしないとagent_countをreset前に合わせて配列外参照してしまうので下に書いている
        }
        else{
            resetStrategy(false);
            this->update();
            this->repaint();
        }
    }
    if(event->key() == Qt::Key_S){
        if(!is_strategy){
            resetAgentAct();
            emit signalRunSimulator();
        }
    }
    if(event->key() == Qt::Key_F){
        if(!is_strategy){
            emit signalRunFullSimulation();
        }
    }
    if(event->key() == Qt::Key_N){
        if(!is_strategy){
            resetAgentAct();
            emit signalSimulateNextTurn();
        }
    }
    if(event->key() == Qt::Key_I){
        if(!is_strategy){
            resetAgentAct();
            emit signalReverseField();
        }
    }
    if(event->key() == Qt::Key_E){
        if(!is_strategy){
            procon::csv::csvExport(QFileDialog::getSaveFileName(this, tr("Save CSV")).toStdString(), *field);
        }
    }
    if(event->key() == Qt::Key_A){
        if(!is_strategy){
            if(field->getTurn().now != field->getTurn().final){
                if(!auto_mode && confirm_count==0 && !selected)manual_team = !manual_team;
                auto_mode = false;
                this->update();
                this->repaint();
            }
        }
        else{
            resetStrategy(true);
            this->update();
            this->repaint();
        }
    }
    if(event->key() == Qt::Key_T){
        resetAgentAct();
        resetStrategy(false);
        is_strategy = !is_strategy;
        this->update();
        this->repaint();
    }
}
