#pragma once

#include <QWidget>
#include "ui_pvp_Create.h"

#include <QTcpServer>
#include <QNetworkInterface>

class pvp_Create : public QWidget
{
	Q_OBJECT

public:
	pvp_Create(QWidget *parent = Q_NULLPTR);
	~pvp_Create();
	/* Status */
	QString getRoom();
	QString getPassword();
	Ui::pvp_Create *get_ui();


private:
	Ui::pvp_Create ui;
	QString room_num;
	QString password;
};
