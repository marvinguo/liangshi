#pragma once

#include "SlidingWidget.h"
#include "MobileActivity.h"
#include "LoginActivity.h"
#include "ClasseActivity.h"
#include "MainMenuActivity.h"
#include "KeActivity.h"
#include "PlayerActivity.h"
#include "LiveActivity.h"
#include "PreviewActivity.h"
#include "TeachingActivity.h"
#include "OfflineClasseActivity.h"

#ifdef Q_OS_ANDROID
extern "C"
{
	JNIEXPORT jboolean JNICALL Java_org_qtproject_qt5_android_bindings_QtActivity_QtActivty_OnStop(JNIEnv *env, jobject context);
}
#endif

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);

	virtual void keyPressEvent(QKeyEvent *k);

public Q_SLOTS:
	void OnGotoLogin();
	void OnGotoMenu();
	void OnGotoNewMenu();
	void OnGotoClass();
	void OnGotoNewClass(ClassDTO dto);
	void OnGotoKe();
	void OnGotoNewCollect(QString id);
	void OnGotoNewPlay(bool classPlay, QString classTitle, KeDTO keDTO);
	void OnGotoNewLive(bool classLive, bool allTeachers, QString& session);
	void OnGotoPreview(bool offline, QString filepath);
	void OnGotoTeaching();
	void OnGotoNewTeaching(TeachingDTO dto);
	void OnGotoOffline();
	void NeedNetworkErrorTriggered();
	void UseOffliceTriggered();

private Q_SLOTS:
	void OnApplicationStateChanged(Qt::ApplicationState state);

private:
	SlidingWidget* _sliding;
	LoginActivity* _login;
	MainMenuActivity* _mainMenu;
	ClasseActivity* _class;
	KeActivity* _collect;
	PlayerActivity* _player;
	LiveActivity* _live;
	PreviewActivity* _preview;
	TeachingActivity* _teaching;
	OfflineClasseActivity* _offline;

	MobileActivity* _currentActivity;
};
