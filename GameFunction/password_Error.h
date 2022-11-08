#pragma once

#include <QDialog>
#include "ui_password_Error.h"

class password_Error : public QDialog
{
	Q_OBJECT

public:
	password_Error(QWidget *parent = Q_NULLPTR);
	~password_Error();

private slots:
	void on_confirm_clicked();

private:
	Ui::password_Error ui;
};
