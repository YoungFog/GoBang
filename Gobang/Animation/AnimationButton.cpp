#include "AnimationButton.h"

QAnimationButton::QAnimationButton(QWidget *parent)
	: QPushButton(parent)
	, x_pTimer(nullptr)
	, x_nCurrentCnt(0)
{
	x_emAnimation = UnKnow;
	x_pTimer = new QTimer();
	x_pTimer->setInterval(10);
	connect(x_pTimer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
	setCursor(QCursor(Qt::PointingHandCursor));
	setStyleSheet("border-image:url(:/Resource/UI_Design/#Block.png);");
	// setting
	step_width = 3;
	step_height = 1;
	STEPCOUNT = 10;
	
}

QAnimationButton::~QAnimationButton()
{
	if (x_pTimer)
	{
		if (x_pTimer->isActive())
		{
			x_pTimer->stop();
		}
		delete x_pTimer;
		x_pTimer = nullptr;
	}
}

void QAnimationButton::enterEvent(QEvent *event)
{
	x_emAnimation = Big;
	if (x_pTimer->isActive())
	{
		x_pTimer->stop();
	}
	x_pTimer->start();
}

void QAnimationButton::leaveEvent(QEvent *event)
{
	x_emAnimation = Small;
	if (x_pTimer->isActive())
	{
		x_pTimer->stop();
	}
	x_pTimer->start();
}

void QAnimationButton::slot_timeout()
{
	QRect _rect = this->geometry();
	QSize _size = this->iconSize();
	// animation
	if (x_emAnimation == Big)
	{
		if (x_nCurrentCnt >=0 && x_nCurrentCnt < STEPCOUNT)
		{
			x_nCurrentCnt++;
			QRect _newRect = QRect(_rect.left() - step_width, _rect.top() - step_height, _rect.width() + 2 * step_width, _rect.height() + 2 * step_height);
			QSize _newSize = QSize(_size.width() + 2 * step_width, _size.height() + 2 * step_height);
			this->setGeometry(_newRect);
			this->setIconSize(_newSize);
		}
		else
		{
			x_pTimer->stop();
		}
		
	}
	else if (x_emAnimation == Small)
	{
		if (x_nCurrentCnt > 0 && x_nCurrentCnt <= STEPCOUNT)
		{
			x_nCurrentCnt--;
			QRect _newRect = QRect(_rect.left() + step_width, _rect.top() + step_height, _rect.width() - 2 * step_width, _rect.height() - 2 * step_height);
			QSize _newSize = QSize(_size.width() - 2 * step_width, _size.height() - 2 * step_height);
			this->setGeometry(_newRect);
			this->setIconSize(_newSize);
		}
		else
		{
			x_pTimer->stop();
		}
	}
}

void QAnimationButton::SizeSet(int type)
{
	if (type == 0)	// 373:96
	{
		step_width = 3;
		step_height = 1;
		STEPCOUNT = 10;
	}
	else if (type == 1)	// 142:57  161:50
	{
		step_width = 3;
		step_height = 1;
		STEPCOUNT = 4;
	}
	else if (type == 2)	// 350:220
	{
		step_width = 3;
		step_height = 2;
		STEPCOUNT = 8;
	}
}