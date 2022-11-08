#include "Fun_4.h"


/* Initialize */
Fun_4::Fun_4(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);

}

// Slot
/* Confirm */
void Fun_4::on_Back_Button_clicked()
{
	this->hide();
}