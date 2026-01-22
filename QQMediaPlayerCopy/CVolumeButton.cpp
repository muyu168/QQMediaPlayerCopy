#include "CVolumeButton.h"
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <iostream>
#include <QThread>

using namespace std;

CVolumeButton::CVolumeButton(QWidget* parent)
	: QPushButton(parent)
{
	setFixedSize(32, 32);

	setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}"
		"QPushButton:hover{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}");
}

CVolumeButton::~CVolumeButton()
{
}

void CVolumeButton::paintEvent(QPaintEvent*)
{
	QStylePainter p(this);
	QStyleOptionButton option;
	initStyleOption(&option);
	p.drawControl(QStyle::CE_PushButton, option);
}

void CVolumeButton::enterEvent(QEnterEvent* event)
{
	if (!m_pVolumeSliderDlg)
		m_pVolumeSliderDlg = new CVolumeSliderDialog(this);

	QPoint p1 = this->mapToGlobal(QPoint(0, 0));  //获取按钮的全局坐标
	QRect rect1 = this->rect();
	QRect rect2 = m_pVolumeSliderDlg->rect();     //获取音量滑动条的矩形区域

	int x = p1.x() + (rect1.width() - rect2.width()) / 2;
	int y = p1.y() - rect2.height() - 5;
	m_pVolumeSliderDlg->move(x, y);   //移动音量滑动条到指定位置

	m_pVolumeSliderDlg->show();
	startTimer(250);

	connect(m_pVolumeSliderDlg, &CVolumeSliderDialog::sig_SliderValueChanged, [=](int value) {
		emit sig_VolumeValue(value);
		});
}

void CVolumeButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_isMute = !m_isMute;
		if (m_isMute)
		{
			if (m_pVolumeSliderDlg)
				m_pVolumeSliderDlg->setSliderValue(0);

		}
		else
		{
			if (m_pVolumeSliderDlg)
				m_pVolumeSliderDlg->setSliderValue(50);

		}
	}
}


/**
 * @brief 定时器事件处理 leaveEvent，直接在 leaveEvent 中无效化 m_pVolumeSliderDlg 也无效化，移动记录也有无效化，定时器事件会不断地比较现在的鼠标位置
 */

void CVolumeButton::timerEvent(QTimerEvent* event)
{
	if ((m_pVolumeSliderDlg != nullptr) && (m_pVolumeSliderDlg->isVisible()))
	{
		QPoint p1 = QCursor::pos();   // 获取当前鼠标位置
		if (m_pVolumeSliderDlg)
		{
			QRect rect1 = this->rect();  // 获取按钮的矩形区域
			QRect rect2 = m_pVolumeSliderDlg->rect();  // 获取音量滑动条的矩形区域
			QRect rect3 = m_pVolumeSliderDlg->geometry();  // 获取音量滑动条的绝对位置

			QPoint p2 = this->mapToGlobal(QPoint(0, 0));   // 获取按钮在屏幕上的全局坐标

			// 判断鼠标是否在按钮的矩形区域之外，如果在，则隐藏音量滑动条
			QRect area(rect3.left(), rect3.top(), rect2.width(), p2.y() + rect1.height() - rect3.top()); // 定义一个矩形区域，从左上角到按钮的下边界

			if (!area.contains(p1))
			{
				m_pVolumeSliderDlg->hide();
			}
		}
	}
	else
	{
		killTimer(event->timerId());
	}
}
