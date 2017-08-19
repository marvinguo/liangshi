#pragma once

#include "../common/library.h"
#include "AnimationAction.h"
#include "Element.h"
#include "HeraDocument.h"
#include "BanBaseWidget.h"
#include "KeTime.h"
#include "Menu.h"
#include "SlidePresenter.h"

class LIANGSHIEXPORT KeWidget : public QWidget, public HeraDocument, public sigslot_liangshi::has_slots<>
{
	Q_OBJECT
	friend class KeFile;
	friend class KeActivity;
	friend class PlayerTitleWidget;
	friend class BanTable;
	friend class ActionTable;

public:
	KeWidget(KeTime* keTime, QWidget *parent = 0);
	virtual ~KeWidget();

//HeraDocument virtual
public:
	virtual bool CanQuit();
	virtual QString GetSaveFileDialongExtension();
	virtual void UpdateFilepath(QString filepath);
	virtual bool HasSavedPath();
	virtual bool SaveDocument(QString filepath = "");
	virtual bool SaveAsDocument(QString filepath);
	virtual QWidget* Convert2QWidget(){return static_cast<QWidget*>(this);}

	//hades signals
	void OnAudioVolume(int percent);
	void OnRecordTime(int time);

public:
	int GetType(){return 1030;}
	LokiBuffer GetMsgPackData();
	void SetMsgPackData(LokiBuffer& buffer);
	void InitKe(QPixmap& pixmap);
	void InitHadesFromExistingFile(QString hadesFile, int seek, int time);

Q_SIGNALS:
	void OnSaveFileNameChanged(QString filename, bool modifysign);
	void OnAudioVolumeChanged(int percent);
	void OnHighlightElementChanged(Element* element);

public Q_SLOTS:
	void DrawModeTriggered();
	void LineModeTriggered();
	void RectModeTriggered();
	void PerectRectModeTriggered();
	void EclipseModeTriggered();
	void PerfectEclipseModeTriggered();
	void SelectModeTriggered();
	void LineWidthTriggered(int width);
	void LineStyleTriggered(int style);
	void LineColorTriggered(QColor color);
	void ReviseDrawTriggered(bool ok);
	void OnPaste();
	void OnUndo();
	void OnRedo();
	void OnInsertPixmapTriggered(QPixmap& pixmap);
	void OnInsertPixmapBackgroundTriggered(QPixmap& pixmap);

public Q_SLOTS:
	void OnRecordTriggered();
	void OnPauseTriggered();
	void OnInsertBanTriggered(QString filepath);
	void OnAttachBanTriggered(QString filepath);
	void OnAddSlidePresenterTriggered(SlidePresenter* presenter);
	void OnNewPageTriggered();
	void OnNewPageWithBackgroundTriggered(QImage& image);
	void OnSwitchPageTriggered();
	void OnPreviousPageTriggered();
	void OnNextPageTriggered();
	void OnPageSizeChange(QSize size);

private Q_SLOTS:
	void OnSelectPageTriggered(int page);
	void AddActionTriggered(AnimationAction& action, Element* element);
	void OnCurrentBanFileNameChanged(QString filename, bool modifysign);

private:
	void SendSwitchPageAnimation(BanBaseWidget* widget);
	void AddPage(BanBaseWidget* page, bool hasAction = true);

private:
	KeTime* _keTime;
	Hades* _hades;
	QString _hadesFile;
	bool _hasStarted;

	QStackedLayout* _mainLayout;

	vector<AnimationAction*> _actions;
	vector<BanBaseWidget*> _allPages;
	int _currentPageIndex;
	BanBaseWidget* _currentPage;
	vector<QString> _attachedTests;

	DrawMode _cachedMode;
	int _cachedWidth;
	int _cachedStyle;
	QColor _cachedColor;
	bool _cachedReviseDraw;

	bool _dirty;
	QString _filepath;

	//need save
private:
	int _pageWidth;
	int _pageHeight;
	int _totalTime;

public:
	MSGPACK_DEFINE(_pageWidth, _pageHeight, _totalTime);
};

