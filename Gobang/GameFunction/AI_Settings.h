#pragma once

#include <QDialog>
#include "ui_AI_Settings.h"
#include "../Animation/AnimationButton.h"

class AI_Settings : public QDialog
{
	Q_OBJECT

public:
	AI_Settings(QWidget *parent = Q_NULLPTR);
	~AI_Settings();
	bool cancel;	// if back
	/* settings data */
	QString challenger_name;
	int difficulty;	// 0 - easy, 1 - normal  2- hard
	bool order_isFirst;

private slots:
	void on_back_button_clicked();	// back
	void on_continue_button_clicked();	// continue

private:
	Ui::AI_Settings ui;

	/* button */
	QAnimationButton* continue_button;
};
