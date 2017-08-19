#pragma once

#include "../common/Qt.h"

#include "ActivityTitleButton.h"
#include "ActivityTitleLabel.h"
#include "../hera/KeTime.h"
#include "../hera/ColorButton.h"
#include "AndroidCamera.h"

class KeTitlebar : public QWidget
{
	Q_OBJECT
public:
	KeTitlebar(QWidget* parent = NULL);

	void SetKeTime(KeTime* keTime);

Q_SIGNALS:
	void OnLineColorChanged(QColor color);
	void OnGetPixmap(QPixmap& pixmap);
	void OnPreview();

public Q_SLOTS:
	void OnRecordButtonTriggered();
	void OnCameraButtonTriggered();
	void OnStartTriggered();
	void OnPauseTriggered();
	void OnTimeUpdateTriggered(int time);
	void OnCameraContinue(bool needContinue);

private:
	ActivityTitleButton* _record;
	ActivityTitleButton* _camera;
	ActivityTitleButton* _preview;
	ActivityTitleLabel* _time;
	ColorButton* _lineColor;
	KeTime* _keTime;
	bool _paused;
};

