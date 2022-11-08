#ifndef ANIMATIONBUTTON_H
#define ANIMATIONBUTTON_H

#include <QPushButton>
#include <QTimer>

class QAnimationButton : public QPushButton
{
	Q_OBJECT

public:
	explicit QAnimationButton(QWidget *parent = nullptr);
	~QAnimationButton();
	void SizeSet(int type);

protected:
	void enterEvent(QEvent *event);

	void leaveEvent(QEvent *event);

	private slots:
	void slot_timeout();

private:
	QTimer* x_pTimer;
	int x_nCurrentCnt;

	enum AnimationDirect
	{
		UnKnow = 0,
		Big,
		Small
	};

	AnimationDirect x_emAnimation;

	int step_width;
	int step_height;
	int STEPCOUNT;
};

#endif // ANIMATIONBUTTON_H
