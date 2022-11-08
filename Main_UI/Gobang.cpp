#include "Gobang.h"

/* Initialize UI */
Gobang::Gobang(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("宿舍战争-五子棋"));
    // wipe off frame
    this->setWindowFlags(Qt::FramelessWindowHint);
    M1 = new AI(this);
    M2 = new PVP_UI(this);
    F4 = new Fun_4(this);
    M1->hide();
    M2->hide();
    F4->hide();
    connect(M1, SIGNAL(M1_hide()), this, SLOT(M1_Reset()));
    connect(M2, SIGNAL(M2_hide()), this, SLOT(M2_Reset()));

    /* Animation_Button */
    // Icon_blue
    icon_blue.addFile(tr(":/Resource2/UI_Design/Mode1_blue.png"));
    icon_blue_2.addFile(tr(":/Resource2/UI_Design/Mode2_blue.png"));
    icon_blue_3.addFile(tr(":/Resource2/UI_Design/Exit_blue.png"));
    icon_blue_4.addFile(tr(":/Resource2/UI_Design/Theme_blue.png"));
    // Icon_orange
    icon_orange.addFile(tr(":/Resource/UI_Design/Mode1.png"));
    icon_orange_2.addFile(tr(":/Resource/UI_Design/Mode2.png"));
    icon_orange_3.addFile(tr(":/Resource/UI_Design/EndGame.png"));
    icon_orange_4.addFile(tr(":/Resource/UI_Design/Theme.png"));
    // mode_1
    Mode1 = new QAnimationButton(ui.Main_UI);
    Mode1->SizeSet(0);
    Mode1->setGeometry(1441, 281, 373, 96);
    Mode1->setIcon(icon_orange);
    Mode1->setIconSize(QSize(373, 96));
    Mode1->setFlat(true);
    connect(Mode1, SIGNAL(clicked()), this, SLOT(on_ModeButton1_clicked()));
    // mode_2
    Mode2 = new QAnimationButton(ui.Main_UI);
    Mode2->SizeSet(0);
    Mode2->setGeometry(1441, 480, 373, 96);
    Mode2->setIcon(icon_orange_2);
    Mode2->setIconSize(QSize(373, 96));
    Mode2->setFlat(true);
    connect(Mode2, SIGNAL(clicked()), this, SLOT(on_ModeButton2_clicked()));
    // Exit
    Exit = new QAnimationButton(ui.Main_UI);
    Exit->SizeSet(1);
    Exit->setGeometry(325, 742, 142, 57);
    Exit->setIcon(icon_orange_3);
    Exit->setIconSize(QSize(142, 57));
    Exit->setFlat(true);
    connect(Exit, SIGNAL(clicked()), this, SLOT(on_EndButton_clicked()));
    // Theme
    Theme = new QAnimationButton(ui.Main_UI);
    Theme->SizeSet(1);
    Theme->setGeometry(136, 742, 142, 57);
    Theme->setIcon(icon_orange_4);
    Theme->setIconSize(QSize(142, 57));
    Theme->setFlat(true);
    connect(Theme, SIGNAL(clicked()), this, SLOT(on_Theme_clicked()));

    /* shadow */
    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(4);
    shadow_effect->setColor(QColor(0, 0, 0, 60));
    shadow_effect->setBlurRadius(5);
    Mode1->setGraphicsEffect(shadow_effect);
    QGraphicsDropShadowEffect* shadow_effect2 = new QGraphicsDropShadowEffect(this);
    shadow_effect2->setOffset(4);
    shadow_effect2->setColor(QColor(0, 0, 0, 60));
    shadow_effect2->setBlurRadius(5);
    Mode2->setGraphicsEffect(shadow_effect2);
}


/* (Animation)Enter with blur from 0 to 1 */
void Gobang::BlurEnter(void)
{
    // QTimer::singleShot(1000, [=]() {; });
    // ui.Background->setStyleSheet(QString::fromUtf8("image:url(:/Resouce/UI_Design/Main UI.png)"));
    /* Animation Set */
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}


/* MouseEvent */
void Gobang::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)  // Left_Press
    {
        ;
    }
}



/* -----Parting Line----- */



/* SLOT (UI_BUTTON) START */


/* End Game */
void Gobang::on_EndButton_clicked()
{
    /* Exit Animation */
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    QTimer::singleShot(1000, [=]() { this->close(); });
}

/* Show Developer Information */
void Gobang::on_Developer_clicked()
{
    ui.Main_UI->hide();
    F4->show();
    F4->exec();
    ui.Main_UI->show();
}

/* Fun1 */
void Gobang::on_FuncButton1_clicked()
{
    
}

/* Fun2 */
void Gobang::on_FuncButton2_clicked()
{
    
}

/* Fun3 */
void Gobang::on_FuncButton3_clicked()
{
    ui.Main_UI->hide();
    F4->show();
    F4->exec();
    ui.Main_UI->show();
}

/* Fun4 */
void Gobang::on_FuncButton4_clicked()
{
    ui.Main_UI->hide();
    F4->show();
    F4->exec();
    ui.Main_UI->show();
}

/* Mode1 */
void Gobang::on_ModeButton1_clicked()
{
    M1->show();
    ui.Main_UI->hide();
    M1->Launch();
}

// back from M1
void Gobang::M1_Reset()
{
    ui.Main_UI->show();
}

/* Mode2 */
void Gobang::on_ModeButton2_clicked()
{
    M2->show();
    ui.Main_UI->hide();
}

// back from M2
void Gobang::M2_Reset()
{
    ui.Main_UI->show();
}

/* Mode3 */
void Gobang::on_ModeButton3_clicked()
{
    //
}

/* Start Game */
void Gobang::on_Theme_clicked()
{
    if (blue)
    {
        Mode1->setIcon(icon_orange);
        Mode2->setIcon(icon_orange_2);
        Exit->setIcon(icon_orange_3);
        Theme->setIcon(icon_orange_4);
        ui.Background->setStyleSheet("image: url(:/Resource3/UI_Design/Main UI.png);");
        blue = false;
    }
    else
    {
        Mode1->setIcon(icon_blue);
        Mode2->setIcon(icon_blue_2);
        Exit->setIcon(icon_blue_3);
        Theme->setIcon(icon_blue_4);
        ui.Background->setStyleSheet("image: url(:/Resource3/UI_Design/Main_UI_blue.png);");
        blue = true;
    }
}


/* SLOT(UI_BUTTON) END */
