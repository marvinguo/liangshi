#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../hera/PlayerWidget.h"
#include "../../library/include/Ares.h"

class ConvertProgress : public QDialog
{
	Q_OBJECT
public:
	ConvertProgress(PlayerWidget* playerWidget, int width, int height, QWidget* parent = 0);

	void ShowModal();

private Q_SLOTS:
	void Convert();

private:
	void ConvertAudio();
	void ConvertVideo();
	void MixAudioVideo();
	void ConvertFrame(Ares* ares, int time);

private:
	QProgressBar* _progress;
	PlayerWidget* _playerWidget;
	int _width;
	int _height;
	int _totalCount;
	int _currentCount;
};

