#pragma once

#include "Qt.h"
#include "library.h"
#include "stl.h"
#include "../../../SharedCode/LokiBuffer.h"

class FileUtil
{
public:
	static QString GetTempFolder();
	static QString GetKeCacheFolder();
	static QString GetDocumentFolder();
	static QString GetFilename(QString filefullpath, QString defaultName = "");
	static bool ZipFolder(QString folder, QString zipFilePath);
	static bool UnZipFolder(QString folder, QString zipFilePath);
	static LokiBuffer ZipLokiBuffer(LokiBuffer& buffer);
	static LokiBuffer UnZipLokiBuffer(LokiBuffer& buffer);
	static LokiBuffer ZipString(QString& string);
	static QString UnZipString(LokiBuffer& buffer);
	static bool RemoveDir(const QString &dirName);
	static bool IsValidExtensionFile(const QString& strImageFileName, const QString& extension);
	static bool IsDirDotTwo(const QString& dir);
	static QString CopyableFilename(QString filepath);
	static QString RandomFilename();
	static bool CopyFolder(const QString& src, const QString& dest);
	static bool CopyFile(const QString& src, const QString& dest);
	static bool GetFilePart(const QString&src, const QString& dest, int postion);
};

