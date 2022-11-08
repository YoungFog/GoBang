#pragma once

#include <QWidget>
#include "ui_PVP_UI.h"

#include "PVP_Client.h"
#include "PVP_Server.h"
#include "PVP_Settings.h"
#include "pvp_Create.h"
#include "pvp_Join.h"

#include "../Animation/AnimationButton.h"

class PVP_UI : public QWidget
{
	Q_OBJECT

signals:
	void M2_hide();

public:
	PVP_UI(QWidget *parent = Q_NULLPTR);
	~PVP_UI();


protected slots:
	void Create_Back();
	void Create_Continue();
	void Join_Back();
	void Join_Continue();
	void PVP_hide();

/* SLOT FUNC */
private slots:
	void on_PVP_BackButton_clicked();    // Back
	void on_PVP_Create_clicked();    // Create
	void on_PVP_Join_clicked();    // Join
	void on_PVP_Setting_clicked();    // Settings
	void hide_process();	// if server or client hides, Main_UI shows

private:
	Ui::PVP_UI ui;
	PVP_Settings *settings;
	PVP_Client *Client_UI;
	PVP_Server *Server_UI;
	pvp_Create* Create;
	pvp_Join* Join;
	Settings_Data settings_data;
	
	/* Button */
	QAnimationButton* PVP_Create;
	QAnimationButton* PVP_Join;
	QAnimationButton* PVP_Setting;

	/* Icon */
	QIcon icon_PVP_Create;
	QIcon icon_PVP_Join;
	QIcon icon_PVP_Setting;
};
