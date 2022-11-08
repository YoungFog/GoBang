#include "PVP_Settings.h"
#include <QGraphicsDropShadowEffect>


/* Settings_Data Class */

Settings_Data::Settings_Data(void)
{
    voice_package = 0;
    player_name = QString::fromLocal8Bit("无名玩家");
    tone = 0;
    provocative = false;
    move = 0;
    emoji = 0;
    num = 0;
    auto_atack = false;
    auto_counter = false;
}


PVP_Settings::PVP_Settings(QWidget *parent)
	: QDialog(parent)
{
    readable = false;
	ui.setupUi(this);
	// wipe off frame
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | windowFlags());
	setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);  // shadow distance
    shadow->setColor(Qt::black); // shadow color
    shadow->setBlurRadius(20);  // radius
    QWidget* widget = new QWidget(this);
    widget->setObjectName("widgetShadow");
    widget->setGraphicsEffect(shadow);  // shadow
    widget->setStyleSheet("#widgetShadow{background:#f5f5f5;border-radius:5px;}");
    QSize s = size();
    widget->resize(s);
    resize(s.width() + 20, s.height() + 20);
    widget->move(10, 10);
    for (QWidget* w : findChildren<QWidget*>())
    {
        if (w != widget)
        {
            if (w->parent() == this)
            {
                QRect r = w->geometry();
                w->setParent(widget);
                w->setGeometry(r);
            }
        }
    }
}

PVP_Settings::~PVP_Settings()
{
}


bool PVP_Settings::IsReadable(void)
{
    return readable;
}

/* Confirm */
void PVP_Settings::on_confirm_clicked(void)
{
    close();
    readable = true;
    /* Voice_Package */
    if (ui.NO1->isChecked())
        settings_data.voice_package = 0;
    else if (ui.NO2->isChecked())
        settings_data.voice_package = 1;
    else if (ui.NO3->isChecked())
        settings_data.voice_package = 2;
    else if (ui.NO4->isChecked())
        settings_data.voice_package = 3;
    /* Name */
    if (ui.Name_Edit->document()->isEmpty())
        settings_data.player_name = QString::fromLocal8Bit("无名玩家");
    else
        settings_data.player_name = ui.Name_Edit->toPlainText();
    /* bool */
    if (ui.Provocative->isChecked())
        settings_data.provocative = true;
    else
        settings_data.provocative = false;

    if (ui.auto_attack->isChecked())
        settings_data.auto_atack = true;
    else
        settings_data.auto_atack = false;

    if (ui.auto_counter->isChecked())
        settings_data.auto_counter = true;
    else
        settings_data.auto_counter = false;
    /* int */
    settings_data.tone = ui.tone_Setting->currentIndex();
    settings_data.move = ui.move_Setting->currentIndex();
    settings_data.emoji = ui.emoji_Setting->currentIndex();
    settings_data.num = ui.num_Setting->currentIndex();
}

/* Cancel */
void PVP_Settings::on_cancel_clicked(void)
{
    close();
    readable = false;
    /* Voice_Package */
    if (settings_data.voice_package == 0)
        ui.NO1->setChecked(true);
    else if (settings_data.voice_package == 1)
        ui.NO2->setChecked(true);
    else if (settings_data.voice_package == 2)
        ui.NO3->setChecked(true);
    else if (settings_data.voice_package == 3)
        ui.NO4->setChecked(true);

    /* Name */
    if (settings_data.player_name == "无名玩家")
        ui.Name_Edit->clear();
    else
        ui.Name_Edit->setText(settings_data.player_name);

    /* bool */
    if (settings_data.provocative == true)
        ui.Provocative->setCheckState(Qt::Checked);
    else
        ui.Provocative->setCheckState(Qt::Unchecked);

    if (settings_data.auto_atack == true)
        ui.auto_attack->setCheckState(Qt::Checked);
    else
        ui.auto_attack->setCheckState(Qt::Unchecked);

    if (settings_data.auto_counter == true)
        ui.auto_counter->setCheckState(Qt::Checked);
    else
        ui.auto_counter->setCheckState(Qt::Unchecked);

    /* int */
    ui.tone_Setting->setCurrentIndex(settings_data.tone);
    ui.move_Setting->setCurrentIndex(settings_data.move);
    ui.emoji_Setting->setCurrentIndex(settings_data.emoji);
    ui.num_Setting->setCurrentIndex(settings_data.num);
}

/* Read settings_data */
void PVP_Settings::ReadFromSettings(Settings_Data& user_settings_data)
{
    user_settings_data.voice_package = settings_data.voice_package;
    user_settings_data.player_name = settings_data.player_name;
    user_settings_data.tone = settings_data.tone;
    user_settings_data.provocative = settings_data.provocative;
    user_settings_data.move = settings_data.move;
    user_settings_data.emoji = settings_data.emoji;
    user_settings_data.num = settings_data.num;
    user_settings_data.auto_atack = settings_data.auto_atack;
    user_settings_data.auto_counter = settings_data.auto_counter;
}