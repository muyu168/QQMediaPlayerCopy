#include "CPlayButtonGroupWidget.h"
#include <QHBoxLayout>

CPlayButtonGroupWidget::CPlayButtonGroupWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_StyledBackground);  // // 设置背景为样式化

	m_playButtonQss = "QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/play.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/play_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/play.svg);border:none;}";

	m_pauseButtonQss = "QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/pause.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/pause_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/pause.svg);border:none;}";

	m_pStopButton = new QPushButton(this);
	m_pStopButton->setText("");
	m_pStopButton->setFixedSize(32, 32);
	m_pStopButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/stop.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/stop_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/stop.svg);border:none;}");

	m_pPreButton = new QPushButton(this);
	m_pPreButton->setText("");
	m_pPreButton->setFixedSize(32, 32);
	m_pPreButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/pre.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/pre_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/pre.svg);border:none;}");

	m_pPlayButton = new QPushButton(this);
	m_pPlayButton->setText("");
	m_pPlayButton->setFixedSize(48, 48);
	m_pPlayButton->setStyleSheet(m_playButtonQss);

	m_pNextButton = new QPushButton(this);
	m_pNextButton->setText("");
	m_pNextButton->setFixedSize(32, 32);
	m_pNextButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/next.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/next_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/next.svg);border:none;}");

	m_pVolumeButton = new CVolumeButton(this);
	//m_pVolumeButton->setText("");
	//m_pAudioButton->setFixedSize(32, 32);
	/*m_pAudioButton->setStyleSheet("QPushButton{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}"
		"QPushButton:hover{background-color:rgb(99, 99, 99);background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open_hover.svg);border:none;}"
		"QPushButton:pressed{background-image:url(:/playCtrlBar/resources/playctrlBar/audio_open.svg);border:none;}");*/

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pStopButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pPreButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pPlayButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pNextButton);
	pHLay->setSpacing(8);
	pHLay->addWidget(m_pVolumeButton);

	pHLay->setContentsMargins(1, 1, 1, 1);

	setLayout(pHLay);

	// 228  50
	setFixedSize(32 * 4 + 48 + 4 * 8 + 20 , 50);

	connect(m_pPlayButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::onPlayButtonClicked);
	connect(m_pStopButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_stop);
	connect(m_pPreButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_pre);
	connect(m_pNextButton, &QPushButton::clicked, this, &CPlayButtonGroupWidget::sig_next);
	connect(m_pVolumeButton, &CVolumeButton::sig_VolumeValue, this, &CPlayButtonGroupWidget::sig_VolumeChanged);
}

CPlayButtonGroupWidget::~CPlayButtonGroupWidget()
{
}

void CPlayButtonGroupWidget::setPlayingState(bool isPlaying){
	m_isPlaying = isPlaying;
	updatePlayButtonStyle();
}

	//更新按钮样式
void CPlayButtonGroupWidget::updatePlayButtonStyle(){
	if(!m_isPlaying){
		m_pPlayButton->setStyleSheet(m_playButtonQss);
	}else {
		m_pPlayButton->setStyleSheet(m_pauseButtonQss);
	}
}

	//播放/暂停按钮点击处理函数
void CPlayButtonGroupWidget::onPlayButtonClicked(){
	if(m_isPlaying){
		emit sig_pause();
		m_isPlaying = false;
	}else {
		emit sig_play();
		m_isPlaying = true;
	}
	updatePlayButtonStyle();
}