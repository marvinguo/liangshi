#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "../common/LokiClient.h"
#include "AnimationAction.h"
#include "Element.h"
#include "HeraDocument.h"
#include "BanBaseWidget.h"

#include "SwitchPageAction.h"
#include "NetworkShowAction.h"
#include "NetworkSwitchPageAction.h"
#include "SizeAction.h"
#include "ShowAction.h"
#include "HideAction.h"
#include "LineStyleAction.h"
#include "LineWidthAction.h"
#include "LineColorAction.h"
#include "TopMostAction.h"
#include "BottomMostAction.h"
#include "TextChangedAction.h"
#include "VideoWidget.h"
#include "MobileVideoWidget.h"
#include "SlidePresenter.h"

#include "../../../SharedCode/LokiCommand.h"

struct VideoWidgets
{
	VideoWidget* local;
	VideoWidget* remote;
};


class LIANGSHIEXPORT LiveWidget : public QWidget, public HeraDocument, public sigslot_liangshi::has_slots<>
{
	Q_OBJECT
public:
	LiveWidget(QString session = "", QWidget *parent = 0);
	virtual ~LiveWidget();

//HeraDocument virtual
public:
	virtual bool CanQuit();
	virtual QString GetSaveFileDialongExtension();
	virtual void UpdateFilepath(QString filepath);
	virtual bool HasSavedPath();
	virtual bool SaveDocument(QString filepath = "");
	virtual bool SaveAsDocument(QString filepath);
	virtual QWidget* Convert2QWidget(){return static_cast<QWidget*>(this);}
	void OnAudioVolume(int percent);
	void OnSdp(const char* identifier, const char* type, const char* sdp);
	void OnCandidateSdp(const char* identifier, const char* sdp_mid, int stp_mline, const char* sdp);

Q_SIGNALS:
	void OnAudioVolumeChanged(int percent);
	void OnLiveCanApplyAndTake(bool whole, bool student, bool apply);
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
	bool OnGetLocalCameraTriggered();

public Q_SLOTS:
	void OnInsertBanTriggered(QString filepath);
	void OnAddSlidePresenterTriggered(SlidePresenter* presenter);
	void OnNewPageTriggered();
	void OnNewPageWithBackgroundTriggered(QImage& image);
	void OnSwitchPageTriggered();
	void OnPreviousPageTriggered();
	void OnNextPageTriggered();
	void OnApplyBoardTriggered();
	void OnTakeBoardTriggered();
#ifndef _LiangshiDesktop
	void OnVideoWidget(bool show);
#endif

private Q_SLOTS:
	void OnSelectPageTriggered(int page);
	void AddActionTriggered(AnimationAction& action, Element* element);

//network slots
private Q_SLOTS:
	void OnConnected();
	void OnBoardLoginTriggered(Loki::BoardLoginCommand command);
	void OnHadesDataTriggered(QString peer_id, QString nickname, LokiBuffer& data);
	void OnBoardDataTriggered(LokiBuffer& data);
	void OnTeacherAgreeBoardTriggered(bool another, bool agree);
	void OnStudentApplyBoardTriggered(QString student_id, QString nickname);
	void OnTeacherTakeBoardTriggered();
	void OnSocketErrorTriggered(QAbstractSocket::SocketError socketError);
	void OnPageSizeChange(QSize size);

private:
	virtual void resizeEvent(QResizeEvent *e);
	Hades* GetHades(int sdptype, QString peer_id, QString nickname, bool has_video, bool caller = true);
	void ShowNetworkRetry();
	void NewFirstPage();
	void SetEnableCurrentPage(bool enable);
	void SendSwitchPageAnimation(BanBaseWidget* widget);
	void PlayAction(AnimationAction* action);
	void PlayNetworkShowAction(NetworkShowAction* action);
	void PlayNetworkSwitchPageAction(NetworkSwitchPageAction* action);
	void PlaySizeAction(SizeAction* action);
	void PlayHideAction(HideAction* action);
	void PlayLineWidthAction(LineWidthAction* action);
	void PlayLineStyleAction(LineStyleAction* action);
	void PlayLineColorAction(LineColorAction* action);
	void PlayTopMostAction(TopMostAction* action);
	void PlayBottomMostAction(BottomMostAction* action);
	void PlayTextChangedAction(TextChangedAction* action);

private:
	//我觉得需要区分主动？被动，初始化过？
	map<QString, Hades*> _n2nHades;

#ifdef _LiangshiDesktop
	map<QString, VideoWidgets> _n2nVideoWidgets;
#else
	MobileVideoWidget* _videoWidget;
#endif

	QStackedLayout* _mainLayout;

	vector<BanBaseWidget*> _allPages;
	int _currentPageIndex;
	BanBaseWidget* _currentPage;

	DrawMode _cachedMode;
	int _cachedWidth;
	int _cachedStyle;
	QColor _cachedColor;
	bool _cachedReviseDraw;

	bool _controlingBoard;
	QString _session;

	LokiClient _lokiClient;

	QMatrix _elementMatrix;
	QMatrix _worldMatrix;
};

