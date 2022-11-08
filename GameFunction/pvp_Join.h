#pragma once

#include <QWidget>
#include "ui_pvp_Join.h"

class pvp_Join : public QWidget
{
	Q_OBJECT

public:
	pvp_Join(QWidget *parent = Q_NULLPTR);
	~pvp_Join();
	/* Status */
	QString getRoom();
	QString getPassword();
	Ui::pvp_Join *get_ui();

private:
	Ui::pvp_Join ui;
	QString room_num;
	QString password;
};
