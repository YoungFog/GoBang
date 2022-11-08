#include "PVP_Server.h"
#include "Regame.h"
#include "Trace.h"
#include <QTime>
#include <QtGlobal>
#include <QGraphicsBlurEffect>

PVP_Server::PVP_Server(QWidget *parent)
	: QWidget(parent)
{
    ui = new Ui::PVP_Server; 
	ui->setupUi(this);
    // wipe off frame
    this->setWindowFlags(Qt::FramelessWindowHint);
    myServer = new QTcpServer(this);
    cur_socket = NULL;
    board = new Board;
    UI = new QPixmap;
    Start = false;
    waiting = new Waiting_UI(this);

    /* Animation_Button */
    // Icon_blue
    icon_ReGame_request.addFile(tr(":/Resource2/UI_Design/AI_Regame.png"));
    icon_Traceback.addFile(tr(":/Resource2/UI_Design/AI_Traceback.png"));
    // ReGame_request
    ReGame_request = new QAnimationButton(ui->Main_UI);
    ReGame_request->SizeSet(1);
    ReGame_request->setGeometry(869, 947, 182, 56);
    ReGame_request->setIcon(icon_ReGame_request);
    ReGame_request->setIconSize(QSize(182, 56));
    ReGame_request->setFlat(true);
    ReGame_request->hide();
    connect(ReGame_request, SIGNAL(clicked()), this, SLOT(on_ReGame_request_clicked()));
    // Traceback
    Traceback = new QAnimationButton(ui->Main_UI);
    Traceback->SizeSet(1);
    Traceback->setGeometry(1643, 852, 142, 56);
    Traceback->setIcon(icon_Traceback);
    Traceback->setIconSize(QSize(142, 56));
    Traceback->setFlat(true);
    connect(Traceback, SIGNAL(clicked()), this, SLOT(on_Traceback_clicked()));

    /* Random Order */
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int rand = qrand() % 10;
    if (rand <= 4)
        server_color = color_black;   // 50 percents to black
    else
        server_color = color_white;

}

PVP_Server::~PVP_Server()
{
    delete ui;
    delete myServer;
    delete cur_socket;
    delete board;
    delete UI;
}

void PVP_Server::server_hide()
{
    hide();
    emit pvp_server_hide();
}

// Initialize
void PVP_Server::Initialize(bool unconnected)
{
    if (!unconnected) // connected
    {
        if(ReGame_request->isVisible())
            ReGame_request->hide();
        cur_socket = NULL;
        if (board)
        {
            delete board;
            board = new Board;
        }
        if (UI)
        {
            delete UI;
            UI = new QPixmap;
        }
        Start = false;
        /* Random Order */
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        int rand = qrand() % 10;
        if (rand <= 4)   // 50 percents
            server_color = color_black;
        else
            server_color = color_white;
    }
    if (myServer)
    {
        delete myServer;
        myServer = new QTcpServer(this);
    }
    server_ip.clear();
    server_port.clear();
    ui->back_Button->setEnabled(true);
    ReGame_request->setEnabled(true);
    ReGame_request->hide();
    Traceback->setEnabled(false);
}

void PVP_Server::Inf_Initialize(void)  // Inf_Initialize
{
    //ui->name_1->setText(server_settings_data.player_name);
    //ui->name_2->setText(client_settings_data.player_name);
    //ui->name_1->setText(QString::fromLocal8Bit("房主"));
    //ui->name_2->setText(QString::fromLocal8Bit("玩家"));
    if (server_color == color_black)
    {
        ui->playerInf_1->setText(QString::fromLocal8Bit("自己 (黑)"));
        ui->playerInf_2->setText(QString::fromLocal8Bit("对手 (白)"));
        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:black");
    }
    else
    {
        ui->playerInf_1->setText(QString::fromLocal8Bit("自己 (白)"));
        ui->playerInf_2->setText(QString::fromLocal8Bit("对手 (黑)"));
        ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到对方落子"));
        ui->pieceInf_2->setStyleSheet("color:red");
        ui->pieceInf_1->setStyleSheet("color:black");
    }
    // Trace reset
    Traceback->setEnabled(false);
    trace_chance = 1;
    ui->Trace_Inf->setText(QString::fromLocal8Bit("本模式仅一次机会"));
    ui->Trace_Inf->setStyleSheet("color:black;background:rgb(247,248,250)");
}

void PVP_Server::ReadSettings(Settings_Data const & user_settings_data)
{
    server_settings_data.voice_package = user_settings_data.voice_package;
    server_settings_data.player_name = user_settings_data.player_name;
    server_settings_data.tone = user_settings_data.tone;
    server_settings_data.provocative = user_settings_data.provocative;
    server_settings_data.move = user_settings_data.move;
    server_settings_data.emoji = user_settings_data.emoji;
    server_settings_data.num = user_settings_data.num;
    server_settings_data.auto_atack = user_settings_data.auto_atack;
    server_settings_data.auto_counter = user_settings_data.auto_counter;
}

void PVP_Server::settings_Initialize()
{
    // 1. character
    // self
    if (server_settings_data.voice_package == 0)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_1.png)");
    else if (server_settings_data.voice_package == 1)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_2.png)");
    else if (server_settings_data.voice_package == 2)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_6.png)");
    else if (server_settings_data.voice_package == 3)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_7.png)");
    // opponent
    if (client_settings_data.voice_package == 0)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_1.png)");
    else if (client_settings_data.voice_package == 1)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_2.png)");
    else if (client_settings_data.voice_package == 2)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_6.png)");
    else if (client_settings_data.voice_package == 3)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_7.png)");
    // 2. Name
    ui->name_1->setText(server_settings_data.player_name);
    ui->name_2->setText(client_settings_data.player_name);
}

QString PVP_Server::ip_Process(QString ip)
{
    if (ip != "")  // not default
        server_ip = ip;
    return server_ip;
}


QString PVP_Server::port_Process(QString port)
{
    if (port != "")  // not default
        server_port = port;
    return server_port;
}


// listen
bool PVP_Server::ip_Listen()
{
    QString ip_descriptor = ip_Process();
    QString port_descriptor = port_Process();
    waiting->TextSet(ip_Process());
    waiting->TextSet2(port_Process());
    bool ret = myServer->listen(QHostAddress(ip_descriptor), port_descriptor.toUInt()); // listen 
    if (!ret)
    {
        /* Illegal port */
        delete myServer;
        myServer = new QTcpServer(this);
        return false;
    }
    else
    {
        /* Create Success */
        myServer->setMaxPendingConnections(1);     // Set num
        connect(myServer, SIGNAL(newConnection()), this, SLOT(ProcessNewConnection()));     // process new connection
        /* Waiting */
        waiting->cancel = false;    // reset
        show();
        ui->Main_UI->hide();
        waiting->exec();
        if (waiting->cancel)
        {
            server_hide();
            Initialize(true);
            return false;
        }
        else
        {
            ui->Main_UI->show();
            return true;
        }
    }
}

// process new connection signal
void PVP_Server::ProcessNewConnection()
{
    cur_socket = myServer->nextPendingConnection();     // Get client descriptor
    waiting->close();
    UI->load(":/Resource/UI_Design/Board_UI.png");
    Start = true;
    // Disconnect
    connect(cur_socket, SIGNAL(disconnected()), this, SLOT(ProcessDisconnected()));
    // Read
    connect(cur_socket, SIGNAL(readyRead()), this, SLOT(ProcessReadyRead()));
    int client_order = (int)(!server_color);
    Send_OrderMessage(client_order);    // send color
    Inf_Initialize();
    if (cur_socket->waitForBytesWritten())
    {
        Send_SettingsMessage();// send settings
 //       if (cur_socket->waitForBytesWritten())
 //           Send_NameMessage(); // send name
    }
}


// Disconnect
void PVP_Server::ProcessDisconnected()
{
    //
}

// Read
void PVP_Server::ProcessReadyRead()
{
    QByteArray Data;
    Data = cur_socket->readAll();
    int symbol = Data[0];
    if (symbol == 1)    // pieceMessage from client
    {
        board->GetMove(Data[1], Data[2]);
        update();
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:black");
    }
    else if (symbol == 4)    // VictoryMessage from client
    {
        board->GetMove(Data[1], Data[2]);
        board->obvious_Victory();   // update win_piece
        update();
        Start = false;
        Traceback->setEnabled(false);
        ReGame_request->show();
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("已被对方战胜"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("轻轻松松拿下"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:red");
    }
    else if (symbol == 5)    // RegameMessage from client
    {
        Regame W(this);
        W.show();
        ui->back_Button->setEnabled(false);
        ReGame_request->setEnabled(false);
        QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
        blureffect->setBlurRadius(50);
        ui->Main_UI->setGraphicsEffect(blureffect);    // Blur
        W.exec();
        ui->back_Button->setEnabled(true);
        ReGame_request->setEnabled(true);
        blureffect->setBlurRadius(0);
        if (W.IsAgree())
        {
            ReGame_request->hide();
            delete board;
            board = new Board;
            delete UI;
            UI = new QPixmap;
            /* Random Order */
            qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
            int rand = qrand() % 10;
            if (rand <= 4)   // 50 percents
                server_color = color_black;
            else
                server_color = color_white;
            UI->load(":/Resource/UI_Design/Board_UI.png");
            Start = true;
            int client_order = (int)(!server_color);
            Send_OrderMessage(client_order);
            Inf_Initialize();
            ui->back_Button->setEnabled(true);
            ReGame_request->setEnabled(true);
            Traceback->setEnabled(false);
            if (cur_socket->waitForBytesWritten())
                Send_AgreeMessage(W.IsAgree());
        }
        else
            Send_AgreeMessage(W.IsAgree());
    }
    else if (symbol == 8)   // AgreeMessage from client
    {
        ui->back_Button->setEnabled(true);
        int status_agree = Data[1];
        if (status_agree)
        {
            ReGame_request->hide();
            delete board;
            board = new Board;
            delete UI;
            UI = new QPixmap;
            /* Random Order */
            qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
            int rand = qrand() % 10;
            if (rand <= 4)   // 50 percents
                server_color = color_black;
            else
                server_color = color_white;
            UI->load(":/Resource/UI_Design/Board_UI.png");
            Start = true;
            int client_order = (int)(!server_color);
            Send_OrderMessage(client_order);
            Inf_Initialize();
            ui->back_Button->setEnabled(true);
            ReGame_request->setEnabled(true);
            Traceback->setEnabled(false);
        }
        else
        {
            /* Your friend has refused you */
            ReGame_request->setEnabled(true);
        }
    }
    else if (symbol == 9)   // BackMessage from client
    {
        // Your friend has left
        server_hide();
        Initialize(false);
    }
    else if (symbol == 11)   // DeadlockMessage from client
    {
        // deadlock
        ReGame_request->show();
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("双方进入僵局"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("双方进入僵局"));
        ui->pieceInf_2->setStyleSheet("color:red");
        ui->pieceInf_1->setStyleSheet("color:red");
        Traceback->setEnabled(false);
    }
    else if (symbol == 13)  // SettingsMessage from client
    {
        client_settings_data.voice_package = Data[1];
        client_settings_data.tone = Data[2];
        client_settings_data.provocative = Data[3];
        client_settings_data.move = Data[4];
        client_settings_data.emoji = Data[5];
        client_settings_data.num = Data[6];
        client_settings_data.auto_atack = Data[7];
        client_settings_data.auto_counter = Data[8];
        Data.remove(0, 9);
        client_settings_data.player_name = Data;
        settings_Initialize();
    }
    else if (symbol == 16)  // NameMessage from client
    {
        //Data.startsWith(Data[1]);
        //client_settings_data.player_name = Data;
        //ui->name_2->setText(client_settings_data.player_name);
    }
    else if (symbol == 17)  // TraceMessage from client
    {
        Trace W(this);
        W.show();
        ui->back_Button->setEnabled(false);
        QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
        blureffect->setBlurRadius(50);
        ui->Main_UI->setGraphicsEffect(blureffect);    // Blur
        W.exec();
        ui->back_Button->setEnabled(true);
        blureffect->setBlurRadius(0);
        if (W.IsAgree())
        {
            if(board->GetPlayer() == server_color)
                board->Traceback();
            else
            {
                board->Traceback();
                board->Traceback();
            }
            update();
            // Inf
            ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到对方落子"));
            ui->pieceInf_1->setStyleSheet("color:black");
            ui->pieceInf_2->setStyleSheet("color:red");
            if(board->GetPiecesCount() == 0)
                Send_Allow_TraceMessage(false);
            else
                Send_Allow_TraceMessage(true);
        }
        else
            Send_Allow_TraceMessage(false);
    }
    else if (symbol == 19)  // Allow_TraceMessage from client
    {
        ui->back_Button->setEnabled(true);
        if (Data[1])
        {
            // Agreed by server
            if (board->GetPlayer() == server_color)
            {
                board->Traceback();
                board->Traceback();
            }
            else
                board->Traceback();
            update();
            // Inf
            ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
            ui->pieceInf_1->setStyleSheet("color:red");
            ui->pieceInf_2->setStyleSheet("color:black");
        }
        else
        {
            // Refused by client
        }
        Start = true;
    }
}

/* Send */

// 0. Send PieceMessage from Server
void PVP_Server::Send_PieceMessage(int put_row, int put_column)
{
    QByteArray Data;
    Data[0] = 0;   // 0 means pieceMessage from Server
    Data[1] = put_row;
    Data[2] = put_column;
    cur_socket->write(Data);     // send message
}

// 2. Send OrderMessage from Server
void PVP_Server::Send_OrderMessage(int order)
{
    QByteArray Data;
    Data[0] = 2;   // 2 means OrderMessage from Server
    Data[1] = (int)(!server_color);
    cur_socket->write(Data);     // send message
}

// 3. Send VictoryMessage from server
void PVP_Server::Send_VictoryMessage(int put_row, int put_column)
{
    QByteArray Data;
    Data[0] = 3;   // 3 means VictoryMessage from server
    Data[1] = put_row;
    Data[2] = put_column;
    cur_socket->write(Data);     // send message
}

// 6. Send RegameMessage from server
void PVP_Server::Send_RegameMessage(void)
{
    ReGame_request->setEnabled(false);
    ui->back_Button->setEnabled(false);
    QByteArray Data;
    Data[0] = 6;   // 6 means RegameMessage from server
    cur_socket->write(Data);     // send message
}

// 7. Send AgreeMessage from server
void PVP_Server::Send_AgreeMessage(bool Status_Agree)
{
    QByteArray Data;
    Data[0] = 7;
    Data[1] = (int)Status_Agree;   // 7 means AgreeMessage from server
    cur_socket->write(Data);     // send message
}

// 10. Send BackMessage from server
void PVP_Server::Send_BackMessage(void)
{
    QByteArray Data;
    Data[0] = 10;   // 10 means BackMessage from server
    cur_socket->write(Data);     // send message
}

// 12. Send DeadlockMessage from client
void PVP_Server::Send_DeadlockMessage(void)
{
    QByteArray Data;
    Data[0] = 12;    // 12 means DeadlockMessage from server
    cur_socket->write(Data);     // send message
}

// 14. Send SettingsMessage from server
void PVP_Server::Send_SettingsMessage(void)
{
    QByteArray Data;
    Data[0] = 14;    // 14 means SettingsMessage from server
    // other data
    Data[1] = server_settings_data.voice_package;
    Data[2] = server_settings_data.tone;
    Data[3] = server_settings_data.provocative;
    Data[4] = server_settings_data.move;
    Data[5] = server_settings_data.emoji;
    Data[6] = server_settings_data.num;
    Data[7] = server_settings_data.auto_atack;
    Data[8] = server_settings_data.auto_counter;
    Data.append(server_settings_data.player_name);
    cur_socket->write(Data);     // send message
}

// 15. Send NameMessage from server
void PVP_Server::Send_NameMessage(void)
{
    //QByteArray Data;
    //Data[0] = 15;
    //cur_socket->write(Data);     // send message
}

// 18. Send TraceMessage from server
void PVP_Server::Send_TraceMessage(void)
{
    ui->back_Button->setEnabled(false);
    QByteArray Data;
    Data[0] = 18;   // 18 means NameMessage from server
    cur_socket->write(Data);     // send message
}

// 20. Send Allow_TraceMessage from server
void PVP_Server::Send_Allow_TraceMessage(bool Status_Allow)
{
    QByteArray Data;
    Data[0] = 20;   // 20 means Allow_TraceMessage from server
    Data[1] = Status_Allow;
    cur_socket->write(Data);     // send message
}

/*----------------- PVP_Game(server) -------------------*/


/* --------- Interaction ---------- */

/* MouseEvent */
void PVP_Server::mousePressEvent(QMouseEvent* event)
{
    if (Start && event->button() == Qt::LeftButton)  // Left_Press
    {
        static int press_x = 0, press_y = 0, standard_x = 0, standard_y = 0, put_column = 0, put_row = 0;
        press_x = event->x();
        press_y = event->y();
        if (server_color == board->GetPlayer() && press_x >= board->GetLeftMargin() - board->GetRange() && press_x <= board->GetLeftMargin() + board->GetLength() + board->GetRange() && press_y >= board->GetTopMargin() - board->GetRange() && press_y <= board->GetTopMargin() + board->GetWidth() + board->GetRange())
        {
            standard_x = press_x - board->GetLeftMargin() + board->GetSize() / 2;
            standard_y = press_y - board->GetTopMargin() + board->GetSize() / 2;
            if (standard_x % board->GetSize() >= board->GetSize() / 2 - board->GetRange() && standard_x % board->GetSize() <= board->GetSize() / 2 + board->GetRange() && standard_y % board->GetSize() >= board->GetSize() / 2 - board->GetRange() && standard_y % board->GetSize() <= board->GetSize() / 2 + board->GetRange())   // allow to click
            {
                put_column = standard_x / board->GetSize();
                put_row = standard_y / board->GetSize();
                if (!board->GetMove(put_row, put_column)) // Move
                {
                    // not allowed to put here
                }
                else
                {
                    update();
                    /* Traceback set */
                    if (trace_chance)
                        Traceback->setEnabled(true);
                    // Inf
                    ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
                    ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到对方落子"));
                    ui->pieceInf_2->setStyleSheet("color:red");
                    ui->pieceInf_1->setStyleSheet("color:black");
                    /* Judge vitory */
                    if (board->obvious_Victory())
                    {
                        Traceback->setEnabled(false);
                        Send_VictoryMessage(put_row, put_column);
                        ReGame_request->show();
                        // Inf
                        ui->pieceInf_1->setText(QString::fromLocal8Bit("您已战胜对手"));
                        ui->pieceInf_2->setText(QString::fromLocal8Bit("我还得回家练练"));
                        ui->pieceInf_2->setStyleSheet("color:red");
                        ui->pieceInf_1->setStyleSheet("color:red");
                    }
                    else
                        Send_PieceMessage(put_row, put_column);
                    /* Judge deadlock */
                    if (board->Deadlock())
                    {
                        // deadlock
                        ReGame_request->show();
                        Send_DeadlockMessage();
                        // Inf
                        ui->pieceInf_1->setText(QString::fromLocal8Bit("双方进入僵局"));
                        ui->pieceInf_2->setText(QString::fromLocal8Bit("双方进入僵局"));
                        ui->pieceInf_2->setStyleSheet("color:red");
                        ui->pieceInf_1->setStyleSheet("color:red");
                    }
                }
            }
        }
    }
}


/* Paint Piece */
void PVP_Server::paintEvent(QPaintEvent* event)
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
        if (method == 0)
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

// back
void PVP_Server::on_back_Button_clicked(void)
{
    server_hide();
    Send_BackMessage();
    cur_socket->waitForReadyRead();
    Initialize(false);
}

// ReGame
void PVP_Server::on_ReGame_request_clicked(void)
{
    Send_RegameMessage();
}

// traceback
void PVP_Server::on_Traceback_clicked()
{
    Start = false;
    Send_TraceMessage();
    trace_chance--;
    Traceback->setEnabled(false);
    ui->Trace_Inf->setText(QString::fromLocal8Bit("悔棋机会已用完"));
    ui->Trace_Inf->setStyleSheet("color:grey;background:rgb(247,248,250)");
}
