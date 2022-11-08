#pragma once

#include <QWidget>
#include "ui_PVP_Client.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QtCore/QObject>

#include "base_Game.h"
#include "PVP_Settings.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <qpixmap.h>
#include "../Animation/AnimationButton.h"
#include <QTcpServer>

class PVP_Client : public QWidget
{
	Q_OBJECT
signals:
    void pvp_client_hide();

public:
	PVP_Client(QWidget *parent = Q_NULLPTR);
	~PVP_Client();
    /* NetWork */
    QString ip_Process(QString default = "");
    QString port_Process(QString default = "");
    bool Connect_To_Server();
    /* Settings */
    void ReadSettings(Settings_Data const &);   // settings read

protected:
    void mousePressEvent(QMouseEvent*); // MouseEvent
    void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;	// Paint Piece
    void Initialize(bool unconnected = false);  // Initialize
    void Inf_Initialize();  // Inf_Initialize
    /* Game */
    void Send_PieceMessage(int put_row, int put_column);    // 1. Send pieceMessage from Client
    void Send_VictoryMessage(int put_row, int put_column);    // 4. Send VictoryMessage from client
    void Send_RegameMessage();    // 5. Send RegameMessage from client
    void Send_AgreeMessage(bool Status_Agree);   // 8. Send AgreeMessage from Client
    void Send_BackMessage();    // 9. Send BackMessage from client
    void Send_DeadlockMessage();    // 11. Send DeadlockMessage from client
    void Send_SettingsMessage();    // 13. Send SettingsMessage from client
    void Send_NameMessage();    // 16. Send NameMessage from client
    void Send_TraceMessage();   // 17. Send TraceMessage from client
    void Send_Allow_TraceMessage(bool Status_Allow); // 19. Send Allow_TraceMessage from client
    /* Hide */
    void client_hide();

private slots:
    void ProcessConnected();     // Connect Success
    void ProcessReadyRead();     // Read server
    void ProcessDisconnected();      // Disconnected
    void on_back_Button_clicked();  // back
    void on_ReGame_request_clicked();  // ReGame
    void on_Traceback_clicked();	// traceback

private:
	Ui::PVP_Client* ui;
    QTcpSocket* myClient;    // Define client

    /* Settings */
    Settings_Data client_settings_data;
    Settings_Data server_settings_data;
    void settings_Initialize();

    /* information */
    QString client_ip;
    QString client_port;

    /* Game Information */
    Board* board;
    QPixmap* UI;
    int trace_chance;
    bool Order; // piece allowed to put(Game Order)
    bool Start; // game allowed to start

    /* Button */
    QAnimationButton* ReGame_request;
    QAnimationButton* Traceback;

    /* Icon */
    QIcon icon_ReGame_request;
    QIcon icon_Traceback;
};
