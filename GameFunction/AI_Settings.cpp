#include "AI_Settings.h"

AI_Settings::AI_Settings(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	// wipe off frame
	this->setWindowFlags(Qt::FramelessWindowHint);
	cancel = false;
	challenger_name = QString::fromLocal8Bit("无名英雄");
	difficulty = 0;
	order_isFirst = true;
	/* Animation_Button */
	// continue_button
	continue_button = new QAnimationButton(this);
	continue_button->SizeSet(1);
	continue_button->setGeometry(879, 895, 161, 50);
	QIcon icon;
	icon.addFile(tr(":/Resource2/UI_Design/AI_settings_start.png"));
	continue_button->setIcon(icon);
	continue_button->setIconSize(QSize(161, 50));
	connect(continue_button, SIGNAL(clicked()), this, SLOT(on_continue_button_clicked()));
}

AI_Settings::~AI_Settings()
{
}

void AI_Settings::on_back_button_clicked(void)
{
	cancel = true;
	close();
}

void AI_Settings::on_continue_button_clicked(void)
{
	// name
	if(ui.challenger_name->document()->isEmpty())
		challenger_name = QString::fromLocal8Bit("无名英雄");
	else
		challenger_name =ui.challenger_name->toPlainText();
	// order
	if (ui.isFirst->isChecked())
		order_isFirst = true;
	else
		order_isFirst = false;
	// difficulty
	if (ui.diff1_easy->isChecked())
		difficulty = 0;
	else if (ui.diff2_normal->isChecked())
		difficulty = 1;
	else if (ui.diff3_hard->isChecked())
		difficulty = 2;
	// continue
	cancel = false;
	close();
}