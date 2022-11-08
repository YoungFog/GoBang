#pragma once

#include <QDialog>
#include "ui_PVP_Settings.h"

class Settings_Data
{
public:
	Settings_Data();
	int voice_package;
	QString player_name;
	int tone;
	bool provocative;
	int move;
	int emoji;
	int num;
	bool auto_atack;
	bool auto_counter;
};

class PVP_Settings : public QDialog
{
	Q_OBJECT

public:
	PVP_Settings(QWidget *parent = Q_NULLPTR);
	~PVP_Settings();
	Settings_Data settings_data;
	bool IsReadable();
	void PVP_Settings::ReadFromSettings(Settings_Data&);	// Read settings_data

private slots:
	void on_confirm_clicked();  // save
	void on_cancel_clicked();  // cancel

private:
	Ui::PVP_Settings ui;
	bool readable;
};
