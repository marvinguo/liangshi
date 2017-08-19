#include "FileUtil.h"
#include "Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

/*
bool FileUtil::ZipFolder(QString folder, QString zipFilePath)
{
	QDir dir(folder);
	bool exist = dir.exists();
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QuaZip zip(zipFilePath);
	if (!zip.open(QuaZip::mdCreate))
	{
		return false;
	}

	QFileInfoList list = dir.entryInfoList();

	int i = 0;
	QDateTime dt1;
	foreach (QFileInfo file, list)
	{
		QString fileFullPath = file.absoluteFilePath();
		QString fileName = file.fileName();
		QuaZipFile zipFile(&zip);
		QString filePath = QDir(dir).filePath(fileFullPath);
		QFileInfo fileInfo(filePath);

		QuaZipNewInfo newInfo(fileName, filePath);
		if (i == 0) // to test code that needs different timestamps
			newInfo.dateTime = newInfo.dateTime.addSecs(-60);
		else if (i == 1) // will use for the next file too
			dt1 = newInfo.dateTime;
		else if (i == 2) // to test identical timestamps
			newInfo.dateTime = dt1;
		if (!zipFile.open(QIODevice::WriteOnly, 
			newInfo, NULL, 0, 
			fileInfo.isDir() ? 0 : 8)) 
		{
			return false;
		}
		if (!fileInfo.isDir())
		{
			QFile file(filePath);
			if (!file.open(QIODevice::ReadOnly))
			{
				return false;
			}
			while (!file.atEnd())
			{
				char buf[4096];
				qint64 l = file.read(buf, 4096);
				if (l <= 0) 
				{
					return false;
				}
				if (zipFile.write(buf, l) != l)
				{
					return false;
				}
			}
			file.close();
		}
		zipFile.close();
		++i;
	}
	zip.setComment(QString("courseware file version 1"));
	zip.close();
	return QFileInfo(zipFilePath).exists();
}
*/

QString FileUtil::GetTempFolder()
{
	QString path;
#if defined(Q_OS_WIN32)
	path = qApp->applicationDirPath() + QObject::tr("/Browser/Temp/");
#elif defined(Q_OS_ANDROID)
	path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/";
#else
	path = QDir::tempPath();
#endif
	//path = QDir::toNativeSeparators(path);
	QDir dir(path);
	dir.mkpath(".");
	return path;
}

QString FileUtil::GetKeCacheFolder()
{
	QString path;
#if defined(Q_OS_WIN32)
	path = qApp->applicationDirPath() + QObject::tr("/Browser/KeCache/");
#elif defined(Q_OS_ANDROID)
	//先检测是否有sd,然后用本机的位置
	path = Util::GetAndroidKePath();
	qDebug() << "Util::GetAndroidKePath:" << path;
#else
	path = QDir::tempPath();
#endif
	//path = QDir::toNativeSeparators(path);
	QDir dir(path);
	dir.mkpath(".");
	return path;
}

QString FileUtil::GetDocumentFolder()
{
	QString path;
#ifdef Q_OS_IOS
	path = QDir::homePath() + "/Documents";
#else
	path = QDir::homePath();
#endif
	//path = QDir::toNativeSeparators(path);
	QDir dir(path);
	dir.mkpath(".");
	return path;
}

QString FileUtil::	GetFilename(QString filefullpath, QString defaultName)
{
	QFileInfo info(filefullpath);

	if(info.exists())
	{
		return info.completeBaseName();
	}
	return defaultName;
}

bool FileUtil::ZipFolder(QString folder, QString zipFilePath)
{
	return JlCompress::compressDir(zipFilePath, folder);
}

bool FileUtil::UnZipFolder(QString folder, QString zipFilePath)
{
	JlCompress::extractDir(zipFilePath, folder);
	return true;
}

LokiBuffer FileUtil::ZipLokiBuffer(LokiBuffer& buffer)
{
	QString input = QUuid::createUuid().toString();
	QString inputPath = QDir::tempPath() + "/" + input;

	QFile file(inputPath);
	file.open(QIODevice::WriteOnly);
	file.write(buffer.Data(), buffer.Size());
	file.flush();
	file.close();

	QString output = QUuid::createUuid().toString();
	QString outputPath = QDir::tempPath() + "/" + output;
	JlCompress::compressFile(outputPath, inputPath);

	QFile outputFile(outputPath);
	outputFile.open(QIODevice::ReadOnly);
	QByteArray array = outputFile.readAll();
	outputFile.close();

	QFile::remove(inputPath);
	QFile::remove(outputPath);
	return LokiBuffer(array.data(), array.size());
}

LokiBuffer FileUtil::UnZipLokiBuffer(LokiBuffer& buffer)
{
	QString input = QUuid::createUuid().toString();
	QString inputPath = QDir::tempPath() + "/" + input;

	QFile inputFile(inputPath);
	inputFile.open(QIODevice::WriteOnly);
	inputFile.write(buffer.Data(), buffer.Size());
	inputFile.flush();
	inputFile.close();

	QString output = QUuid::createUuid().toString();
	QString outputPath = QDir::tempPath() + "/" + output;
	//JlCompress::extractFile(inputPath, outputPath);
	UnZipFolder(outputPath, inputPath);

	QByteArray allText;
	//get the first file in outputPath
	QDir dir(outputPath);
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks);
	QFileInfoList files = dir.entryInfoList();
	for(QFileInfoList::iterator i = files.begin(); i != files.end(); ++i)
	{
		QFileInfo file = *i;
		if(file.isFile())
		{
			QFile outputFile(file.absoluteFilePath());
			outputFile.open(QIODevice::ReadOnly);

			allText = outputFile.readAll();
			outputFile.close();
			break;
		}
	}

	QFile::remove(inputPath);
	RemoveDir(outputPath);
	return LokiBuffer(allText.data(), allText.size());
}

LokiBuffer FileUtil::ZipString(QString& theQString)
{
	QString input = QUuid::createUuid().toString();
	QString inputPath = QDir::tempPath() + "/" + input;

	QFile file(inputPath);
	file.open(QIODevice::WriteOnly);
	string stdString(theQString.toStdString());
	file.write(stdString.c_str(), stdString.size());
	file.flush();
	file.close();

	QString output = QUuid::createUuid().toString();
	QString outputPath = QDir::tempPath() + "/" + output;
	JlCompress::compressFile(outputPath, inputPath);

	QFile outputFile(outputPath);
	outputFile.open(QIODevice::ReadOnly);
	QByteArray array = outputFile.readAll();
	outputFile.close();

	QFile::remove(inputPath);
	QFile::remove(outputPath);
	return LokiBuffer(array.data(), array.size());
}

QString FileUtil::UnZipString(LokiBuffer& buffer)
{
	QString input = QUuid::createUuid().toString();
	QString inputPath = QDir::tempPath() + "/" + input;

	QFile inputFile(inputPath);
	inputFile.open(QIODevice::WriteOnly);
	inputFile.write(buffer.Data(), buffer.Size());
	inputFile.flush();
	inputFile.close();

	QString output = QUuid::createUuid().toString();
	QString outputPath = QDir::tempPath() + "/" + output;
	//JlCompress::extractFile(inputPath, outputPath);
	UnZipFolder(outputPath, inputPath);

	QString allText;
	//get the first file in outputPath
	QDir dir(outputPath);
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks);
	QFileInfoList files = dir.entryInfoList();
	for(QFileInfoList::iterator i = files.begin(); i != files.end(); ++i)
	{
		QFileInfo file = *i;
		if(file.isFile())
		{
			QFile outputFile(file.absoluteFilePath());
			outputFile.open(QIODevice::ReadOnly);
			allText = outputFile.readAll();
			outputFile.close();
			break;
		}
	}

	QFile::remove(inputPath);
	RemoveDir(outputPath);
	return allText;
}

bool FileUtil::RemoveDir(const QString &dirName)
{
	bool result;
	QDir dir(dirName);

	if (dir.exists(dirName))
	{
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
		{
			if (info.isDir())
			{
				result = RemoveDir(info.absoluteFilePath());
			}
			else
			{
				result = QFile::remove(info.absoluteFilePath());
			}

			if (!result)
			{
				return result;
			}
		}
		result = dir.rmdir(dirName);
	}
	return result;
}

bool FileUtil::IsValidExtensionFile(const QString& strImageFileName, const QString& extension)
{
	QStringList acceptedImageFileTypeList;
	acceptedImageFileTypeList << extension;
	QStringList splittedStrList = strImageFileName.split(".");
	if(((splittedStrList.size())-1) >= 0)
	{
		QString fileExtensionToCheck = splittedStrList[((splittedStrList.size())-1)];
		if( acceptedImageFileTypeList.contains(fileExtensionToCheck, Qt::CaseInsensitive) )
			return true;
		else
			return false;
	}
	else
		return false;
}

bool FileUtil::IsDirDotTwo(const QString& dir)
{
	if(dir == "." || dir == "..")
	{
		return true;
	}
	return false;
}

QString FileUtil::CopyableFilename(QString filepath)
{
	while(QFile::exists(filepath))
	{
		QFileInfo info(filepath);
		filepath = info.absoluteDir().path() + "/" + info.completeBaseName() + "1." + info.suffix();
	}
	return filepath;
}

QString FileUtil::RandomFilename()
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	const int randomStringLength = 12;

	QString randomString;
	for(int i=0; i<randomStringLength; ++i)
	{
		int index = qrand() % possibleCharacters.length();
		QChar nextChar = possibleCharacters.at(index);
		randomString.append(nextChar);
	}
	return randomString;
}

bool FileUtil::CopyFolder(const QString& srcFilePath, const QString& destFilePath)
{
	QFileInfo srcFileInfo(srcFilePath);
	if(srcFileInfo.isDir())
	{
		QDir targetDir(destFilePath);
		QDir sourceDir(srcFilePath);
		QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
		foreach (const QString &fileName, fileNames)
		{
			const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + fileName;
			const QString newTgtFilePath = destFilePath + QLatin1Char('/') + fileName;
			if (!CopyFolder(newSrcFilePath, newTgtFilePath))
				return false;
		}
	}
	else
	{
		if (!CopyFile(srcFilePath, destFilePath))
			return false;
	}
	return true;
}

bool FileUtil::CopyFile(const QString& src, const QString& dest)
{
	QFile::remove(dest);
	QFile::copy(src, dest);
	return true;
}

bool FileUtil::GetFilePart(const QString& src, const QString& dest, int postion)
{
	QFile srcFile(src);
	if(!srcFile.open(QIODevice::ReadOnly))
		return false;
	QByteArray blob = srcFile.readAll();
	srcFile.close();

	QFile destFile(dest);
	if(!destFile.open(QIODevice::WriteOnly))
		return false;
	destFile.write(blob.left(postion));
	destFile.flush();
	destFile.close();
	return true;
}
