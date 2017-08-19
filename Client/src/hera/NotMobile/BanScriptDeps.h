#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT BanScriptDeps
{
public:
	BanScriptDeps();
	~BanScriptDeps();

	void Init(QString depsFolder);
	bool HasDepsFile(QString filename, QString& fullpath);
	vector<QString> GetFileNames(){return _filenames;}
	bool HasFilename(QString filename);
	void AddFile(QString filepath);
	void Remove(QString filename);
	void OpenFile(QString filename);
	void CopyTo(QString folder);
	void CopyFileTo(QString filename, QString dest);

private:
	QString _depsTempFolder;
	vector<QString> _filenames;
};

