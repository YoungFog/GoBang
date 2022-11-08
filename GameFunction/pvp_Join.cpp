#include "pvp_Join.h"

pvp_Join::pvp_Join(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);
	room_num = "empty";
	password = "empty";
}

pvp_Join::~pvp_Join()
{
}

QString pvp_Join::getRoom(void)
{
	return room_num;
}

QString pvp_Join::getPassword(void)
{
	return password;
}

Ui::pvp_Join *pvp_Join::get_ui(void)
{
	return &ui;
}
