#pragma once

#include "../../Qt.h"
#include "../../library.h"

class WindowsFileAssociation : QObject
{
public:
	WindowsFileAssociation();
	~WindowsFileAssociation();

	void RegisterFileType(const QString& documentId, const QString& fileTypeName, const QString& fileExtension, const QString& appName="", qint32 appIconIndex = 0);
	bool RebuildIcons();

private:
	bool SetHkcrUserRegKey(QString key, const QString& value, const QString& valueName = QString::null);
	void RegisterCommand(const QString& command, const QString& documentId, const QString cmdLineArg, const QString appName);
};