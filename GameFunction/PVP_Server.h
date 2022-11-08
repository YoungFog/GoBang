#pragma once

#include <QWidget>
#include "ui_PVP_Server.h"
#include <QTcpServer>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QTcpSocket>

#include "PVP_Settings.h"
#include "base_Game.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <qpixmap.h>

#include "Waiting_UI.h"
#include "../Animation/AnimationButton.h"


class PVP_Server : public QWidget
{
	Q_OBJECT
signals:
    void pvp_server_hide();

public:
	PVP_Server(QWidget *parent = Q_NULLPTR);
	~PVP_Server();
    /* NetWork */
    QString ip_Process(QString default = "");
    QString port_Process(QString default = "");
    bool ip_Listen();   // listen ip
    /* Settings */
    void ReadSettings(Settings_Data const &);   // settings read
    

protected:
    void mousePressEvent(QMouseEvent*); // MouseEvent
    void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;	// Paint Piece
    void Initialize(bool unconnected = false);   // Initialize
    void Inf_Initialize();  // Inf_Initialize
    /* Game */
    void Send_PieceMessage(int put_row, int put_column);    // 0. Send PieceMessage from Server
    void Send_OrderMessage(int order);    // 2. Send OrderMessage from Server
    void Send_VictoryMessage(int put_row, int put_column);    // 3. Send VictoryMessage from Server
    void Send_RegameMessage();  // 6. Send RegameMessage from Server
    void Send_AgreeMessage(bool Status_Agree);   // 7. Send AgreeMessage from Server
    void Send_BackMessage();    // 10. Send BackMessage from server
    void Send_DeadlockMessage();    // 12. Send DeadlockMessage from server
    void Send_SettingsMessage();    // 14. Send SettingsMessage from server
    void Send_NameMessage();    // 15. Send NameMessage from server
    void Send_TraceMessage();   // 18. Send TraceMessage from server
    void Send_Allow_TraceMessage(bool Status_Allow); // 20. Send Allow_TraceMessage from server
    /* Hide */
    void server_hide();

private slots:
    void ProcessNewConnection();     // new connection signal
    void ProcessDisconnected();      // Disconnected from client
    void ProcessReadyRead();     // read client
    void on_back_Button_clicked();  // back
    void on_ReGame_request_clicked();  // ReGame
    void on_Traceback_clicked();	// traceback

private:
	Ui::PVP_Server* ui;
    QTcpServer* myServer;   // Define server
    QTcpSocket* cur_socket;     // Define Client descriptor

    /* Settings */
    Settings_Data server_settings_data;
    Settings_Data client_settings_data;
    void settings_Initialize();

    /* NetWork Information */
    QString server_ip;
    QString server_port;

    /* Game Information */
    Board* board;
    QPixmap* UI;
    Waiting_UI* waiting;
    int trace_chance;
    bool Start;
    bool server_color;

    /* Button */
    QAnimationButton* ReGame_request;
    QAnimationButton* Traceback;

    /* Icon */
    QIcon icon_ReGame_request;
    QIcon icon_Traceback;
};
