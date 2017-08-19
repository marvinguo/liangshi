#pragma once

#include "../common/Qt.h"
#include "FourStateImageButton.h"
#include "TwoStateImageButton.h"

class LIANGSHIEXPORT SysTitleWidget : public QWidget
{
	Q_OBJECT
public:
	SysTitleWidget(bool hasMax, bool hasFull = false, QWidget *parent = 0);
	~SysTitleWidget();

Q_SIGNALS:
	void OnMin();
	void OnMax();
	void OnClose();

public Q_SLOTS:
	void OnMaxTriggered();

private Q_SLOTS:
	void OnFullscreenTriggered();
	void OnMinTriggered();
	void OnCloseTriggered();

private:
	FourStateImageButton* _min;
	FourStateImageButton* _max;
	FourStateImageButton* _close;
	TwoStateImageButton* _full;

	bool _isMax;
	bool _hasMax;
	bool _isFullscreen;
	QByteArray _savedGeometry;
};

