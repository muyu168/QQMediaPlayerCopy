#include "CTimeSlider.h"
#include <QEvent>
#include <QMouseEvent>

CTimeSlider::CTimeSlider(QWidget* p)
	:QSlider(p)
{
	setAttribute(Qt::WA_StyledBackground);

	this->setOrientation(Qt::Horizontal);
	this->setFixedHeight(m_SliderHeight);
	this->setRange(0, 100);

	// 将当前打开的路径写入注册表
	this->installEventFilter(this);

	QString qss = "QSlider {"
    "    background-color: transparent;"  // 改为透明，不遮挡背景
    "    height: 30px;"                   // 增加可点击区域
    "}"
    "QSlider::groove:horizontal {"
    "    background: rgba(255, 255, 255, 0.2);"  // 半透明白色轨道
    "    height: 4px;"                            // 轨道高度
    "    border-radius: 2px;"
    "}"
    "QSlider::sub-page:horizontal {"
    "    background-color: rgb(50,150,250);"  // 已播放：蓝色
    "    height: 4px;"
    "    border-radius: 2px;"
    "}"
    "QSlider::add-page:horizontal {"
    "    background-color: rgba(255, 255, 255, 0.3);"  // 未播放：半透明白色
    "    height: 4px;"
    "    border-radius: 2px;"
    "}"
    "QSlider::handle:horizontal {"
    "    background: white;"           // 白色滑块
    "    width: 12px;"
    "    height: 12px;"
    "    margin: -4px 0px;"            // 垂直居中
    "    border-radius: 6px;"          // 圆形
    "    border: 2px solid rgba(78, 165, 252, 1);"   // 蓝色边框
    "}"
    "QSlider::handle:horizontal:hover {"
    "    background: rgba(78, 165, 252, 1);"         // 悬停时变色
    "    border: 2px solid white;"
    "}";

	this->setStyleSheet(qss);

	connect(this, &QSlider::sliderMoved, this, &CTimeSlider::onSliderMoved);
}

CTimeSlider::~CTimeSlider()
{
}

bool CTimeSlider::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress && watched == this)
	{
		slider_mouseLButtonPress(watched, event);
	}

	return QWidget::eventFilter(watched, event);
}

void CTimeSlider::slider_mouseLButtonPress(QObject* slider, QEvent* event)
{
	do
	{
		QSlider* sliderCtrl = static_cast<QSlider*>(slider);
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (Qt::LeftButton != mouseEvent->button())
		{
			break;
		}

		int cxctl = 0;  
		int cxwnd = 0;  
		int mxpos = 0;  

		if (Qt::Horizontal == sliderCtrl->orientation())
		{
			// 水平模式下的滑块
			cxctl = sliderCtrl->minimumSizeHint().width();
			cxwnd = sliderCtrl->width();
			if (sliderCtrl->invertedAppearance())
				mxpos = cxwnd - mouseEvent->x();
			else
				mxpos = mouseEvent->x();
		}
		else
		{
			// 垂直模式下的滑块
			cxctl = sliderCtrl->minimumSizeHint().height();
			cxwnd = sliderCtrl->height();
			if (sliderCtrl->invertedAppearance())
				mxpos = mouseEvent->y();
			else
				mxpos = cxwnd - mouseEvent->y();
		}

		if (cxwnd <= cxctl)
		{
			break;
		}

		// 计算鼠标点击时的 position 值
		int scpos = sliderCtrl->minimum() +
			(int)((sliderCtrl->maximum() - sliderCtrl->minimum()) *
				((mxpos - cxctl / 2.0) / (cxwnd - cxctl)));

		if (sliderCtrl->sliderPosition() == scpos)
		{
			break;
		}

		sliderCtrl->setSliderPosition(scpos);
		emit sig_SliderMove(scpos); 
	} while (0);
}

void CTimeSlider::onSliderMoved(int position)
{
	emit sig_SliderMove(position);
}

void CTimeSlider::setTimeSliderRange(qint64 value)
{
	this->setRange(0, value);
}

void CTimeSlider::setSliderValue(qint64 value)
{
	this->setValue(value);
}