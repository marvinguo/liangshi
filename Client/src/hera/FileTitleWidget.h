#pragma once

#include "../common/Qt.h"
#include "OneImageButton.h"
#include "../common/library.h"

enum FileTitleType
{
	FileTitleType_WithOpenWithoutSave,
	FileTitleType_WithSaveWithoutOpen,
};

class LIANGSHIEXPORT FileTitleWidget : public QWidget
{
	Q_OBJECT
public:
	FileTitleWidget(FileTitleType type, QWidget *parent = 0);
	~FileTitleWidget();

	OneImageButton* GetOpen(){return _open;}
	OneImageButton* GetSave(){return _save;}
	OneImageButton* GetSaveAs(){return _saveAs;}

Q_SIGNALS:

private Q_SLOTS:

protected:

private:
	OneImageButton* _open;
	OneImageButton* _save;
	OneImageButton* _saveAs;
};

