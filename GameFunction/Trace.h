#pragma once

#include <QDialog>
#include "ui_Trace.h"

class Trace : public QDialog
{
	Q_OBJECT

public:
	Trace(QWidget *parent = Q_NULLPTR);
	~Trace();
	bool IsAgree();

private slots:
	void on_Agree_clicked();  // agree
	void on_Refuse_clicked();  // refuse

private:
	Ui::Trace ui;
	bool status_agree;
};
