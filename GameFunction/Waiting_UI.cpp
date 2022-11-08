#include "Waiting_UI.h"

Waiting_UI::Waiting_UI(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	cancel = false;
	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);
}

Waiting_UI::~Waiting_UI()
{
}

void Waiting_UI::on_back_button_clicked(void)
{
	cancel = true;
	close();
}

void Waiting_UI::TextSet(QString ip_text)
{
	ui.ip_Tip->setText(ip_text);
}

void Waiting_UI::TextSet2(QString port_text)
{
	ui.ip_Tip_2->setText(port_text);
}