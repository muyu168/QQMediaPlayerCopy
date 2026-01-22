  #include "MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QStringListModel>
#include <QScreen>
#include <Windows.h>
#include <QApplication>
  
using namespace std;

BOOL CALLBACK EnumVLC(HWND hwnd, LPARAM lParam)
{
	TCHAR szTitle[1024];

	int nLen = GetWindowText(hwnd, szTitle, 1024);

	if (nLen > 0)
	{
		EnableWindow(hwnd, FALSE);
		KillTimer(NULL, 1);
	}

	return TRUE;
}

void CALLBACK TimeProc(HWND hwnd, UINT msg, UINT_PTR id, DWORD time)
{
	HWND vlcHwnd = FindWindowExW(NULL, NULL, NULL, L"vlcmain");

	if (vlcHwnd)
	{
		EnumChildWindows(vlcHwnd, EnumVLC, NULL);
	}
}
  
string qstr2str(const QString& qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

CMainWindow::CMainWindow(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowTitle("vlcmain");
	setMouseTracking(true);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setAttribute(Qt::WA_Hover);

	// 设置最小窗口尺寸，Qt 会自动处理
	setMinimumSize(kMinWindowWidth, kMinWindowHeight);
  
	m_pVlc.reset(new CVlcKits());

	int nRet = m_pVlc->initVLC();
	  
	switch (nRet)
	{
	case -1:
		QMessageBox::information(this, tr("Warn"),
			tr("libvlc_new failed"));

		exit(EXIT_FAILURE);

	case -2:
		QMessageBox::information(this, tr("Warn"),
			tr("libvlc_media_player_new failed"));

		exit(EXIT_FAILURE);
	}

	connect(m_pVlc.get(), &CVlcKits::sig_UpdateTimeText, this, &CMainWindow::OnUpdateTimeText);
	connect(m_pVlc.get(), &CVlcKits::sig_TimeSliderPos, this, &CMainWindow::OnUpdateTimeSlider);

	InitUI();
}

void CMainWindow::InitUI()
{
	//初始化垂直布局
	QVBoxLayout* pVLay = new QVBoxLayout(this);
	pVLay->setSpacing(0);

	//初始化标题栏控件
	m_pTitleBar = new CTitleBar(this);
	m_pVideoWidget = new VideoWidget(this);
	m_pPlayCtrlBar = new CPlayCtrlBar(this);
	m_pPlaylistWidget = new CPlayListWidget(this);

	//添加控件到垂直布局
	pVLay->addWidget(m_pTitleBar);

	//初始化播放栏水平布局
	QHBoxLayout* pHLay = new QHBoxLayout;
	pHLay->addWidget(m_pVideoWidget);
	pHLay->addWidget(m_pPlaylistWidget);
	pVLay->addLayout(pHLay);

	pVLay->addWidget(m_pPlayCtrlBar);
	pVLay->setContentsMargins(0, 0, 0, 0);

	m_pPlaylistWidget->hide();  //默认隐藏右侧播放列表
	this->resize(kMinWindowWidth, kMinWindowHeight);

	//连接控件信号槽
	connect(m_pTitleBar, &CTitleBar::sig_close, this, &CMainWindow::On_Close);
	connect(m_pTitleBar, &CTitleBar::sig_ShowFullFcreen, this, &CMainWindow::On_ShowFullScreen);
	connect(m_pTitleBar, &CTitleBar::sig_showMiniMode, this, &CMainWindow::On_ShowMiniMode);
	connect(m_pTitleBar, &CTitleBar::sig_openfile, this, &CMainWindow::on_openFile);
	connect(m_pTitleBar, &CTitleBar::sig_openUrl, this, &CMainWindow::on_openUrl);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenFile, this, &CMainWindow::on_openFile);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenUrl, this, &CMainWindow::on_openUrl);
	connect(m_pVideoWidget, &VideoWidget::sig_OpenPlaylist, this, &CMainWindow::On_openRightPlaylist);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_fullScreen, this, &CMainWindow::On_ShowFullScreen);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_playRate, this, &CMainWindow::OnSetPlayRate);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_play, this, &CMainWindow::on_Play);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_pause, this, &CMainWindow::on_Pause);  
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_stop, this, &CMainWindow::on_Stop);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_SliderMove,m_pVlc.get(), &CVlcKits::setVideoPostion);
	connect(m_pPlaylistWidget, &CPlayListWidget::sig_doubleClickFileName,
		this,
		&CMainWindow::OnPlay);
	connect(m_pPlayCtrlBar, &CPlayCtrlBar::sig_VolumeChanged, m_pVlc.get(), &CVlcKits::setVolume);
}

void CMainWindow::resizeEvent(QResizeEvent* event)
{
	if (!this->isMaximized() && !this->isFullScreen())
	{
		QScreen* pScreen = qApp->primaryScreen();
		QRect appRect = pScreen->availableGeometry();
		int sw = appRect.width();
		int sh = appRect.height();

		int thisW = this->width();
		int thisH = this->height();
	}
}

void CMainWindow::On_Close()
{
	if (m_isPlay)
	{
		QMessageBox::StandardButton rb = QMessageBox::warning(this, tr("warn"),
			tr("Do you want to end player?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		if (rb == QMessageBox::Yes)
		{

			close();
		}
	}
	else
	{
		close();
	}
}

void CMainWindow::on_openFile(const QStringList& fileList)
{
	if (fileList.isEmpty())
		return;

	// 初始化播放列表模型
	m_pStringListModel = new QStringListModel(this);

	// 设置播放列表模型数据
	m_pStringListModel->setStringList(fileList);

	// 设置播放列表控件数据
	m_pPlaylistWidget->setPlayList(m_pStringListModel);

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}

	//定时监控VLC窗口
	SetTimer(NULL, 1, 300, TimeProc);
	m_pVideoWidget->setPlayStatus(true);
	m_pVideoWidget->showTopWidgetOpenfilesButton(false);

	//获取文件名
	QString fileName = fileList[0];
	int index1 = fileName.lastIndexOf('/');
	fileName = fileName.mid(index1 + 1);

	//设置标题栏文件名标签
	m_pTitleBar->setFileNameLabelText(fileName);

	m_isPlay = true;
	m_pPlayCtrlBar->setPlayingState(m_isPlay); 

	m_pPlayCtrlBar->showTimeLabel(true);
	m_pPlayCtrlBar->showSlider(true);

	m_pTimer = new QTimer(this);
	m_pTimer->start(3000);
	connect(m_pTimer, &QTimer::timeout, this, &CMainWindow::On_timer);


}


void CMainWindow::on_openUrl(const QString& url)
{
	m_pVlc->play(url, (void*)(m_pVideoWidget->winId()));

// 提示用户
    QMessageBox::information(this, tr("提示"), 
        tr("正在连接网络流，请稍候...\n"
           "如果长时间无响应，请检查：\n"
           "1. URL 是否正确\n"
           "2. 网络连接是否正常\n"
           "3. 防火墙设置"));
    

	m_isPlay = true;
	m_pPlayCtrlBar->setPlayingState(m_isPlay); 

	m_pPlayCtrlBar->showTimeLabel(true);
	m_pPlayCtrlBar->showSlider(true);
	  

	m_pTimer = new QTimer(this);
	m_pTimer->start(3000);
	connect(m_pTimer, &QTimer::timeout, this, &CMainWindow::On_timer);
}

void CMainWindow::On_openRightPlaylist()
{
	const int playlistwidth = 400;		//播放列表宽度
	if (m_pPlaylistWidget->isHidden())
	{
		//显示播放列表
		m_pPlaylistWidget->show();

		//扩大窗口宽度
		int newplaylistwidth = this->width() + playlistwidth;
		this->resize(newplaylistwidth , this->height());
	}
	else
	{
		//隐藏播放列表
		m_pPlaylistWidget->hide();

		//缩小窗口宽度
		int newplaylistwidth = this->width() - playlistwidth;
		this->resize(newplaylistwidth , this->height());
	}
}

void CMainWindow::On_timer()
{
	if (m_isPlay)
	{
		if (mouse_last_moved_pos_ == QCursor::pos())
		{
			QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
			QRect rect = m_pVideoWidget->rect();

			QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
			QPoint p2 = QCursor::pos();

			if (m_isPlay)
			{
				if (rect2.contains(p2))
				{
					m_pVideoWidget->showTopWidget(false);
					this->setCursor(Qt::BlankCursor);   //设置光标不可见
					m_pTimer->stop();
				}
				else
				{
					this->setCursor(Qt::ArrowCursor);  //设置光标可见
				}
			}
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);  //设置光标可见
		}
	}
}

void CMainWindow::OnUpdateTimeText(const QString& str)
{
	m_pPlayCtrlBar->setCurPlayTime(str);
}

void CMainWindow::OnUpdateTimeSlider(const int& value)
{
	m_pPlayCtrlBar->setSliderValue(value);
}

// 更改视频播放速率
void CMainWindow::OnSetPlayRate(double rate)
{
	m_pVlc->setPlayRate(rate);
}

void CMainWindow::OnPlay(const QString& fileName)
{
	QStringList fileList;
	fileList << fileName;

	if (0 != m_pVlc->play(fileList, (void*)(m_pVideoWidget->winId())))
	{
		QMessageBox::information(this, tr("Warn"), tr("can't play this video file"));
		return;
	}
}
void CMainWindow::on_Play(){
	if(m_pVlc){
		m_pVlc->play();
	}
}

void CMainWindow::on_Pause(){
	if(m_pVlc){
		m_pVlc->pause();
	}
}

void CMainWindow::on_Stop(){
	if(m_pVlc){
		m_isPlay = false;
		m_pPlayCtrlBar->setPlayingState(m_isPlay);

		m_pVlc->stop();
		m_pPlayCtrlBar->showTimeLabel(false);
		m_pPlayCtrlBar->showSlider(false);
	}
}

void CMainWindow::On_ShowFullScreen()
{
	this->showFullScreen();
	m_pTitleBar->hide();
	m_pPlayCtrlBar->hide();
	m_pPlaylistWidget->hide();
}

void CMainWindow::On_ShowMiniMode()
{
	if (m_isPlay)
	{
		m_isMiniMode = true;
		m_pTitleBar->hide();
		m_pPlayCtrlBar->hide();
		m_pPlaylistWidget->hide();
		this->update();
	}
} 

void CMainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
	QRect rect = m_pVideoWidget->rect();

	QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
	QPoint p2 = QCursor::pos();

	if (rect2.contains(p2))
	{
		m_isMiniMode = false;  // 更新迷你模式标志

		//判断是否处于全屏模式
		if (this->isFullScreen())
		{
			//是则退出全屏模式，并且显示标题栏、播放栏
			this->showNormal();
			m_pTitleBar->show();
			m_pPlayCtrlBar->show();
		}
		else
		{
			//否则进入全屏模式，并且隐藏标题栏、播放栏
			this->showFullScreen();
			m_pTitleBar->hide();
			m_pPlayCtrlBar->hide();
			m_pPlaylistWidget->hide();
		}
	}
}

void CMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		//退出全屏模式
		if (this->isFullScreen())
		{
			this->showNormal();
		} 
		// 退出迷你模式
		if (m_isMiniMode || this->isFullScreen())
		{
			m_isMiniMode = false;
			m_pTitleBar->show();
			m_pPlayCtrlBar->show();
		}
	}
}

void CMainWindow::mouseMoveEvent(QMouseEvent* event)
{
	mouse_moved_ = true;
	mouse_last_moved_pos_ = QCursor::pos();

	if (m_pTimer && !m_pTimer->isActive())
		m_pTimer->start(3000);

	QPoint p1 = m_pVideoWidget->mapToGlobal(QPoint(0, 0));
	QRect rect = m_pVideoWidget->rect();

	QRect rect2(p1.x(), p1.y(), rect.width(), rect.height());
	QPoint p2 = QCursor::pos();

	if (m_isPlay)
	{
		if (rect2.contains(p2))
		{
			m_pVideoWidget->showTopWidget(true);
		}
		else
		{
			m_pVideoWidget->showTopWidget(false);
		}
	}

	/*********************************窗口拖拽功能*********************************/
	//判断是否处于全屏模式，是则不允许拖拽窗口，直接返回
	if (isMaximized())
	{
		return;
	}
	//设置鼠标样式
	SetMouseCursor(event->pos().x(), event->pos().y());

	if ((event->buttons() == Qt::LeftButton) && left_button_pressed_)
	{
		QPoint point_offset = event->globalPos() - last_position_;  
		if (mouse_press_region_ == kMousePositionMid)
		{
			if (m_isMiniMode)
			{
				// 迷你模式下光标可见，直接拖拽窗口
				setCursor(Qt::ArrowCursor);
				move(point_offset + last_point_);
			}
		}
		else
		{
			// 根据鼠标位置初始化新窗口大小
			QRect rect = geometry();

			switch (mouse_press_region_)
			{
			case kMousePositionLeftTop:
				rect.setTopLeft(rect.topLeft() + point_offset);
				break;
			case kMousePositionTop:
				rect.setTop(rect.top() + point_offset.y());
				break;
			case kMousePositionRightTop:
				rect.setTopRight(rect.topRight() + point_offset);
				break;
			case kMousePositionRight:
				rect.setRight(rect.right() + point_offset.x());
				break;
			case kMousePositionRightButtom:
				rect.setBottomRight(rect.bottomRight() + point_offset);
				break;
			case kMousePositionButtom:
				rect.setBottom(rect.bottom() + point_offset.y());
				break;
			case kMousePositionLeftButtom:
				rect.setBottomLeft(rect.bottomLeft() + point_offset);
				break;
			case kMousePositionLeft:
				rect.setLeft(rect.left() + point_offset.x());
				break;
			default:
				break;
			}

			// Qt支持的setMinimumSize 会自动限制最小尺寸?
			// 但我们需要确保不会因为限制而导致窗口移动?
			// 所以在设置前检查并调整
			if (rect.width() < minimumWidth())
			{
				if (mouse_press_region_ == kMousePositionLeft ||
					mouse_press_region_ == kMousePositionLeftTop ||
					mouse_press_region_ == kMousePositionLeftButtom)
				{
					rect.setLeft(rect.right() - minimumWidth());
				}
				else
				{
					rect.setRight(rect.left() + minimumWidth());
				}
			}

			if (rect.height() < minimumHeight())
			{
				if (mouse_press_region_ == kMousePositionTop ||
					mouse_press_region_ == kMousePositionLeftTop ||
					mouse_press_region_ == kMousePositionRightTop)
				{
					rect.setTop(rect.bottom() - minimumHeight());
				}
				else
				{
					rect.setBottom(rect.top() + minimumHeight());
				}
			}

			//更新窗口大小
			setGeometry(rect);
			last_position_ = event->globalPos();
		}
	}
	/******************************************************************/
}

void CMainWindow::mouseReleaseEvent(QMouseEvent* event)
{
	left_button_pressed_ = false;
}

void CMainWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		left_button_pressed_ = true;
		last_point_ = pos();
		last_position_ = event->globalPos();
		mouse_press_region_ = GetMouseRegion(event->pos().x(), event->pos().y());
	}
}

void CMainWindow::SetMouseCursor(int x, int y)
{
	Qt::CursorShape cursor;
	int region = GetMouseRegion(x, y);
	switch (region)
	{
	case kMousePositionLeftTop:
	case kMousePositionRightButtom:
		cursor = Qt::SizeFDiagCursor; break;
	case kMousePositionRightTop:
	case kMousePositionLeftButtom:
		cursor = Qt::SizeBDiagCursor; break;
	case kMousePositionLeft:
	case kMousePositionRight:
		cursor = Qt::SizeHorCursor; break;
	case kMousePositionTop:
	case kMousePositionButtom:
		cursor = Qt::SizeVerCursor; break;
	case kMousePositionMid:
		cursor = Qt::ArrowCursor; break;
	default:
		break;
	}
	setCursor(cursor);
}

int CMainWindow::GetMouseRegion(int x, int y)
{
	int region_x = 0, region_y = 0;

	// X方向判断（左、中、右）
	if (x < kMouseRegionLeft)
	{
		region_x = 1;
	}
	else if (x > (width() - kMouseRegionRight))
	{
		region_x = 3;
	}
	else
	{
		region_x = 2;
	}

	// Y方向判断（上、中、下）
	if (y < kMouseRegionTop)
	{
		region_y = 1;
	}
	else if (y > (height() - kMouseRegionButtom))
	{
		region_y = 3;
	}
	else
	{
		region_y = 2;
	}

	return region_y * 10 + region_x;
}