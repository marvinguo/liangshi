#pragma once

#include "Menu.h"
#include "Element.h"
#include "HeraDocument.h"
#include "AnimationAction.h"
#include "BanFile.h"
#include "BanDocument.h"
#include "ImageBuffer.h"
#include "CompressedImageBuffer.h"

enum DrawMode
{
	DrawMode_Draw,
	DrawMode_Line,
	DrawMode_Rect,
	DrawMode_PerfectRect,
	DrawMode_Ellipse,
	DrawMode_PerfectEllipse,
	DrawMode_Select,
};

class LIANGSHIEXPORT BanBaseWidget : public QWidget, public BanDocument
{
	Q_OBJECT
	friend class HeraMainWidget;
	friend class BanElement;
	friend class KeWidget;
	friend class LiveWidget;
	friend class PlayerPage;
	friend class PlayerTitleWidget;
	friend class BanElementScriptValue;
	friend class ImageElementScriptValue;
	friend class TextElementScriptValue;
	friend class KeFile;
	friend class Slide;
	friend class SlidePresenter;
public:
	BanBaseWidget(bool embedded = false, QWidget *parent = 0);
	virtual ~BanBaseWidget();

public:
	QString GetObjectUuid();
	void SetMode(DrawMode mode);
	void DrawOnQtPainter(QPainter* painter);
	int GetLayer(){return _layer;}
	QPixmap ToFullPixmap(double xZoom = 1, double yZoom = 1, bool withBackground = false);
	void SetLayer(int layer);
	bool CanEditElementInside(){return _canEditInside;}
	void SetElementMatrix(QMatrix matrix){_elementMatrix = matrix;}
	void SetWorldMatrix(QMatrix matrix){_worldMatrix = matrix;}
	//for slides functions
	void ChangeImageElementSize(string uuid, QPoint leftTop, QSize size);
	void AddToDrawing(string uuid);
	void RemoveFromDrawing(string uuid);

//HeraDocument virtual
public:
	virtual bool CanQuit();
	virtual QString GetSaveFileDialongExtension();
	virtual void UpdateFilepath(QString filepath);
	virtual bool HasSavedPath();
	virtual bool SaveDocument(QString filepath = "");
	virtual bool SaveAsDocument(QString filepath);
	virtual QWidget* Convert2QWidget(){return static_cast<QWidget*>(this);}

//QWidget virtual 
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual bool eventFilter(QObject *, QEvent *);
	virtual void resizeEvent(QResizeEvent *e);
	virtual void paintEvent(QPaintEvent *e);
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent *);
	virtual void dragEnterEvent(QDragEnterEvent *e);
	virtual void dropEvent(QDropEvent *e);

//MaterialDocWidget virtual 
public:
	virtual ScType GetType();
	virtual QPixmap ToPixmap(double xZoom = 1, double yZoom = 1, bool withBackground = false, bool refresh = true);
	virtual QPixmap ToFixedPixmap(int x, int y, bool withBackground = false);
	virtual LokiBuffer GetMsgPackData(bool init = false);
	virtual void SetMsgPackData(LokiBuffer& buffer);
	virtual void UpdateDataFromMsgPack();

Q_SIGNALS:
	void OnRedoChanged(bool canRedo);
	void OnUndoChanged(bool canUndo);
	void OnSaveFileNameChanged(QString filename, bool modifysign);
	void OnAnimationAction(AnimationAction& action, Element* element);
	void OnBitmapSizeChanged(QSize size);
	void OnHighlightElementChanged(Element* element);

public Q_SLOTS:
	void DrawModeTriggered();
	void LineModeTriggered();
	void RectModeTriggered();
	void PerectRectModeTriggered();
	void EclipseModeTriggered();
	void PerfectEclipseModeTriggered();
	void SelectModeTriggered();
	void LineWidthTriggered(int width){ LineWidthChanged(width); }
	void LineStyleTriggered(int style){ LineStyleChanged(style); }
	void LineColorTriggered(QColor color){ LineColorChanged(color); }
	void ReviseDrawTriggered(bool ok);
	void OnCopyTriggered();
	void OnPasteTriggered();
	void OnUndoTriggered();
	void OnRedoTriggered();
	void OnInsertPixmapTriggered(QPixmap& pixmap);
	void OnInsertPixmapBackgroundTriggered(QPixmap& pixmap);
	void OnInsertBanTriggered(QString filepath);
	void OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete);

	virtual void RemoveDrawingElementTriggered(Element* element);
	void OnTopMostElementTriggered(Element* element);
	void OnBottomMostElementTriggered(Element* element);
	void SetModifiedTriggered(bool modified = true);

protected:
	void Repaint(bool updateBackgroud = true);
	void LetDrawingPixmapReady();
	void SetPainterStyle(QPainter& painter);
	void RenderPixmap(QPixmap& pixmap);
	QRect GetUnitedRect(double xZoom = 1, double yZoom = 1);
	void HighlightElement(QPoint pos, bool repaint = true);
	void DeHighlightElement(bool repaint = true);
	void SetHighlightElement(Element* element);
	void InitShortCutKey();
	void AddElement(Element* element, bool updateProperty = true, bool highlight = true, bool addToDrawing = true);
	Element* FindElementByUuid(string uuid);
	void ResetElementToInit();
	void ClearAll();
	void ChangeModeCursor();
	QPoint GetCursorPoint();
	QCursor GetDrawingCursor(QString resource);
	void LineWidthChanged(int width, bool all = false);
	void LineStyleChanged(int style, bool all = false);
	void LineColorChanged(QColor color, bool all = false);

protected:
	bool _reviseDraw;
	bool _embedded;
	bool _dirty;
	bool _disablePaste;
	bool _disableDraw;
	bool _disableHighlight;
	bool _disableDrop;
	bool _redoing;
	bool _needSaveScript;
	bool _canEditInside;
	bool _emitAdditionAction;
	int _layer;

	QPixmap _backgroudPixmap;
	QPixmap _drawingPixmap;

	//只有keWidget需要记录所有的element
	//板书里不需要所有，但是现在简化，都这样吧
	set<Element*> _allElements;
	list<Element*> _drawingElements;
	vector<Element*> _undos;
	Element* _highlightElement;

	bool _isDrawing;
	DrawMode _mode;
	QPoint _clickPoint;
	vector<QPoint> _movingPoints;

	SColor _lineColor;
	QColor _lineQColor;
	int _lineWidth;
	Qt::PenStyle _lineStyle;

	//当前的matrix,在新加入的element里，使用
	QMatrix _elementMatrix;
	QMatrix _worldMatrix;

	QUuid _uuid;
	LokiBuffer _zipCode;
	QPixmap _cachePixmap;
	CompressedImageBuffer _backgroundImage;

public:
	MSGPACK_DEFINE(_zipCode, _backgroundImage);
};

