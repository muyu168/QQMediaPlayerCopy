
#pragma once

#include <QtWidgets/QWidget>
#include "CTitleBar.h"
#include "CPlayCtrlBar.h"
#include "VideoWidget.h"
#include <queue>
#include <list>
#include <mutex>
#include <Windows.h>
#include "CPlayListWidget.h"
#include "CVlcKits.h"
#include <memory>

namespace {
    const int kMouseRegionLeft = 7;
    const int kMouseRegionTop = 7;
    const int kMouseRegionButtom = 7;
    const int kMouseRegionRight = 7;  
    const int kMinWindowWidth = 800;   // 最小窗口宽度
    const int kMinWindowHeight = 600;  // 最小窗口高度
}

//�������,�����ڷֳ���9������
enum MousePosition
{
    kMousePositionLeftTop = 11,
    kMousePositionTop = 12,
    kMousePositionRightTop = 13,
    kMousePositionLeft = 21,
    kMousePositionMid = 22,
    kMousePositionRight = 23,
    kMousePositionLeftButtom = 31,
    kMousePositionButtom = 32,
    kMousePositionRightButtom = 33,
};

class CMainWindow : public QWidget
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = Q_NULLPTR);

private:
    void InitUI();

protected:
	void resizeEvent(QResizeEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void sig_playStateChanged(bool isPlaying);
	
protected slots: 
    void on_Play();
    void on_Pause();
    void on_Stop();
    void On_Close();
    void On_ShowFullScreen();
    void On_ShowMiniMode();
	void on_openFile(const QStringList& fileList);
    void on_openUrl(const QString& url);
    void On_openRightPlaylist();
    void On_timer();
    void OnUpdateTimeText(const QString& str);
    void OnUpdateTimeSlider(const int& value);
    void OnSetPlayRate(double rate);
    void OnPlay(const QString& fileName);

private:
    //设置鼠标样式
    void SetMouseCursor(int x, int y);

    //获取鼠标区域
    int GetMouseRegion(int x, int y);

private:
    QPoint           last_point_;             //记录鼠标最后一次点击的位置
    QPoint           last_position_;          //记录鼠标最后一次移动的位置
    bool             left_button_pressed_ = false;   //记录鼠标左键是否按下
    int              mouse_press_region_ = kMousePositionMid; //记录鼠标点击区域
    bool             mouse_moved_ = false;
	CTitleBar*       m_pTitleBar = nullptr;
	CPlayCtrlBar*    m_pPlayCtrlBar = nullptr;
	VideoWidget*     m_pVideoWidget = nullptr;
	CPlayListWidget* m_pPlaylistWidget = nullptr;
	bool             m_bPlaylistIsOpen = false;
	QStringListModel* m_pStringListModel = nullptr;
	std::string        m_curPlayFileName;

	std::unique_ptr<CVlcKits> m_pVlc;
	bool m_isPlay = false;
    bool m_isMiniMode = false;               // 迷你模式标志：true=只显示视频，false=显示完整UI
    QTimer* m_pTimer = nullptr;
    QPoint mouse_last_moved_pos_;
};
