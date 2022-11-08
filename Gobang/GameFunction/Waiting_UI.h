#pragma once

#include <QDialog>
#include "ui_Waiting_UI.h"

class Waiting_UI : public QDialog
{
	Q_OBJECT

public:
	Waiting_UI(QWidget *parent = Q_NULLPTR);
	~Waiting_UI();
	bool cancel;
	void TextSet(QString ip_text);
	void TextSet2(QString port_text);

private slots:
	void on_back_button_clicked();	// back

private:
	Ui::Waiting_UI ui;
};
