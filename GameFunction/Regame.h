#pragma once

#include <QDialog>
#include "ui_Regame.h"

class Regame : public QDialog
{
	Q_OBJECT

public:
	Regame(QWidget *parent = Q_NULLPTR);
	~Regame();
	bool IsAgree();

private slots:
	void on_Agree_clicked();  // agree
	void on_Refuse_clicked();  // refuse

private:
	Ui::Regame ui;
	bool status_agree;
};
