#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "jsedit.h"

class LIANGSHIEXPORT BanScriptEditor : public JSEdit
{
	Q_OBJECT
public:
	BanScriptEditor(QWidget *parent = 0);
	virtual ~BanScriptEditor();

//virtual
private:
	virtual void closeEvent(QCloseEvent *e);

private:
};

