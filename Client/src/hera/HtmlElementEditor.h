#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"
#include "HtmlElementViewer.h"
#include "HtmlTextElement.h"

class LIANGSHIEXPORT HtmlElementEditor : public QDialog, public QtSingleton<HtmlElementEditor>
{
	Q_OBJECT
public:
	HtmlElementEditor();
	~HtmlElementEditor();
	void ShowModal(HtmlTextElement* element);

private Q_SLOTS:
	void OnEditTextChangeTriggered();
	void OnOpenTriggered();
	void OnSaveTriggered();
	void OnOKTriggered();

private:
	HtmlElementViewer* _viewer;
	HtmlTextElement* _element;
	QTextEdit* _edit;
	QVBoxLayout* _mainLayout;
};

