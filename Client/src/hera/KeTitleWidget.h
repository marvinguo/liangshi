#pragma once

#include "../common/Qt.h"
#include "AudioVolumeIndicator.h"
#include "KeTime.h"
#include "TitleButton.h"
#include "SlidePresenter.h"

class LIANGSHIEXPORT KeTitleWidget : public QWidget
{
	Q_OBJECT
public:
	KeTitleWidget(KeTime* keTime, QWidget *parent = 0);
	~KeTitleWidget();

Q_SIGNALS:
	void OnInsertScFile(QString filepath);
	void OnAttachScFile(QString filepath);
	void OnNewPage();
	void OnNewPageWithBackground(QImage& image);
	void OnSwitchPage();
	void OnPreview();
	void OnPreviousPage();
	void OnNextPage();
	void OnAddSlidePresenter(SlidePresenter* presenter);

public Q_SLOTS:
	void OnAudioVolumeTriggered(int percent);
	void OnStartTriggered();
	void OnPauseTriggered();
	void OnTimeUpdateTriggered(int time);

private Q_SLOTS:
	void OnRecrodButtonTriggered();
	void OnInsertTriggered();
	void OnNewPageTriggered();
	void OnSwitchPageTriggered();
	void OnPreviewTriggered();
	void OnAddTestTriggered();
#ifdef _LiangshiDesktop
	void OnImportTriggered();
#endif

private:
	QLabel* _time;
	TitleButton* _record;
	TitleButton* _insert;
	TitleButton* _new;
	TitleButton* _switch;
	TitleButton* _preview;
	TitleButton* _test;
	TitleButton* _import;
	AudioVolumeIndicator* _volume;

	bool _paused;
	KeTime* _keTime;
};

