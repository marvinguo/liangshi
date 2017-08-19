#pragma once

#include "../common/Qt.h"

#include "ActivityTitleButton.h"
#include "ActivityTitleLabel.h"
#include "../hera/ColorButton.h"
#include "AndroidCamera.h"

class LiveTitlebar : public QWidget
{
	Q_OBJECT
public:
	LiveTitlebar(QWidget* parent = NULL);

	ActivityTitleButton* GetApplyButton(){return _apply;}
	ActivityTitleButton* GetTakeButton(){return _take;}

Q_SIGNALS:
	bool OnGetLocalCamera();
	void OnGetPixmap(QPixmap& pixmap);
	void OnLineColorChanged(QColor color);
	void OnApplyBoard();
	void OnTakeBoard();
	void OnShowVideo(bool show);

private Q_SLOTS:
	void OnCameraButtonTriggered();
	void OnVideoButtonTriggered();

private:
	ActivityTitleButton* _apply;
	ActivityTitleButton* _take;
	ActivityTitleButton* _camera;
	ActivityTitleButton* _video;
	ColorButton* _lineColor;
	bool _showVideo;
};

