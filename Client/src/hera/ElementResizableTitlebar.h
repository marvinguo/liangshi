#pragma once

#include "../common/Qt.h"
#include "TitleButton.h"

class LIANGSHIEXPORT ElementResizableTitlebar : public QWidget
{
	Q_OBJECT
public:
	ElementResizableTitlebar(QWidget *parent = 0);
	~ElementResizableTitlebar();

	TitleButton* GetDeleteButton(){return _delete;}
	TitleButton* GetEditButton(){return _edit;}
	TitleButton* GetTitleButton(){return _title;}
	TitleButton* GetTopMostButton(){return _topMost;}
	TitleButton* GetBottomMostButton(){return _bottomMost;}
	
protected:
	TitleButton* _delete;
	TitleButton* _edit;
	TitleButton* _title;
	TitleButton* _topMost;
	TitleButton* _bottomMost;
};

