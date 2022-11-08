#include "AI.h"
#include <QTime>
#include <QtGlobal>

/* ------------ AI_UI -------------- */

AI::AI(QWidget *parent)
	: QWidget(parent)
{
    ui = new Ui::AI;
    ui->setupUi(this);
    AI_settings = new AI_Settings(this);
    // wipe off frame
    this->setWindowFlags(Qt::FramelessWindowHint);
    Manual_Initialize(true);

    /* Animation_Button */
    // Icon
    icon_Traceback.addFile(tr(":/Resource2/UI_Design/AI_Traceback.png"));
    icon_Regame.addFile(tr(":/Resource2/UI_Design/AI_Regame.png"));
    icon_Rechoose.addFile(tr(":/Resource2/UI_Design/AI_Rechoose.png"));
    // Traceback
    Traceback = new QAnimationButton(this);
    Traceback->SizeSet(1);
    Traceback->setGeometry(1643, 852, 142, 56);
    Traceback->setIcon(icon_Traceback);
    Traceback->setIconSize(QSize(142, 56));
    Traceback->setFlat(true);
    connect(Traceback, SIGNAL(clicked()), this, SLOT(on_Traceback_clicked()));
    // Regame
    Regame = new QAnimationButton(this);
    Regame->SizeSet(1);
    Regame->setGeometry(724, 950, 182, 56);
    Regame->setIcon(icon_Regame);
    Regame->setIconSize(QSize(182, 56));
    Regame->setFlat(true);
    connect(Regame, SIGNAL(clicked()), this, SLOT(on_Regame_clicked()));
    // Rechoose
    Rechoose = new QAnimationButton(this);
    Rechoose->SizeSet(1);
    Rechoose->setGeometry(1019, 950, 182, 56);
    Rechoose->setIcon(icon_Rechoose);
    Rechoose->setIconSize(QSize(182, 56));
    Rechoose->setFlat(true);
    connect(Rechoose, SIGNAL(clicked()), this, SLOT(on_Rechoose_clicked()));
    
}

AI::~AI()
{
    delete board;
    delete UI;
}


void AI::Manual_Initialize(bool first)
{
    if (!first)
    {
        if(board)
            delete board;
        if (UI)
            delete UI;
    }
    board = new Board;
    Start = false;
    UI = new QPixmap();
    UI->load(":/Resource/UI_Design/Board_UI.png");
    update();
}

void AI::Launch(void)
{
    Traceback->hide();
    Regame->hide();
    Rechoose->hide();
    AI_settings->exec();
    if (AI_settings->cancel)
    {
        AI_hide();
    }
    else
    {
        /* Settings */
        Traceback->show();
        Traceback->setEnabled(false);
        trace_chance = 3;
        ui->Trace_Inf->setText(QString::fromLocal8Bit("共剩余三次机会"));
        ui->Trace_Inf->setStyleSheet("color:black;background:rgb(247,248,250)");
        AI_fun.setColor(!AI_settings->order_isFirst);
        ui->name_1->setText(AI_settings->challenger_name);
        if (AI_fun.setColor() == color_white)
        {
            ui->playerInf_1->setText(QString::fromLocal8Bit("AI挑战者 (黑)"));
            ui->playerInf_2->setText(QString::fromLocal8Bit("AI (白)"));
            ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
            ui->pieceInf_1->setStyleSheet("color:red");
            ui->pieceInf_2->setStyleSheet("color:black");
        }
        else
        {
            ui->playerInf_1->setText(QString::fromLocal8Bit("AI挑战者 (白)"));
            ui->playerInf_2->setText(QString::fromLocal8Bit("AI (黑)"));
            ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到AI方落子"));
            ui->pieceInf_2->setStyleSheet("color:red");
            ui->pieceInf_1->setStyleSheet("color:black");
        }
        /* check if AI first */
        if (board->GetPlayer() == (bool)AI_fun.setColor())
        {
            Piece** tmp_pieces = board->GetPieces();
            /* Random Start */
            qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
            int rand = qrand() % 10;
            if (rand <= 1)
                board->GetMove(6, 7);   // 20 percents
            else if (rand <= 3)
                board->GetMove(7, 6);
            else if (rand <= 5)
                board->GetMove(8, 7);
            else if (rand <= 7)
                board->GetMove(7, 8);
            else
                board->GetMove(7, 7);
            update();
            /* Traceback set */
            Traceback->setEnabled(false);
            // INF_card update(after AI)
            ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
            ui->pieceInf_1->setStyleSheet("color:red");
            ui->pieceInf_2->setStyleSheet("color:black");
        }
        Start = true;
    }
}

void AI::AI_hide()
{
    hide();
    emit M1_hide();
}

// back
void AI::on_back_button_clicked()
{
    AI_hide();
    Manual_Initialize();
}

// regame
void AI::on_Regame_clicked()
{
    Regame->hide();
    Rechoose->hide();
    Manual_Initialize();
    /* color set */
    Traceback->setEnabled(false);
    trace_chance = 3;
    ui->Trace_Inf->setText(QString::fromLocal8Bit("共剩余三次机会"));
    ui->Trace_Inf->setStyleSheet("color:black;background:rgb(247,248,250)");
    AI_fun.setColor(!AI_settings->order_isFirst);
    if (AI_fun.setColor() == color_white)
    {
        ui->playerInf_1->setText(QString::fromLocal8Bit("AI挑战者 (黑)"));
        ui->playerInf_2->setText(QString::fromLocal8Bit("AI (白)"));
        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:black");
    }
    else
    {
        ui->playerInf_1->setText(QString::fromLocal8Bit("AI挑战者 (白)"));
        ui->playerInf_2->setText(QString::fromLocal8Bit("AI (黑)"));
        ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到AI方落子"));
        ui->pieceInf_1->setStyleSheet("color:black");
        ui->pieceInf_2->setStyleSheet("color:red");
    }
    /* check if AI first */
    if (board->GetPlayer() == (bool)AI_fun.setColor())
    {
        Piece** tmp_pieces = board->GetPieces();
        /* Random Start */
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        int rand = qrand() % 10;
        if (rand <= 1)
            board->GetMove(6, 7);   // 20 percents
        else if (rand <= 3)
            board->GetMove(7, 6);
        else if (rand <= 5)
            board->GetMove(8, 7);
        else if (rand <= 7)
            board->GetMove(7, 8);
        else
            board->GetMove(7, 7);
        update();
        // INF_card update(after AI)
        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:black");
    }
    Start = true;
}

// rechoose
void AI::on_Rechoose_clicked()
{
    Manual_Initialize();
    hide();
    show();
    Launch();
}

// traceback
void AI::on_Traceback_clicked()
{
    board->Traceback();
    board->Traceback();
    update();
    trace_chance--;
    if (trace_chance == 2)
    {
        Traceback->setEnabled(true);
        ui->Trace_Inf->setText(QString::fromLocal8Bit("还剩余两次机会"));
        ui->Trace_Inf->setStyleSheet("color:black;background:rgb(247,248,250)");
    }
    if (trace_chance == 1)
    {
        Traceback->setEnabled(true);
        ui->Trace_Inf->setText(QString::fromLocal8Bit("仅剩余一次机会"));
        ui->Trace_Inf->setStyleSheet("color:black;background:rgb(247,248,250)");
    }
    else if (trace_chance == 0)
    {
        Traceback->setEnabled(false);
        ui->Trace_Inf->setText(QString::fromLocal8Bit("悔棋机会已用完"));
        ui->Trace_Inf->setStyleSheet("color:grey;background:rgb(247,248,250)");
    }
    if(board->GetPiecesCount() <= 1)
        Traceback->setEnabled(false);   // not empty
}


void AI::mousePressEvent(QMouseEvent* event)
{
	if (Start && event->button() == Qt::LeftButton)  // Left_Press
	{
        static int press_x = 0, press_y = 0, standard_x = 0, standard_y = 0, put_column = 0, put_row = 0;
        press_x = event->x();
        press_y = event->y();
		if (board->GetPlayer() == (bool)!(AI_fun.setColor()) && press_x >= board->GetLeftMargin() - board->GetRange() && press_x <= board->GetLeftMargin() + board->GetLength() + board->GetRange() && press_y >= board->GetTopMargin() - board->GetRange() && press_y <= board->GetTopMargin() + board->GetWidth() + board->GetRange())
		{
            standard_x = press_x - board->GetLeftMargin() + board->GetSize() / 2;
            standard_y = press_y - board->GetTopMargin() + board->GetSize() / 2;
			if (standard_x % board->GetSize() >= board->GetSize() / 2 - board->GetRange() && standard_x % board->GetSize() <= board->GetSize() / 2 + board->GetRange() && standard_y % board->GetSize() >= board->GetSize() / 2 - board->GetRange() && standard_y % board->GetSize() <= board->GetSize() / 2 + board->GetRange())   // allow to click
			{
                put_column = standard_x / board->GetSize();
                put_row = standard_y / board->GetSize();
                if (board->GetColor(put_row, put_column) != status_none)
                {
                    // NOT VALID
                }
                else
                {
                    board->GetMove(put_row, put_column);    // move
                    // INF_card update(after gamer)
                    ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
                    ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到AI方落子"));
                    ui->pieceInf_1->setStyleSheet("color:black");
                    ui->pieceInf_2->setStyleSheet("color:red");
                    repaint();
                    /* Traceback set */
                    Traceback->setEnabled(false);
                    /* Judge deadlock */
                    if (board->Deadlock())
                    {
                        ui->pieceInf_1->setText(QString::fromLocal8Bit("双方进入僵局"));
                        ui->pieceInf_2->setText(QString::fromLocal8Bit("双方进入僵局"));
                        ui->pieceInf_1->setStyleSheet("color:red");
                        ui->pieceInf_2->setStyleSheet("color:red");
                        Regame->show();
                        Rechoose->show();
                        Traceback->setEnabled(false);
                        Start = false;
                        return;
                    }
                    /* Judge gamer vitory */
                    if (board->obvious_Victory())
                    {
                        ui->pieceInf_1->setText(QString::fromLocal8Bit("恭喜您战胜了AI"));
                        ui->pieceInf_2->setText(QString::fromLocal8Bit("请不要告诉我的主人"));
                        ui->pieceInf_1->setStyleSheet("color:red");
                        ui->pieceInf_2->setStyleSheet("color:red");
                        Regame->show();
                        Rechoose->show();
                        Traceback->setEnabled(false);
                        Start = false;
                    }
                    else
                    {
                        /* AI order */
                        Piece** tmp_pieces = board->GetPieces();
                        int* final_decision = AI_fun.get_Decision(tmp_pieces, (AI_settings->difficulty + 1) * 2);
                        board->GetMove(final_decision[0], final_decision[1]);
                        // INF_card update(after AI)
                        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
                        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
                        ui->pieceInf_1->setStyleSheet("color:red");
                        ui->pieceInf_2->setStyleSheet("color:black");
                        update();
                        /* Traceback set */
                        if(trace_chance)
                            Traceback->setEnabled(true);
                        /* Judge deadlock */
                        if (board->Deadlock())
                        {
                            ui->pieceInf_1->setText(QString::fromLocal8Bit("双方进入僵局"));
                            ui->pieceInf_2->setText(QString::fromLocal8Bit("双方进入僵局"));
                            ui->pieceInf_1->setStyleSheet("color:red");
                            ui->pieceInf_2->setStyleSheet("color:red");
                            Regame->show();
                            Rechoose->show();
                            Traceback->setEnabled(false);
                            Start = false;
                            return;
                        }
                        /* Judge AI vitory */
                        if (board->obvious_Victory())
                        {
                            if (board->GetPlayer())
                                ui->pieceInf_1->setText(QString::fromLocal8Bit("您已被AI战胜"));
                            ui->pieceInf_2->setText(QString::fromLocal8Bit("主人传话让你再努力"));
                            ui->pieceInf_1->setStyleSheet("color:red");
                            ui->pieceInf_2->setStyleSheet("color:red");
                            Regame->show();
                            Rechoose->show();
                            Traceback->setEnabled(false);
                            Start = false;
                        }
                        /* Judge if near deadlock */
                        if (board->GetPiecesCount() == 150 || board->GetPiecesCount() == 151)
                        {
                            ui->pieceInf_1->setText(QString::fromLocal8Bit("从现在起 可随时重开"));
                            ui->pieceInf_2->setText(QString::fromLocal8Bit("从现在起 可随时重开"));
                            ui->pieceInf_1->setStyleSheet("color:red");
                            ui->pieceInf_2->setStyleSheet("color:red");
                            Regame->show();
                            Rechoose->show();
                        }
                    }
                }
            }
		}
	}
}


/* Paint Piece */
void AI::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // anti-aliasing
    painter.drawPixmap(this->rect(), *UI);
    QPen pen; // width adjustment
    pen.setWidth(2);
    painter.setPen(pen);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // Draw Pieces
    for (int row = 0; row <= board->GetWidth() / board->GetSize(); row++)
        for (int column = 0; column <= board->GetLength() / board->GetSize(); column++)
        {
            if (board->GetColor(row, column) == (int)color_white)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(board->GetLeftMargin() + board->GetSize() * column - 11.25, board->GetTopMargin() + board->GetSize() * row - 11.25, 22.5, 22.5);
            }
            else if (board->GetColor(row, column) == (int)color_black)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(board->GetLeftMargin() + board->GetSize() * column - 11.25, board->GetTopMargin() + board->GetSize() * row - 11.25, 22.5, 22.5);
            }
        }
    // Win line
    if (board->GetWinPiece()[0] != status_none)
    {
        int method = board->GetWinPiece()[0];
        int r = board->GetWinPiece()[1];
        int c = board->GetWinPiece()[2];
        pen.setWidth(5);
        pen.setColor(Qt::red);
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
        painter.setPen(pen);
        if(method == 0)
            painter.drawLine(board->GetLeftMargin() + board->GetSize() * c, board->GetTopMargin() + board->GetSize() * r, board->GetLeftMargin() + board->GetSize() * (c + 4), board->GetTopMargin() + board->GetSize() * r);
        else if (method == 1)
            painter.drawLine(board->GetLeftMargin() + board->GetSize() * c, board->GetTopMargin() + board->GetSize() * r, board->GetLeftMargin() + board->GetSize() * c, board->GetTopMargin() + board->GetSize() * (r + 4));
        else if (method == 2)
            painter.drawLine(board->GetLeftMargin() + board->GetSize() * c, board->GetTopMargin() + board->GetSize() * r, board->GetLeftMargin() + board->GetSize() * (c + 4), board->GetTopMargin() + board->GetSize() * (r + 4));
        else
            painter.drawLine(board->GetLeftMargin() + board->GetSize() * c, board->GetTopMargin() + board->GetSize() * r, board->GetLeftMargin() + board->GetSize() * (c + 4), board->GetTopMargin() + board->GetSize() * (r - 4));
    }
    else
    {
        // last_piece mark
        if (board->GetPiecesCount() > 10)
        {
            if (board->GetPlayer() == color_black)
                pen.setColor(Qt::black);
            else
                pen.setColor(Qt::white);
            painter.setPen(pen);
            brush.setStyle(Qt::SolidPattern);
            if (board->GetPlayer() == color_black)
                brush.setColor(Qt::black);
            else
                brush.setColor(Qt::white);
            painter.setBrush(brush);
            QPoint last_point = board->GetPieceStack().top();
            painter.drawEllipse(board->GetLeftMargin() + board->GetSize() * last_point.y() - 2.5, board->GetTopMargin() + board->GetSize() * last_point.x() - 2.5, 5, 5);
        }
    }
}





/* -------------- AI_Evaluation -------------- */

Evaluation::Evaluation(void)
{
    memset(chessTemplate_tuple, 0, sizeof(chessTemplate_tuple));    // Initialize
    // win_Form
    chessTemplate_tuple[2][2][2][2][2][2] = win_Form;
    chessTemplate_tuple[2][2][2][2][2][0] = win_Form;
    chessTemplate_tuple[0][2][2][2][2][2] = win_Form;
    chessTemplate_tuple[2][2][2][2][2][1] = win_Form;
    chessTemplate_tuple[1][2][2][2][2][2] = win_Form;
    chessTemplate_tuple[3][2][2][2][2][2] = win_Form; // border
    chessTemplate_tuple[2][2][2][2][2][3] = win_Form;
    // lose_Form
    chessTemplate_tuple[1][1][1][1][1][1] = lose_Form;
    chessTemplate_tuple[1][1][1][1][1][0] = lose_Form;
    chessTemplate_tuple[0][1][1][1][1][1] = lose_Form;
    chessTemplate_tuple[1][1][1][1][1][2] = lose_Form;
    chessTemplate_tuple[2][1][1][1][1][1] = lose_Form;
    chessTemplate_tuple[3][1][1][1][1][1] = lose_Form;
    chessTemplate_tuple[1][1][1][1][1][3] = lose_Form;
    // self_flex4_From
    chessTemplate_tuple[0][2][2][2][2][0] = self_flex4_Form;
    // op_flex4_From
    chessTemplate_tuple[0][1][1][1][1][0] = op_flex4_Form;
    // self_flex3_Form
    chessTemplate_tuple[0][2][2][2][0][0] = self_flex3_Form;
    chessTemplate_tuple[0][0][2][2][2][0] = self_flex3_Form;
    chessTemplate_tuple[0][2][0][2][2][0] = self_flex3_Form;
    chessTemplate_tuple[0][2][2][0][2][0] = self_flex3_Form;
    // op_flex3_Form
    chessTemplate_tuple[0][1][1][1][0][0] = op_flex3_Form;
    chessTemplate_tuple[0][0][1][1][1][0] = op_flex3_Form;
    chessTemplate_tuple[0][1][0][1][1][0] = op_flex3_Form;
    chessTemplate_tuple[0][1][1][0][1][0] = op_flex3_Form;
    // self_flex2_Form
    chessTemplate_tuple[0][2][2][0][0][0] = self_flex2_Form;
    chessTemplate_tuple[0][2][0][2][0][0] = self_flex2_Form;
    chessTemplate_tuple[0][2][0][0][2][0] = self_flex2_Form;
    chessTemplate_tuple[0][0][2][2][0][0] = self_flex2_Form;
    chessTemplate_tuple[0][0][2][0][2][0] = self_flex2_Form;
    chessTemplate_tuple[0][0][0][2][2][0] = self_flex2_Form;
    // op_flex2_Form
    chessTemplate_tuple[0][1][1][0][0][0] = op_flex2_Form;
    chessTemplate_tuple[0][1][0][1][0][0] = op_flex2_Form;
    chessTemplate_tuple[0][1][0][0][1][0] = op_flex2_Form;
    chessTemplate_tuple[0][0][1][1][0][0] = op_flex2_Form;
    chessTemplate_tuple[0][0][1][0][1][0] = op_flex2_Form;
    chessTemplate_tuple[0][0][0][1][1][0] = op_flex2_Form;
    // self_flex1_Form
    chessTemplate_tuple[0][2][0][0][0][0] = self_flex1_Form;
    chessTemplate_tuple[0][0][2][0][0][0] = self_flex1_Form;
    chessTemplate_tuple[0][0][0][2][0][0] = self_flex1_Form;
    chessTemplate_tuple[0][0][0][0][2][0] = self_flex1_Form;
    // op_flex1_Form
    chessTemplate_tuple[0][1][0][0][0][0] = op_flex1_Form;
    chessTemplate_tuple[0][0][1][0][0][0] = op_flex1_Form;
    chessTemplate_tuple[0][0][0][1][0][0] = op_flex1_Form;
    chessTemplate_tuple[0][0][0][0][1][0] = op_flex1_Form;

    int loc_1, loc_2, loc_3, loc_4, loc_5, loc_6;
    int left5_op, left5_self, right5_op, right5_self; // side5_color -> Piece_num of this color among 5 pieces(counted from left/right side)
    for (loc_1 = 0; loc_1 < 4; ++loc_1) 
        for (loc_2 = 0; loc_2 < 3; ++loc_2) 
            for (loc_3 = 0; loc_3 < 3; ++loc_3) 
                for (loc_4 = 0; loc_4 < 3; ++loc_4) 
                    for (loc_5 = 0; loc_5 < 3; ++loc_5) 
                        for (loc_6 = 0; loc_6 < 4; ++loc_6)
                        {
                            left5_op = left5_self = right5_op = right5_self = 0;

                            if (loc_1 == 1)
                                left5_op++;
                            else if (loc_1 == 2)
                                left5_self++;

                            if (loc_2 == 1) 
                            { 
                                left5_op++; 
                                right5_op++; 
                            }
                            else if (loc_2 == 2) 
                            {
                                left5_self++; 
                                right5_self++; 
                            }

                            if (loc_3 == 1) 
                            { 
                                left5_op++; 
                                right5_op++; 
                            }
                            else if (loc_3 == 2) 
                            { 
                                left5_self++; 
                                right5_self++; 
                            }

                            if (loc_4 == 1)
                            {
                                left5_op++; 
                                right5_op++;
                            }
                            else if (loc_4 == 2)
                            { 
                                left5_self++; 
                                right5_self++; 
                            }

                            if (loc_5 == 1) 
                            {
                                left5_op++;
                                right5_op++; 
                            }
                            else if (loc_5 == 2) 
                            { 
                                left5_self++; 
                                right5_self++; 
                            }

                            if (loc_6 == 1)
                                right5_op++;
                            else if (loc_6 == 2)
                                right5_self++;

                            // border exists
                            if (loc_1 == 3 || loc_6 == 3)
                            {
                                // left border
                                if (loc_1 == 3 && loc_6 != 3)
                                {
                                    // self_block4_Form
                                    if (right5_op == 0 && right5_self == 4) // even though right exits flex4, weight of flex4 is far beyond block4
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block4_Form;

                                    // op_block4_Form
                                    if (right5_op == 4 && right5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block4_Form;

                                    // self_block3_Form
                                    if (right5_op == 0 && right5_self == 3)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block3_Form;

                                    // op_block3_Form
                                    if (right5_op == 3 && right5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block3_Form;

                                    // self_block2_Form
                                    if (right5_op == 0 && right5_self == 2)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block2_Form;

                                    // op_block2_Form
                                    if (right5_op == 2 && right5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block2_Form;
                                }
                                // right border
                                else if (loc_6 == 3 && loc_1 != 3)
                                {
                                    // self_block4_Form
                                    if (left5_op == 0 && left5_self == 4)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block4_Form;

                                    // op_block4_Form
                                    if (left5_op == 4 && left5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block4_Form;

                                    // self_block3_Form
                                    if (left5_op == 3 && left5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block3_Form;

                                    // op_block3_Form
                                    if (left5_op == 0 && left5_self == 3)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block3_Form;

                                    // self_block2_Form
                                    if (left5_op == 2 && left5_self == 0)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block2_Form;

                                    // op_block2_Form
                                    if (left5_op == 0 && left5_self == 2)
                                        if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                            chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block2_Form;
                                }
                            }

                            /* none of border */
                            else
                            {
                                // self_block4_Form
                                if ((left5_op == 0 && left5_self == 4) || (right5_op == 0 && right5_self == 4))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block4_Form;

                                // op_block4_Form
                                if ((left5_op == 4 && left5_self == 0) || (right5_op == 4 && right5_self == 0))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block4_Form;

                                // self_block3_Form
                                if ((left5_op == 0 && left5_self == 3) || (right5_op == 0 && right5_self == 3))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block3_Form;

                                // op_block3_Form
                                if ((left5_op == 3 && left5_self == 0) || (right5_op == 3 && right5_self == 0))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block3_Form;

                                // self_block2_Form
                                if ((left5_op == 0 && left5_self == 2) || (right5_op == 0 && right5_self == 2))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = self_block2_Form;

                                // op_block2_Form
                                if ((left5_op == 2 && left5_self == 0) || (right5_op == 2 && right5_self == 0))
                                    if (chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] == 0)
                                        chessTemplate_tuple[loc_1][loc_2][loc_3][loc_4][loc_5][loc_6] = op_block2_Form;

                            }
                        }
    Data_Initialize();
}

void Evaluation::Data_Initialize(void)
{
    /* Evaluation Feedback Data */
    score = 0;
    count_win = 0;
    count_lose = 0;
    count_flex4 = 0;
    count_block4 = 0;
    count_flex3 = 0;
    victory_color = status_none;
    memset(form_data, 0, sizeof(form_data));
    memset(border_pieces, 0, sizeof(border_pieces));
    /* Board set */
    for (int r = 0; r < 17; r++)
        border_pieces[r][0] = 3;
    for (int r = 0; r < 17; r++)
        border_pieces[r][16] = 3;
    for (int c = 0; c < 17; c++)
        border_pieces[0][c] = 3;
    for (int c = 0; c < 17; c++)
        border_pieces[16][c] = 3;
}

int Evaluation::getScore(void)
{
    return score;
}

int Evaluation::getVictory_color(void)
{
    return victory_color;
}

int* Evaluation::Get_killCount(void)
{
    static int killCount[5];
    killCount[0] = count_win;
    killCount[1] = count_lose;
    killCount[2] = count_flex4;
    killCount[3] = count_block4;
    killCount[4] = count_flex3;
    return killCount;
}

void Evaluation::evaluate(int** pieces, bool color)
{
    /* Read pieces_data */
    if (color == color_black)
    {
        for (int r = 0; r < 15; r++)
            for (int c = 0; c < 15; c++)
                if (pieces[r][c] == status_none)
                    border_pieces[r + 1][c + 1] = 0;
                else if (pieces[r][c] == (int)color_black)
                    border_pieces[r + 1][c + 1] = 2;
                else if (pieces[r][c] == (int)color_white)
                    border_pieces[r + 1][c + 1] = 1;
    }
    else
    {
        for (int r = 0; r < 15; r++)
            for (int c = 0; c < 15; c++)
                if (pieces[r][c] == status_none)
                    border_pieces[r + 1][c + 1] = 0;
                else if (pieces[r][c] == (int)color_black)
                    border_pieces[r + 1][c + 1] = 1;
                else if (pieces[r][c] == (int)color_white)
                    border_pieces[r + 1][c + 1] = 2;
    }

    /* Update form_data */
    int form_type;
    // row
    for(int r = 1; r <= 15; r++)
        for (int c = 0; c < 12; c++)
        {
            form_type = chessTemplate_tuple[border_pieces[r][c]][border_pieces[r][c+1]][border_pieces[r][c+2]][border_pieces[r][c+3]][border_pieces[r][c+4]][border_pieces[r][c+5]];
            form_data[0][form_type]++;
        }
    // column
    for (int c = 1; c <= 15; c++)
        for (int r = 0; r < 12; r++)
        {
            form_type = chessTemplate_tuple[border_pieces[r][c]][border_pieces[r+1][c]][border_pieces[r+2][c]][border_pieces[r+3][c]][border_pieces[r+4][c]][border_pieces[r+5][c]];
            form_data[1][form_type]++;
        }
    // right_down
    for (int r = 0; r < 12; r++) 
        for (int c = 0; c < 12; c++) 
        {
            form_type = chessTemplate_tuple[border_pieces[r][c]][border_pieces[r+1][c+1]][border_pieces[r+2][c+2]][border_pieces[r+3][c+3]][border_pieces[r+4][c+4]][border_pieces[r+5][c+5]];
            form_data[2][form_type]++;
        }
    // left_down
    for (int r = 0; r < 12; r++) 
        for (int c = 5; c < 17; c++) 
        {
            form_type = chessTemplate_tuple[border_pieces[r][c]][border_pieces[r+1][c-1]][border_pieces[r+2][c-2]][border_pieces[r+3][c-3]][border_pieces[r+4][c-4]][border_pieces[r+5][c-5]];
            form_data[3][form_type]++;
        }

    /* Calculate score and record key_form num */
    score = 0;  // reset
    count_win = 0; 
    count_lose = 0; 
    count_flex4 = 0; 
    count_block4 = 0; 
    count_flex3 = 0;
    victory_color = status_none;
    for (int form = 1; form < 17; form++)
    {
        // calculate score basically
        score += weight[form] * (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
        // record key_form num
        if (form == win_Form)
            count_win = (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
        else if (form == lose_Form)
            count_lose = (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
        else if (form == self_flex4_Form)
            count_flex4 = (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
        else if (form == self_block4_Form)
            count_block4 = (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
        else if (form == self_flex3_Form)
            count_flex3 = (form_data[0][form] + form_data[1][form] + form_data[2][form] + form_data[3][form]);
    }
    if (count_win)
        victory_color = (int)color; // self wins
    if (count_lose)
        victory_color = (int)(!color);  // opponent wins
    // Reset
    memset(form_data, 0, sizeof(form_data));    
}


/* -------------- AI_Function -------------- */

AI_Function::AI_Function(void) 
{
    AI_color = status_none;
    pieces_int = new int* [15];
    decision = new int[2];
    for (int r = 0; r < 15; r++)
        pieces_int[r] = new int[15];
    curDecision_r = 0;
    curDecision_c = 0;
    memset(choice_score, 0, sizeof(choice_score));
    memset(choice_r, 0, sizeof(choice_r));
    memset(choice_c, 0, sizeof(choice_c));
}

int AI_Function::setColor(int color)
{
    if (color == para_default)
        return AI_color;
    AI_color = color;
    return AI_color;
}

int* AI_Function::get_Decision(Piece** pieces, int search_depth)
{
    for (int r = 0; r < 15; r++)
        for (int c = 0; c < 15; c++)
            pieces_int[r][c] = pieces[r][c].Piece_colorSet();
    if (!kill_Search(pieces_int, 16))   // kill_Search first
    {
        /* Minimax search (based on alpha-beta prune) */
        Search(pieces_int, search_depth, INT_MIN, INT_MAX, search_depth);
    }
    decision[0] = curDecision_r;
    decision[1] = curDecision_c;
    return decision;
}

void AI_Function::provide_Choice(int** pieces, bool self, int num)
{
    /* partly search (mark valid) */
    bool valid[15][15]; // true means allowed to search
    memset(valid, 0, sizeof(valid));
    for (int r = 0; r < 15; r++)
        for (int c = 0; c < 15; c++)
        {
            if (pieces[r][c] != status_none)
                for (int extend = -3; extend <= 3; extend++)
                {
                    if (r + extend >= 0 && r + extend < 15)
                    {
                        valid[r + extend][c] = true;
                        if (c + extend >= 0 && c + extend < 15)
                            valid[r + extend][c + extend] = true;
                        if (c - extend >= 0 && c - extend < 15)
                            valid[r + extend][c - extend] = true;
                    }
                    if (c + extend >= 0 && c + extend < 15)
                        valid[r][c + extend] = true;
                }
        }
    /* compare and record score(after putting valid piece) */
    int simulated_score[15][15];
    for (int r = 0; r < 15; r++)
        for (int c = 0; c < 15; c++)
        {
            if (pieces[r][c] == status_none && valid[r][c]) // allow to simulate
            {
                if (self)
                {
                    pieces[r][c] = AI_color;
                    evaluation.evaluate(pieces, AI_color);
                }

                else
                {
                    pieces[r][c] = !AI_color;
                    evaluation.evaluate(pieces, !AI_color);
                }
                simulated_score[r][c] = evaluation.getScore();
                pieces[r][c] = status_none; // traceback
            }
            else
            {
                simulated_score[r][c] = INT_MIN;
            }
        }
    // get 10 best choices
    int max;
    for (int tmp = 0; tmp < num; tmp++)
    {
        max = INT_MIN;
        for (int r = 0; r < 15; r++)
            for (int c = 0; c < 15; c++)
            {
                if (simulated_score[r][c] > max)
                {
                    max = simulated_score[r][c];
                    choice_score[tmp] = max;
                    choice_r[tmp] = r;
                    choice_c[tmp] = c;
                }
            }
        simulated_score[choice_r[tmp]][choice_c[tmp]] = INT_MIN;
    }
}


int AI_Function::Search(int** pieces, int cur_depth, int alpha, int beta, const int search_depth)
{
    evaluation.evaluate(pieces, AI_color);  // evaluate pieces
    // Victory End
    if (evaluation.getVictory_color() != status_none)
    {
        return evaluation.getScore();   // Final Victory score
    }
    // Search End
    if (cur_depth == 0)
    {
        provide_Choice(pieces, true, 10);   // self choice
        return choice_score[0];
    }
    // max
    else if (cur_depth % 2 == 0)
    {
        provide_Choice(pieces, true, 10);   // self choice
        int store_choice[2][10];
        for(int i = 0; i < 2; i++)
            for (int j = 0; j < 10; j++)
                if (i == 0)
                    store_choice[i][j] = choice_r[j];
                else
                    store_choice[i][j] = choice_c[j];
        for (int k = 0; k < 10; k++)
        {
            pieces[store_choice[0][k]][store_choice[1][k]] = AI_color;
            int new_alpha = Search(pieces, cur_depth - 1, alpha, beta, search_depth);
            pieces[store_choice[0][k]][store_choice[1][k]] = status_none;   // traceback
            if (new_alpha > alpha)
            {
                alpha = new_alpha;
                if (cur_depth == search_depth)   // update final decision
                {
                    curDecision_r = store_choice[0][k];
                    curDecision_c = store_choice[1][k];
                }
            }
            if (alpha >= beta)
                break;  // prune
        }
        return alpha;
    }
    // min
    else
    {
        provide_Choice(pieces, false, 10);  // opponent choice
        int store_choice[2][10];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 10; j++)
                if (i == 0)
                    store_choice[i][j] = choice_r[j];
                else
                    store_choice[i][j] = choice_c[j];
        for (int k = 0; k < 10; k++)
        {
            pieces[store_choice[0][k]][store_choice[1][k]] = !AI_color;
            int new_beta = Search(pieces, cur_depth - 1, alpha, beta, search_depth);
            pieces[store_choice[0][k]][store_choice[1][k]] = status_none;   // traceback
            if (new_beta < beta)
                beta = new_beta;
            if (alpha >= beta)
                break;  // prune
        }
        return beta;
    }
}


QList<QPoint> AI_Function::provide_kill_Choice(int** pieces)
{
    QPoint appended_choice;
    QList<QPoint> kill_Choice;
    provide_Choice(pieces, true, 20);
    evaluation.evaluate(pieces, AI_color);
    int* ori_killCount = evaluation.Get_killCount(); // 0-count_win, 1-count_lose, 2-count_flex4, 3-count_block4, 4-count_flex3
    for (int tmp = 0; tmp < 20; tmp++)
    {
        pieces[choice_r[tmp]][choice_c[tmp]] = AI_color;
        appended_choice.setX(choice_r[tmp]);
        appended_choice.setY(choice_c[tmp]);
        evaluation.evaluate(pieces, AI_color);
        pieces[choice_r[tmp]][choice_c[tmp]] = status_none; // traceback
        int* killCount = evaluation.Get_killCount();
        if (killCount[0] || killCount[2] > ori_killCount[2] || killCount[3] > ori_killCount[3] || killCount[4] > ori_killCount[4])
            kill_Choice.append(appended_choice);    // new kill_point
    }
    return kill_Choice;
}

bool AI_Function::kill_Search(int** pieces, int cur_depth)
{
    evaluation.evaluate(pieces, AI_color);
    // Victory End
    if (evaluation.getVictory_color() != status_none)
    {
        if (evaluation.getVictory_color() == AI_color)
            return true;    // Kill_Search Success
        else
            return false;
    }
    // Search End
    if (cur_depth == 0)
    {
        provide_Choice(pieces, true, 1);    // check if kill
        pieces[choice_r[0]][choice_c[0]] = AI_color;
        evaluation.evaluate(pieces, AI_color);
        pieces[choice_r[0]][choice_c[0]] = status_none; // traceback
        if (evaluation.getVictory_color() == AI_color)
            return true;
        return false;
    }
    // max
    else if (cur_depth % 2 ==0)
    {
        QList<QPoint> kill_Choice = provide_kill_Choice(pieces);
        if (kill_Choice.length() == 0)
            return false;   // NO kill_choice
        for (auto kill_check : kill_Choice)
        {
            pieces[kill_check.x()][kill_check.y()] = AI_color;
            if (kill_Search(pieces, cur_depth - 1))
            {
                pieces[kill_check.x()][kill_check.y()] = status_none;   // traceback
                if (cur_depth == 16)
                {
                    curDecision_r = kill_check.x();
                    curDecision_c = kill_check.y();
                }
                return true;    // enable to kill
            }
        }
        return false;
    }
    // min
    else
    {
        provide_Choice(pieces, false, 1);
        pieces[choice_r[0]][choice_c[0]] = !AI_color;
        bool continue_license = kill_Search(pieces, cur_depth - 1);
        pieces[choice_r[0]][choice_c[0]] = status_none;
        return continue_license;
    }
}
// 1000 is a perfect number, isn't it?