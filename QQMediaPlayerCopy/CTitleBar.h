/*

标题栏

*/


#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CTitleBar : public QWidget
{
	Q_OBJECT

public:
	CTitleBar(QWidget *parent = Q_NULLPTR);
	~CTitleBar();

	void setFileNameLabelText(QString name);

private:
	void InitData();
	void RestoreUI();

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

signals:
	void sig_close();
	void sig_ShowFullFcreen();
	void sig_showMiniMode();  // 显示小窗口显示
	void sig_openfile(const QStringList& fileList);
	void sig_openUrl(const QString& url);

private slots:
	void onClicked();
	void openFile();
	void openFloder();
	void openNetStream();
	void about();
	void exit();

private:
	QPushButton* m_pLogoBtn;
	QLabel* m_pFileNameLabel;
	QPushButton* m_pMinimodeBtn;
	QPushButton* m_pSettopBtn;  // 置顶
	QPushButton* m_pMinBtn;
	QPushButton* m_pMaxBtn;
	QPushButton* m_pCloseBtn;
};
