#pragma once

#include <QDialog>
#include "ui_Fun_4.h"

class Fun_4 : public QDialog
{
	Q_OBJECT

public:
	Fun_4(QWidget *parent = Q_NULLPTR);


/* SLOT FUNC */
private slots:
	void on_Back_Button_clicked();    // Back

private:
	Ui::Fun_4 ui;
};
