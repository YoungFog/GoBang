#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Gobang.h"

#include "Fun_4.h"  // Fun_4
#include "../GameFunction/AI.h"    // Mode1
#include "../GameFunction/PVP_UI.h" // Mode2

#include "../Animation/AnimationButton.h"

#include <QMouseEvent>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>

#include <QTimer>


class Gobang : public QMainWindow
{
    Q_OBJECT

public:
    Gobang(QWidget *parent = Q_NULLPTR);
    void BlurEnter(void);    // (Animation)Enter with blur from 0 to 1

protected:
    void mousePressEvent(QMouseEvent*); // MouseEvent

/* SLOT FUNC */
private slots:
    void on_EndButton_clicked();    // End Game
    void on_Theme_clicked();    // Theme change
    void on_Developer_clicked();    // Show Developer Information
    void on_FuncButton1_clicked();    // Fun1
    void on_FuncButton2_clicked();    // Fun2
    void on_FuncButton3_clicked();    // Fun3
    void on_FuncButton4_clicked();    // Fun4
    void on_ModeButton1_clicked();    // Mode1
    void on_ModeButton2_clicked();    // Mode2
    void on_ModeButton3_clicked();    // Mode3
    void M1_Reset();  // back from M1
    void M2_Reset();  // back from M2

private:
    Ui::GobangClass ui;
    Fun_4 *F4;
    AI *M1;
    PVP_UI *M2;

    /* Status */
    bool blue = false;

    /* Button */
    QAnimationButton* Mode1;
    QAnimationButton* Mode2;
    QAnimationButton* Exit;
    QAnimationButton* Theme;

    /* Icon */
    QIcon icon_orange;
    QIcon icon_orange_2;
    QIcon icon_orange_3;
    QIcon icon_orange_4;
    QIcon icon_blue;
    QIcon icon_blue_2;
    QIcon icon_blue_3;
    QIcon icon_blue_4;
};
