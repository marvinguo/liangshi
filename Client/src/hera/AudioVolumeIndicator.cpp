#include "AudioVolumeIndicator.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

AudioVolumeIndicator::AudioVolumeIndicator(QString title, QWidget *parent):
QProgressBar(parent)
{
	setMaximum(100);
	setMinimum(0);
	setTextVisible(true);
	setFormat(title);
	setValue(0);
}

AudioVolumeIndicator::~AudioVolumeIndicator()
{
}
