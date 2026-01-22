#include "CommonUtils.h"
#include <QSettings>


CommonUtils::CommonUtils()
{
}

CommonUtils::~CommonUtils()
{
}

void CommonUtils::writeIni(QString inipath, QString keystr, QString value)
{
	QSettings *configIniWrite = new QSettings(inipath, QSettings::IniFormat);
	configIniWrite->setValue(keystr, value);

	// 删除配置文件写入对象
	delete configIniWrite;
}

QString CommonUtils::readIni(QString inipath, QString keystr)
{
	QSettings *configIniRead = new QSettings(inipath, QSettings::IniFormat);
	QString ipResult = configIniRead->value(keystr).toString();

	// 删除配置文件读取对象
	delete configIniRead;

	return ipResult;
}