#include "BanScriptDeps.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanScriptDeps::BanScriptDeps():
_depsTempFolder(FileUtil::GetTempFolder() + QUuid::createUuid().toString())
{
	QDir().mkdir(_depsTempFolder);
}

BanScriptDeps::~BanScriptDeps()
{
	FileUtil::RemoveDir(_depsTempFolder);
}

void BanScriptDeps::Init(QString depsFolder)
{
	QDir sourceDir(depsFolder);
	QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
	foreach (const QString &fileName, fileNames)
	{
		const QString newSrcFilePath = depsFolder + QLatin1Char('/') + fileName;
		AddFile(newSrcFilePath);
	}
}

bool BanScriptDeps::HasDepsFile(QString filename, QString& fullpath)
{
	QString filepath = _depsTempFolder + "/" + filename;
	QFileInfo info(filepath);
	if(info.exists())
	{
		fullpath = filepath;
		return true;
	}
	return false;
}

bool BanScriptDeps::HasFilename(QString filename)
{
	return std::find(_filenames.begin(), _filenames.end(), filename) != _filenames.end();
}

void BanScriptDeps::AddFile(QString filepath)
{
	QFileInfo info(filepath);
	QString filename = info.fileName();
	_filenames.push_back(filename);
	QString dest = _depsTempFolder + "/" + filename;
	FileUtil::CopyFile(filepath, dest);
}

void BanScriptDeps::Remove(QString filename)
{
	auto it = std::find(_filenames.begin(), _filenames.end(), filename);
	if(it != _filenames.end())
		_filenames.erase(it);
	QString dest = _depsTempFolder + "/" + filename;
	QFile::remove(dest);
}

void BanScriptDeps::OpenFile(QString filename)
{
	QString dest = _depsTempFolder + "/" + filename;
	//这个破内存泄漏，不管了
	QProcess* process = new QProcess();
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	QStringList arguments;
	arguments << dest;
	process->start("collect.exe", arguments);
}

void BanScriptDeps::CopyTo(QString folder)
{
	FileUtil::CopyFolder(_depsTempFolder, folder);
}

void BanScriptDeps::CopyFileTo(QString filename, QString dest)
{
	QString src = _depsTempFolder + "/" + filename;
	FileUtil::CopyFile(src, dest);
}