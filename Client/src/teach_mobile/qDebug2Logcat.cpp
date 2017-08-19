#include "qDebug2Logcat.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


#if defined(Q_OS_ANDROID)

static const char *g_TAG = 0;

static void messageOutput2Logcat(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	int prio = ANDROID_LOG_VERBOSE;
	QByteArray localMsg = msg.toUtf8();
	switch (type) {
	case QtDebugMsg:
		prio = ANDROID_LOG_DEBUG;
		break;
	case QtWarningMsg:
		prio = ANDROID_LOG_WARN;
		break;
	case QtCriticalMsg:
		prio = ANDROID_LOG_INFO;
		break;
	case QtFatalMsg:
		prio = ANDROID_LOG_FATAL;
		abort();
	}
	__android_log_write(prio, g_TAG, localMsg.data());
}

void installLogcatMessageHandler(const char *TAG)
{
	g_TAG = (TAG == 0 ? "QDebug" : TAG);
	qInstallMessageHandler(messageOutput2Logcat);
}
#endif
