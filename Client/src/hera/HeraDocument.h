#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

//HeraMainWidget里的内嵌，主要是要支持退出功能
//这个以后要改，现在来看，更多的是接口功能
class LIANGSHIEXPORT HeraDocument
{
protected:
	HeraDocument();
	virtual ~HeraDocument();

public:
	virtual bool CanQuit() = 0;
	virtual QString GetSaveFileDialongExtension() = 0;
	virtual void UpdateFilepath(QString filepath) = 0;
	virtual bool HasSavedPath() = 0;
	virtual bool SaveDocument(QString filepath = "") = 0;
	virtual bool SaveAsDocument(QString filepath) = 0;
	virtual QWidget* Convert2QWidget() = 0;
};

