#include "PVP_UI.h"
#include <QGraphicsBlurEffect>
#include "password_Error.h"

PVP_UI::PVP_UI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);
	settings = new PVP_Settings(this);
	Client_UI = new PVP_Client(this);
	Server_UI = new PVP_Server(this);
	Create = new pvp_Create(this);
	Join = new pvp_Join(this);
	settings->hide();
	Client_UI->hide();
	Server_UI->hide();
	Create->hide();
	Join->hide();
	/* signals and slots */
	connect(Client_UI, SIGNAL(pvp_client_hide()), this, SLOT(hide_process()));
	connect(Server_UI, SIGNAL(pvp_server_hide()), this, SLOT(hide_process()));
	/* Animation_Button */
	// Icon
	icon_PVP_Setting.addFile(tr(":/Resource/UI_Design/PVP_Setting.png"));
	icon_PVP_Create.addFile(tr(":/Resource/UI_Design/PVP_Create.png"));
	icon_PVP_Join.addFile(tr(":/Resource/UI_Design/PVP_Join.png"));
	// PVP_Setting
	PVP_Setting = new QAnimationButton(ui.Main_UI);
	PVP_Setting->SizeSet(1);
	PVP_Setting->setGeometry(880, 915, 161, 50);
	PVP_Setting->setIcon(icon_PVP_Setting);
	PVP_Setting->setIconSize(QSize(161, 50));
	PVP_Setting->setFlat(true);
	connect(PVP_Setting, SIGNAL(clicked()), this, SLOT(on_PVP_Setting_clicked()));
	// PVP_Create
	PVP_Create = new QAnimationButton(ui.Main_UI);
	PVP_Create->SizeSet(2);
	PVP_Create->setGeometry(566, 460, 350, 220);
	PVP_Create->setIcon(icon_PVP_Create);
	PVP_Create->setIconSize(QSize(350, 220));
	PVP_Create->setFlat(true);
	connect(PVP_Create, SIGNAL(clicked()), this, SLOT(on_PVP_Create_clicked()));
	// PVP_Join
	PVP_Join = new QAnimationButton(ui.Main_UI);
	PVP_Join->SizeSet(2);
	PVP_Join->setGeometry(1000, 460, 350, 220);
	PVP_Join->setIcon(icon_PVP_Join);
	PVP_Join->setIconSize(QSize(350, 220));
	PVP_Join->setFlat(true);
	connect(PVP_Join, SIGNAL(clicked()), this, SLOT(on_PVP_Join_clicked()));
}

PVP_UI::~PVP_UI()
{
	delete Client_UI;
	delete Server_UI;
	delete settings;
	delete Create;
	delete Join;
}

void PVP_UI::PVP_hide()
{
	hide();
	emit M2_hide();
}

// Back
void PVP_UI::on_PVP_BackButton_clicked()
{
	PVP_hide();
}

// Create UI
void PVP_UI::on_PVP_Create_clicked()
{
	static bool once = true;
	ui.Main_UI->hide();
	Create->show();
	// Slot
	if (once)
	{
		connect(Create->get_ui()->Back_Button, SIGNAL(clicked()), this, SLOT(Create_Back()));
		connect(Create->get_ui()->Create_Button, SIGNAL(clicked()), this, SLOT(Create_Continue()));
		once = false;
	}
}

// Join UI
void PVP_UI::on_PVP_Join_clicked()
{
	static bool once = true;
	ui.Main_UI->hide();
	Join->show();
	// Slot
	if (once)
	{
		connect(Join->get_ui()->back_button, SIGNAL(clicked()), this, SLOT(Join_Back()));
		connect(Join->get_ui()->Join_Button, SIGNAL(clicked()), this, SLOT(Join_Continue()));
		once = false;
	}
}

void PVP_UI::Create_Back(void)
{
	Create->hide();
	ui.Main_UI->show();
}

void PVP_UI::Create_Continue(void)
{
	/* show */
	if (!(Create->get_ui()->password->document()->isEmpty()))
	{
		Create->hide();
		ui.Main_UI->show();
		// Transmit Data
		Server_UI->ip_Process(Create->getRoom());
		Server_UI->port_Process(Create->get_ui()->password->toPlainText());
		// server(show included)
		ui.Main_UI->hide();
		Server_UI->ip_Listen();
	}
	else
	{
		/* empty ip/port warning */
		password_Error* w;
		w = new password_Error(Create);
		QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
		Create->get_ui()->password->hide();
		blureffect->setBlurRadius(50);
		Create->get_ui()->Main_UI->setGraphicsEffect(blureffect);
		w->exec();
		Create->get_ui()->password->show();
		blureffect->setBlurRadius(0);
	}
}

void PVP_UI::Join_Back(void)
{
	Join->hide();
	ui.Main_UI->show();
}

void PVP_UI::Join_Continue(void)
{
	/* show */
	if (!(Join->get_ui()->room_num->document()->isEmpty()) && !(Join->get_ui()->password->document()->isEmpty()))
	{
		Join->hide();
		ui.Main_UI->show();
		// Transmit Data
		Client_UI->ip_Process(Join->get_ui()->room_num->toPlainText());
		Client_UI->port_Process(Join->get_ui()->password->toPlainText());
		ui.Main_UI->hide();
		// client(show included)
		if (!Client_UI->Connect_To_Server())
		{
			ui.Main_UI->show();
			Join->show();
			ui.Main_UI->hide();
			password_Error* w;
			w = new password_Error(Join);
			QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
			blureffect->setBlurRadius(50);
			Join->setGraphicsEffect(blureffect);
			Join->get_ui()->password->hide();
			Join->get_ui()->Main_UI->setGraphicsEffect(blureffect);    // Blur
			w->exec();
			Join->get_ui()->password->show();
			blureffect->setBlurRadius(0);
		}
	}
	else
	{
		/* empty ip/port warning */
		password_Error* w;
		w = new password_Error(Join);
		QGraphicsBlurEffect* blureffect = new QGraphicsBlurEffect(this);
		blureffect->setBlurRadius(50);
		Join->setGraphicsEffect(blureffect);
		Join->get_ui()->password->hide();
		Join->get_ui()->Main_UI->setGraphicsEffect(blureffect);    // Blur
		w->exec();
		Join->get_ui()->password->show();
		blureffect->setBlurRadius(0);
	}
}


// Settings
void PVP_UI::on_PVP_Setting_clicked()
{
	settings->show();
	ui.Main_UI->hide();
	settings->exec();
	ui.Main_UI->show();
	/* Transmit data */
	if (settings->IsReadable())
	{
		//settings_data
		settings->ReadFromSettings(settings_data);
		// Save Success Dialog
		Server_UI->ReadSettings(settings_data);
		Client_UI->ReadSettings(settings_data);
	}
}

// if server or client hides, Main_UI shows
void PVP_UI::hide_process()
{
	ui.Main_UI->show();
}
