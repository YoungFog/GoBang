#include "PVP_Client.h"
#include "Regame.h"
#include "Trace.h"
#include <QGraphicsBlurEffect>

PVP_Client::PVP_Client(QWidget *parent)
	: QWidget(parent)
{
    ui = new Ui::PVP_Client;
	ui->setupUi(this);
    // wipe off frame
    this->setWindowFlags(Qt::FramelessWindowHint);
    myClient = new QTcpSocket(this);
    board = new Board;
    UI = new QPixmap;
    Order = false;
    Start = false;

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
}

PVP_Client::~PVP_Client()
{
    delete ui;
    delete myClient;
    delete board;
    delete UI;
}

void PVP_Client::client_hide()
{
    hide();
    emit pvp_client_hide();
}

// Initialize
void PVP_Client::Initialize(bool unconnected)
{
    if (!unconnected)
    {
        if (ReGame_request->isVisible())
            ReGame_request->hide();
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
    }
    if (myClient)
    {
        delete myClient;
        myClient = new QTcpSocket(this);
    }
    client_ip.clear();
    client_port.clear();
    Start = false;
    ui->back_Button->setEnabled(true);
    ReGame_request->setEnabled(true);
    ReGame_request->hide();
}

void PVP_Client::Inf_Initialize(void)  // Inf_Initialize
{
    //ui->name_1->setText(client_settings_data.player_name);
    //ui->name_2->setText(server_settings_data.player_name);
    //ui->name_1->setText(QString::fromLocal8Bit("玩家"));
    //ui->name_2->setText(QString::fromLocal8Bit("房主"));
    if (Order)  // first
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

void PVP_Client::ReadSettings(Settings_Data const & user_settings_data)
{
    client_settings_data.voice_package = user_settings_data.voice_package;
    client_settings_data.player_name = user_settings_data.player_name;
    client_settings_data.tone = user_settings_data.tone;
    client_settings_data.provocative = user_settings_data.provocative;
    client_settings_data.move = user_settings_data.move;
    client_settings_data.emoji = user_settings_data.emoji;
    client_settings_data.num = user_settings_data.num;
    client_settings_data.auto_atack = user_settings_data.auto_atack;
    client_settings_data.auto_counter = user_settings_data.auto_counter;
}

void PVP_Client::settings_Initialize()
{
    // 1. character
    // self
    if (server_settings_data.voice_package == 0)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_1.png)");
    else if (server_settings_data.voice_package == 1)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_2.png)");
    else if (server_settings_data.voice_package == 2)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_6.png)");
    else if (server_settings_data.voice_package == 3)
        ui->character_2->setStyleSheet("image: url(:/Resource2/UI_Design/character_7.png)");
    // opponent
    if (client_settings_data.voice_package == 0)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_1.png)");
    else if (client_settings_data.voice_package == 1)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_2.png)");
    else if (client_settings_data.voice_package == 2)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_6.png)");
    else if (client_settings_data.voice_package == 3)
        ui->character_1->setStyleSheet("image: url(:/Resource2/UI_Design/character_7.png)");
    // 2. Name
    ui->name_1->setText(client_settings_data.player_name);
    ui->name_2->setText(server_settings_data.player_name);
}

QString PVP_Client::ip_Process(QString ip)
{
    if (ip != "")  // not default
        client_ip = ip;
    return client_ip;
}


QString PVP_Client::port_Process(QString port)
{
    if (port != "")  // not default
        client_port = port;
    return client_port;
}


// connect
bool PVP_Client::Connect_To_Server(void)
{
    show();
    QString serverIP = ip_Process();    // get ip
    QString serverPort = port_Process();    // get port
    myClient->connectToHost(QHostAddress(serverIP), serverPort.toInt());   // connect 
    connect(myClient, SIGNAL(connected()), this, SLOT(ProcessConnected()));   // connected
    connect(myClient, SIGNAL(readyRead()), this, SLOT(ProcessReadyRead()));  // read
    connect(myClient, SIGNAL(disconnected()), this, SLOT(ProcessDisconnected()));   // disconnect
    if (!myClient->waitForConnected())
    {
        // Wrong password
        client_hide();
        Initialize(false);
        return false;
    }
    return true;
}


// Connected
void PVP_Client::ProcessConnected()
{
    Start = true;
    UI->load(":/Resource/UI_Design/Board_UI.png");
    Send_SettingsMessage(); // send settings
//    if (myClient->waitForBytesWritten())
//        Send_NameMessage(); // send name
}

// disconnect
void PVP_Client::ProcessDisconnected()
{
    //
}

// Read
void PVP_Client::ProcessReadyRead()
{
    QByteArray Data;
    Data = myClient->readAll();
    int symbol = Data[0];
    if (symbol == 0)    // pieceMessage from server
    {
        Order = true;
        board->GetMove(Data[1], Data[2]);
        update();
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:black");
    }
    else if (symbol == 2)    // OrderMessage from server
    {
        int Ori_order = Data[1];
        if (Ori_order == (int)color_black)  // client first
            Order = true;
        else
            Order = false;
        Inf_Initialize();
    }
    else if (symbol == 3)   // VictoryMessage from server
    {
        board->GetMove(Data[1], Data[2]);
        board->obvious_Victory();   // update win_piece
        update();
        Start = false;
        ReGame_request->show();
        Traceback->setEnabled(false);
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("已被对方战胜"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("轻轻松松拿下"));
        ui->pieceInf_1->setStyleSheet("color:red");
        ui->pieceInf_2->setStyleSheet("color:red");
    }
    else if (symbol == 6)   // RegameMessage from server
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
            Start = true;
            UI->load(":/Resource/UI_Design/Board_UI.png");
            Traceback->setEnabled(false);
        }
        Send_AgreeMessage(W.IsAgree());
    }
    else if (symbol == 7)   // AgreeMessage from server
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
            Start = true;
            UI->load(":/Resource/UI_Design/Board_UI.png");
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
    else if (symbol == 10)   // BackMessage from server
    {
        // Your friend has left
        client_hide();
        Initialize(false);
    }
    else if (symbol == 12)   // DeadlockMessage from server
    {
        // deadlock
        ReGame_request->show();
        Traceback->setEnabled(false);
        // Inf
        ui->pieceInf_1->setText(QString::fromLocal8Bit("双方进入僵局"));
        ui->pieceInf_2->setText(QString::fromLocal8Bit("双方进入僵局"));
        ui->pieceInf_2->setStyleSheet("color:red");
        ui->pieceInf_1->setStyleSheet("color:red");
    }
    else if (symbol == 14)   // SettingsMessage from server
    {
        server_settings_data.voice_package = Data[1];
        server_settings_data.tone = Data[2];
        server_settings_data.provocative = Data[3];
        server_settings_data.move = Data[4];
        server_settings_data.emoji = Data[5];
        server_settings_data.num = Data[6];
        server_settings_data.auto_atack = Data[7];
        server_settings_data.auto_counter = Data[8];
        Data.remove(0, 9);
        server_settings_data.player_name = Data;
        settings_Initialize();
    }
    else if (symbol == 15) // NameMessage from server
    {
        // QString read
        //QString name;
        //name = name.fromUtf8(Data);
        //server_settings_data.player_name = name;
        // show
        //Inf_Initialize();
    }
    else if (symbol == 18)  // TraceMessage from server
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
            if (Order)
                board->Traceback();
            else
            {
                board->Traceback();
                board->Traceback();
            }
            update();
            Order = false;
            // Inf
            ui->pieceInf_1->setText(QString::fromLocal8Bit("等待对方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("轮到对方落子"));
            ui->pieceInf_1->setStyleSheet("color:black");
            ui->pieceInf_2->setStyleSheet("color:red");
            if (board->GetPiecesCount() == 0)
                Send_Allow_TraceMessage(false);
            else
                Send_Allow_TraceMessage(true);
        }
        else
            Send_Allow_TraceMessage(false);
    }
    else if (symbol == 20)  // Allow_TraceMessage from server
    {
        ui->back_Button->setEnabled(true);
        if (Data[1])
        {
            // Agreed by server
            if (Order)
            {
                board->Traceback();
                board->Traceback();
            }
            else
                board->Traceback();
            Order = true;
            // Inf
            ui->pieceInf_1->setText(QString::fromLocal8Bit("轮到我方落子"));
            ui->pieceInf_2->setText(QString::fromLocal8Bit("等待我方落子"));
            ui->pieceInf_1->setStyleSheet("color:red");
            ui->pieceInf_2->setStyleSheet("color:black");
            update();
        }
        else
        {
            // Refused by server
        }
        Start = true;
    }
}

/* Send */

// 1. Send pieceMessage from Client
void PVP_Client::Send_PieceMessage(int put_row, int put_column)
{
    QByteArray Data;
    Data[0] = 1;   // 1 means pieceMessage from Client
    Data[1] = put_row;
    Data[2] = put_column;
    myClient->write(Data);     // send message
}

// 4. Send VictoryMessage from client
void PVP_Client::Send_VictoryMessage(int put_row, int put_column)
{
    QByteArray Data;
    Data[0] = 4;   // 4 means VictoryMessage from client
    Data[1] = put_row;
    Data[2] = put_column;
    myClient->write(Data);     // send message
}

// 5. Send RegameMessage from client
void PVP_Client::Send_RegameMessage(void)
{
    ReGame_request->setEnabled(false);
    ui->back_Button->setEnabled(false);
    QByteArray Data;
    Data[0] = 5;   // 5 means RegameMessage from client
    myClient->write(Data);     // send message
}

// 8. Send AgreeMessage from client
void PVP_Client::Send_AgreeMessage(bool Status_Agree)
{
    QByteArray Data;
    Data[0] = 8;
    Data[1] = (int)Status_Agree;   // 8 means AgreeMessage from client
    myClient->write(Data);     // send message
}

// 9. Send BackMessage from client
void PVP_Client::Send_BackMessage(void)
{
    QByteArray Data;
    Data[0] = 9;    // 9 means BackMessage from client
    myClient->write(Data);     // send message
}

// 11. Send DeadlockMessage from client
void PVP_Client::Send_DeadlockMessage(void)
{
    QByteArray Data;
    Data[0] = 11;    // 11 means DeadlockMessage from client
    myClient->write(Data);     // send message
}

// 13. Send SettingsMessage from client
void PVP_Client::Send_SettingsMessage(void)
{
    QByteArray Data;
    Data[0] = 13;    // 13 means SettingsMessage from client
    // other data
    Data[1] = client_settings_data.voice_package;
    Data[2] = client_settings_data.tone;
    Data[3] = client_settings_data.provocative;
    Data[4] = client_settings_data.move;
    Data[5] = client_settings_data.emoji;
    Data[6] = client_settings_data.num;
    Data[7] = client_settings_data.auto_atack;
    Data[8] = client_settings_data.auto_counter;
    Data.append(client_settings_data.player_name);
    myClient->write(Data);     // send message
}

// 16. Send NameMessage from client
void PVP_Client::Send_NameMessage(void)
{
    //QByteArray Data;
    //Data[0] = 16;   // 16 means NameMessage from client
    //Data.append(client_settings_data.player_name);
    //myClient->write(Data);     // send message
}

// 17. Send TraceMessage from client
void PVP_Client::Send_TraceMessage(void)
{
    ui->back_Button->setEnabled(false);
    QByteArray Data;
    Data[0] = 17;   // 17 means NameMessage from client
    myClient->write(Data);     // send message
}

// 19. Send Allow_TraceMessage from client
void PVP_Client::Send_Allow_TraceMessage(bool Status_Allow)
{
    QByteArray Data;
    Data[0] = 19;   // 19 means Allow_TraceMessage from client
    Data[1] = Status_Allow;
    myClient->write(Data);     // send message
}

/*----------------- PVP_Game(server) -------------------*/

/* --------- Interaction ---------- */

/* MouseEvent */
void PVP_Client::mousePressEvent(QMouseEvent* event)
{
    if (Start && event->button() == Qt::LeftButton)  // Left_Press
    {
        static int press_x = 0, press_y = 0, standard_x = 0, standard_y = 0, put_column = 0, put_row = 0;
        press_x = event->x();
        press_y = event->y();
        if (Order && press_x >= board->GetLeftMargin() - board->GetRange() && press_x <= board->GetLeftMargin() + board->GetLength() + board->GetRange() && press_y >= board->GetTopMargin() - board->GetRange() && press_y <= board->GetTopMargin() + board->GetWidth() + board->GetRange())
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
                    Order = false;
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
                        Traceback->setEnabled(false);
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
                        Traceback->setEnabled(false);
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
void PVP_Client::paintEvent(QPaintEvent* event)
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
void PVP_Client::on_back_Button_clicked(void)
{
    client_hide();
    Send_BackMessage();
    myClient->waitForReadyRead();
    Initialize(false);
}

// ReGame
void PVP_Client::on_ReGame_request_clicked(void)
{
    Send_RegameMessage();
}

// traceback
void PVP_Client::on_Traceback_clicked()
{
    Start = false;
    Send_TraceMessage();
    trace_chance--;
    Traceback->setEnabled(false);
    ui->Trace_Inf->setText(QString::fromLocal8Bit("悔棋机会已用完"));
    ui->Trace_Inf->setStyleSheet("color:grey;background:rgb(247,248,250)");
}