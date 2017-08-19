#pragma once

#include "../common/Qt.h"
#include "AudioVolumeIndicator.h"
#include "KeTime.h"
#include "TitleButton.h"
#include "SlidePresenter.h"

class LIANGSHIEXPORT LiveTitleWidget : public QWidget
{
	Q_OBJECT
public:
	LiveTitleWidget(QWidget *parent = 0);
	~LiveTitleWidget();

	void SetEnabled(bool whole, bool student, bool apply);

Q_SIGNALS:
	void OnInsertScFile(QString filepath);
	void OnNewPage();
	void OnNewPageWithBackground(QImage& image);
	void OnSwitchPage();
	void OnApplyBoard();
	void OnTakeBoard();
	void OnPreviousPage();
	void OnNextPage();
	void OnAddSlidePresenter(SlidePresenter* presenter);

public Q_SLOTS:
	void OnAudioVolumeTriggered(int percent);

private Q_SLOTS:
	void OnInsertTriggered();
	void OnNewPageTriggered();
	void OnSwitchPageTriggered();
#ifdef _LiangshiDesktop
	void OnImportTriggered();
#endif
	void OnApplyTriggered();
	void OnTakeTriggered();

private:
	QHBoxLayout* _mainLayout;
	bool _initFromNet;

	TitleButton* _insert;
	TitleButton* _new;
	TitleButton* _switch;
	TitleButton* _import;
	AudioVolumeIndicator* _localVolume;
	AudioVolumeIndicator* _remoteVolume;
	TitleButton* _apply;
	TitleButton* _take;
};

