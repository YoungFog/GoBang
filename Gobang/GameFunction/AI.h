#pragma once

#include <QWidget>
#include "ui_AI.h"
#include "base_Game.h"
#include "AI_Settings.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <qpixmap.h>

#include "../Animation/AnimationButton.h"


/* --------- AI_Evaluation --------- */
class Evaluation
{
public:
	Evaluation();
	int getScore();
	int getVictory_color();
	int* Get_killCount();	// 0-count_win, 1-count_lose, 2-count_flex4, 3-count_block4, 4-count_flex3
	void evaluate(int** pieces_array, bool color_gamer);
	void Data_Initialize();

private:
	/* code_name of chess form */
	const int invalid_Form = 0;	    // 0 (others are not considered)
	// self
	const int win_Form = 1;	        // 1
	const int self_flex4_Form = 2;	// 2
	const int self_block4_Form = 3;	// 3
	const int self_flex3_Form = 4;	// 4
	const int self_block3_Form = 5;	// 5
	const int self_flex2_Form = 6;	// 6
	const int self_block2_Form = 7;	// 7
	const int self_flex1_Form = 8;	// 8
	// opponent
	const int lose_Form = 9;	    // 9
	const int op_flex4_Form = 10;	// 10
	const int op_block4_Form = 11;	// 11
	const int op_flex3_Form = 12;	// 12
	const int op_block3_Form = 13;	// 13
	const int op_flex2_Form = 14;	// 14
	const int op_block2_Form = 15;	// 15
	const int op_flex1_Form = 16;	// 16

	/* Evaluation Data */
	int chessTemplate_tuple[4][4][4][4][4][4]; // [0-none 1-opponent 2-self 3-border]			  
	int weight[17] = {						   // (tuple) store all chess form as template tuple
		0,         // invalid_Form 0 
		1000000,   // win_Form 1
		50000,     // self_flex4_Form 2
		400,       // self_block4_Form 3
		400,       // self_flex3_Form 4
		20,        // self_block3_Form 5
		20,        // self_flex2_Form 6
		1,         // self_block2_Form 7
		1,         // self_flex1_Form 8
		-10000000, // lose_Form 9 
		-1000000,   // op_flex4_Form 10
		-1000000,   // op_block4_Form 11 
		-8000,     // op_flex3_Form 12
		-50,       // op_block3_Form 13
		-50,       // op_flex2_Form 14
		-3,        // op_block2_Form 15
		-3         // op_flex1_Form 16
	};
											  
	/* Evaluation Feedback Data */
	int score;
	int count_win, count_lose, count_flex4, count_block4, count_flex3;	// (self) Store key num
	int victory_color;
	int form_data[4][17];   // [0-row 1-column 2-right_down 3-left_down] num of all forms in 4 directions
	int border_pieces[17][17];   // [0-none 1-opponent 2-self 3-border] assume border exists
};


/* --------- AI_Function --------- */
class AI_Function
{
public:
	AI_Function();
	int setColor(int color = para_default);
	int* get_Decision(Piece** pieces, int search_depth);   // decision[0] -> r, decision[1] -> c

protected:
	Evaluation evaluation;
	void provide_Choice(int** pieces, bool self, int num);	// provide num best choice
	int Search(int** pieces, int cur_depth, int alpha, int beta, const int search_depth);	// Minimax Search(alpha-beta prune) 
	QList<QPoint> provide_kill_Choice(int** pieces);
	bool kill_Search(int**pieces, int depth);

private:
	int AI_color;
	int** pieces_int;	// transition from class Piece**
	int* decision;	// return form
	/* Final Decision */
	int curDecision_r;
	int curDecision_c;
	/* provide 20 best choice(sorted)(0--better) */
	int choice_r[20];
	int choice_c[20];
	int choice_score[20];
};



/* --------- AI_UI --------- */
class AI : public QWidget
{
	Q_OBJECT

signals:
	void M1_hide();

public:
	AI(QWidget *parent = Q_NULLPTR);
	~AI();
	void Manual_Initialize(bool first = false);
	void Launch();

protected:
	void mousePressEvent(QMouseEvent*); // MouseEvent
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;	// Paint Piece
	void AI_hide();

private slots:
	void on_back_button_clicked();	// back
	void on_Regame_clicked();	// regame
	void on_Rechoose_clicked();	// rechoose
	void on_Traceback_clicked();	// traceback

private:
	Ui::AI *ui;
	AI_Settings* AI_settings;
	AI_Function AI_fun;
	Board *board;
	QPixmap *UI;
	bool Start;
	int trace_chance;

	/* Button */
	QAnimationButton* Traceback;
	QAnimationButton* Regame;
	QAnimationButton* Rechoose;
	QIcon icon_Traceback, icon_Regame, icon_Rechoose;

};
