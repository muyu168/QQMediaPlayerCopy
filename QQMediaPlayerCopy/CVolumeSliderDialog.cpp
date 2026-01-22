#include "CVolumeSliderDialog.h"
#include <QVBoxLayout>
#include <QEvent>
#include <windows.h>

CVolumeSliderDialog::CVolumeSliderDialog(QWidget* parent)
	: QDialog(parent)
{
	this->setFixedSize(40, 200);
	QVBoxLayout* pVLay = new QVBoxLayout(this);
	m_pSlider = new QSlider(this);
	m_pSlider->setOrientation(Qt::Vertical);
	pVLay->addWidget(m_pSlider);

	setFixedSize(40, 120);
	setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);   //ToolTip : 鼠标停留在控件上显示提示信息
	setStyleSheet("QDialog{background-color: rgba(54, 54, 54, 0.5);}");  

	connect(m_pSlider, &QSlider::valueChanged, [=](int value) {
		emit sig_SliderValueChanged(value);
		});
}

CVolumeSliderDialog::~CVolumeSliderDialog()
{
}

// 参考qt的文档: bool QWidget::event(QEvent *event)
// 重写popup dialog的event函数，需要去除其double-click事件的写法bool CVolumeSliderDialog::event(QEvent* event)
bool CVolumeSliderDialog::event(QEvent* event)
{
	static bool class_amended = false;

	if (event->type() == QEvent::WinIdChange)
	{
		HWND hwnd = (HWND)winId();

		if (class_amended == false)
		{
			class_amended = true;
			DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
			class_style &= ~CS_DROPSHADOW;
			::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统设置窗口阴影样式
		}
	}

	return QWidget::event(event);
}
