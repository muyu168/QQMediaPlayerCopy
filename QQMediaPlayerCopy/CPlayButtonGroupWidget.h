#pragma once

#include <QWidget>
#include <QPushButton>
#include <QString>
#include "CVolumeButton.h"

class CPlayButtonGroupWidget : public QWidget
{
	Q_OBJECT

public:
	CPlayButtonGroupWidget(QWidget *parent = Q_NULLPTR);
	~CPlayButtonGroupWidget();

signals:
	void sig_play();
	void sig_pause();
	void sig_stop();
	void sig_next();
	void sig_pre();
	void sig_VolumeChanged(int volume);

public slots:
	//更新状态
	void setPlayingState(bool isPlaying);

private slots:
	//播放/暂停按钮点击处理函数
	void onPlayButtonClicked();

private:
	//更新按钮样式
	void updatePlayButtonStyle();

private:
	bool m_isPlaying = false;
	QPushButton* m_pStopButton = nullptr;
	QPushButton* m_pPreButton = nullptr;
	QPushButton* m_pPlayButton = nullptr;
	QPushButton* m_pNextButton = nullptr;
	CVolumeButton* m_pVolumeButton = nullptr;
	QString m_playButtonQss;						//播放图标样式
	QString m_pauseButtonQss;						//暂停图标样式
	
};
