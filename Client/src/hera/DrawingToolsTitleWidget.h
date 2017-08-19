#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/stl.h"
#include "Element.h"
#include "FourStateImageButton.h"
#include "BanCheckableButton.h"
#include "ColorButton.h"
#include "NumberButton.h"
#include "KeTime.h"
#include "TitleButton.h"
#include "TitleComboBox.h"
#include "TitleCheckBox.h"

class LIANGSHIEXPORT DrawingToolsTitleWidget : public QWidget
{
	Q_OBJECT
public:
	DrawingToolsTitleWidget(KeTime* keTime, QWidget *parent = 0);
	~DrawingToolsTitleWidget();

	TitleButton* GetPasteButton(){return _paste;}
	TitleButton* GetUndoButton(){return _undo;}
	TitleButton* GetRedoButton(){return _redo;}

Q_SIGNALS:
	void OnDraw();
	void OnLine();
	void OnRect();
	void OnPRect();
	void OnEllipse();
	void OnPEllipse();
	void OnSelect();
	void OnLineColorChanged(QColor color);
	void OnLineWidthChanged(int width);
	void OnLineStyleChanged(int style);
	void OnGetPixmap(QPixmap& pixmap);
	bool OnGetLocalCamera();
	void EnableReviseDraw(bool);

public Q_SLOTS:
	void OnStartTriggered();
	void OnPauseTriggered();
	void OnHighlightElementChangeTriggered(Element* element);

private Q_SLOTS:
	void OnDrawTriggered();
	void OnLineTriggered();
	void OnRectTriggered();
	void OnPRectTriggered();
	void OnEllipseTriggered();
	void OnPEllipseTriggered();
	void OnSelectTriggered();
	void OnPenColorChangedTriggered(QColor color);
	void OnPenWidthChangedTriggered(int width);
	void OnPenStyleChangeTriggered(const QString &title);
	void OnCheckedTriggered(bool checked);
	void OnScreenshotTriggered();
	void OnCameraTriggered();
	void OnReviseChecked(int state);

private:
	void EnableButtons(bool enabled);

private:
	//有一天，可能需要freeDraw而不是vector
	ScCheckableButton* _freeDraw;
	ScCheckableButton* _draw;
	ScCheckableButton* _line;
	ScCheckableButton* _rect;
	ScCheckableButton* _pRect;
	ScCheckableButton* _ellipse;
	ScCheckableButton* _pEllipse;
	ScCheckableButton* _select;

	TitleComboBox* _lineStyle;
	TitleCheckBox* _revise;

	TitleButton* _paste;
	TitleButton* _undo;
	TitleButton* _redo;
	TitleButton* _screenshot;
	TitleButton* _camera;

	NumberButton* _lineWidth;
	ColorButton* _lineColor;

	KeTime* _keTime;
};

