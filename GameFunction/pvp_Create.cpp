#include "pvp_Create.h"

pvp_Create::pvp_Create(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);
	// Set IP
	QString myAddr;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();    // get all address
	for (int i = 0; i < ipAddressesList.length(); i++)
	{
		QHostAddress addr = ipAddressesList.at(i);
		if (addr.toString().contains("192.") || addr.toString().contains("172."))    // HFUT ip 172
			myAddr = addr.toString();
	}
	ui.Room->setText(myAddr);
	room_num = myAddr;
	password = "empty";
}

pvp_Create::~pvp_Create()
{
}

QString pvp_Create::getRoom(void)
{
	return room_num;
}

QString pvp_Create::getPassword(void)
{
	return password;
}

Ui::pvp_Create* pvp_Create::get_ui(void)
{
	return &ui;
}
