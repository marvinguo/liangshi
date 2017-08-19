#pragma once
#include "../common/Qt.h"

#if defined(Q_OS_ANDROID)
	#include <android/log.h>
	void installLogcatMessageHandler(const char *TAG);
#else
	#define installLogcatMessageHandler(TAG)
#endif
