#include "COpenFileButton.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMenu>
#include <QEvent>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>
#include "CNetStreamDlg.h"

#define LEFT_BUTTON_WIDTH 145
#define RIGHT_BUTTON_WIDTH 60
#define BUTTON_HEIGHT 40

COpenFileButton::COpenFileButton(QWidget* p) : QWidget(p)
{
	// 设置总宽度

	m_totalWidth = LEFT_BUTTON_WIDTH + RIGHT_BUTTON_WIDTH;

	m_pOpenFileButton = new QPushButton(this);
	m_pOpenFileButton->setText(tr("open file"));
	m_pOpenFileButton->setFixedSize(LEFT_BUTTON_WIDTH, BUTTON_HEIGHT);

	QString qssLeft = "QPushButton{"
		"padding-left:30px;"
		"border-top-left-radius:20px;border-bottom-left-radius:20px;"
		"background-color: rgb(55, 55, 55);"
		"color:rgb(255, 255, 255);"
		"text-align:left;"
		"}";

	m_pOpenFileButton->setIcon(QIcon(":/videoWidget/resources/videoWidget/openfile.svg"));
	m_pOpenFileButton->setLayoutDirection(Qt::LeftToRight);
	m_pOpenFileButton->setStyleSheet(qssLeft);

	m_pArrowButton = new QPushButton(this);
	m_pArrowButton->setFixedSize(RIGHT_BUTTON_WIDTH, BUTTON_HEIGHT);

	/*QString button_qss = "QPushButton::menu-indicator:open{"
		"image:url(:/titleBar/resources/titleBar/down_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/titleBar/resources/titleBar/up_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}";*/

	QString qssRight = "QPushButton{"
		"background-image: url(:/videoWidget/resources/videoWidget/vline.svg);"
		"background-repeat: no-repeat;"
		"background-origin: padding;"
		"background-position: left;"
		"border-top-right-radius:20px;border-bottom-right-radius:20px;"
		"background-color: rgb(55, 55, 55);}"
		"QPushButton::menu-indicator:open{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"  
		"subcontrol-origin:padding;border:none;}";

	m_pArrowButton->setStyleSheet(qssRight);

	//QString menu_qss = "QMenu{background-color:rgb(253,253,253);}"
	//	"QMenu::item{"
	//	"font:16px;"
	//	"background-color:rgb(253,253,253);"
	//	"padding:8px 32px;"
	//	"margin:0px 8px;"
	//	"border-bottom:1px solid #DBDBDB;}"
	//	/*ѡ��������*/
	//	"QMenu::item:selected{background-color: #FFF8DC;}";

	std::string menu_qss = R"(
		QMenu
		{
			background-color:rgb(73, 73, 73);
		}
 
		QMenu::item
		{
			 font:16px;
			 color:white;
			 background-color:rgb(73, 73, 73);
			 margin:8px 8px;
		}
 
		/*ѡ��������*/
		QMenu::item:selected
		{
			 background-color:rgb(54, 54, 54);
		}
	)";

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(QString::fromStdString(menu_qss));
	pMenu->setFixedWidth(m_totalWidth);
	QAction* pAc1 = new QAction(tr("open file"), this);
	QAction* pAc2 = new QAction(tr("open floder"), this);
	QAction* pAc3 = new QAction(tr("open net stream"), this);
	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	pMenu->addAction(pAc3);
	m_pArrowButton->setMenu(pMenu);
	pMenu->installEventFilter(this);  // 安装事件过滤器，以便调整菜单的位置

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pOpenFileButton);
	pHLay->setSpacing(3);

	pHLay->addWidget(m_pArrowButton);
	pHLay->setContentsMargins(0, 0, 0, 0);

	setLayout(pHLay);

	setFixedSize(m_totalWidth, BUTTON_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);  //设置窗口背景透明

	connect(m_pOpenFileButton, &QPushButton::clicked, this, &COpenFileButton::on_openfile);
	connect(pAc1, &QAction::triggered, this, &COpenFileButton::on_openfile);
	connect(pAc2, &QAction::triggered, this, &COpenFileButton::on_openFloder);
	connect(pAc3, &QAction::triggered, this, &COpenFileButton::on_openNetStream);
}

COpenFileButton::~COpenFileButton()
{
}

void COpenFileButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // �����
	painter.setBrush(QBrush(QColor(55, 55, 55)));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	painter.drawRoundedRect(rect, BUTTON_HEIGHT / 2, BUTTON_HEIGHT / 2);  // 绘制圆角矩形 15px

	QWidget::paintEvent(event);
}

bool COpenFileButton::eventFilter(QObject* watched, QEvent* event)
{
	if(m_pArrowButton)
	{
		if (event->type() == QEvent::Show && watched == m_pArrowButton->menu())
		{
			int menuXPos = m_pArrowButton->menu()->pos().x() - LEFT_BUTTON_WIDTH;   // 默认显示在当前按钮左侧
			int menuWidth = m_pArrowButton->menu()->size().width();
			int buttonWidth = m_pArrowButton->size().width();
			QPoint pos = QPoint(menuXPos, m_pArrowButton->menu()->pos().y() + 2);

			m_pArrowButton->menu()->move(pos);
			return true;
		}
	}

	return false;
}

void COpenFileButton::on_openfile()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\CMainWindow";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // 获取文件夹路径
	
	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // 获取默认文件夹路径
	}

	//同时打开多个文件
	QStringList filelist = QFileDialog::getOpenFileNames(
		this, 
		tr("select video file"),
		lastPath,
		tr("video file (*.flv *.rmvb *.avi *.mp4);; all files(*.*);; "));

	if (filelist.isEmpty())
	{
		return;
	}

	int end = filelist[0].lastIndexOf("/");
	QString tmppath = filelist[0].left(end + 1);
	settings.setValue("openfile_path", tmppath);  // 将当前打开的文件夹路径保存到注册表中

	emit sig_openfile(filelist);
}

void COpenFileButton::on_openFloder()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\CMainWindow";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // 获取文件夹路径

	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // 获取默认文件夹路径 
	}

	QString path = QFileDialog::getExistingDirectory(this, 
		tr("choose video directory"), lastPath);

	if (path.isEmpty())
	{
		return;
	}

	QDir dir(path);

	QStringList nameFilters;
	nameFilters << "*.mp4" << "*.flv" << "*.rmvb" << "*.avi";

	// 筛选出文件夹中的视频文件
	QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	
	int _size = files.size();
	if (_size == 0)
	{
		QMessageBox::information(this, tr("Warn"), tr("this floder no video files"));
		return;
	}

	QStringList fileList;
	for (int i = 0; i < _size; i++)
	{
		QString fileName = path + "/" + files[i];
		fileList << fileName;
	}

	settings.setValue("openfile_path", path);  // 将当前打开的文件夹路径保存到注册表中
	
	emit sig_openfile(fileList);
}

void COpenFileButton::on_openNetStream()
{
	CNetStreamDlg dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		QString strText = dlg.getUrl();
		emit sig_openUrl(strText);
	}
}
