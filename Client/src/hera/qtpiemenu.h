#pragma once

#include "qtpieitem.h"

class QtPieMenu : public QtPieItem
{
	Q_OBJECT

private:
	enum ActivateReason
	{
		MousePress = 0x01,
		MouseRelease = 0x02,
		KeyPress = 0x04,
		Hovering = 0x08,
	};

public:
	QtPieMenu(const QString &title, QWidget *parent = 0, const char *name = 0, uint innerRad = 15, uint outerRad = 75);
	QtPieMenu(const QIcon &icon, QWidget *parent = 0, const char *name = 0, uint innerRad = 15, uint outerRad = 75);
	QtPieMenu(const QIcon &icon, const QString &title, QWidget *parent = 0, const char *name = 0, uint innerRad = 15, uint outerRad = 75);
	virtual ~QtPieMenu();

	void SetItemText(const QString &text, int index);
	QString GetItemText(int index) const;

	void SetItemIcon(const QIcon &icon, int index);
	QIcon GetItemIcon(int index) const;

	void SetItemWeight(int weight, int index);
	int GetItemWeight(int index) const;

	void SetItemEnabled(bool enabled, int index);
	bool IsItemEnabled(int index) const;


	void InsertItem(const QIcon &icon, QObject *receiver, const char *member, int index = -1);
	void InsertItem(const QString &text, QVariant data, QObject *receiver, const char *member, int index = -1);
	void InsertItem(const QString &text, QObject *receiver, const char *member, int index = -1);
	void InsertItem(const QIcon &icon, const QString &text, QObject *receiver, const char *member, int index = -1);
	void InsertItem(QtPieMenu *item, int index = -1);

	void Popup(const QPoint &pos);
	void HideMenu();
	int Count() const{return _items.size();};

	int IndexAt(const QPoint &pos);

	QtPieMenu *SubMenuAt(int index) const;
	void RemoveItemAt(int index);
	void Clear();

	void SetHighlightedItem(int index);
	int HighlightedItem();

	void SetInnerRadius(int r);
	int GetInnerRadius() const;

	void SetOuterRadius(int r);
	int GetOuterRadius() const;

	bool IsShaded() const;
	QtPieItem::ItemType GetItemType(int index) const;

//QWidget virtuals
private:
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual bool event(QEvent *e);   
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void keyPressEvent(QKeyEvent *);
	virtual void showEvent(QShowEvent *);
	virtual void hideEvent(QHideEvent *);
	virtual QSize sizeHint() const;

Q_SIGNALS:
	void activated();
	void activated(int);
	void highlighted(int);
	void canceled();
	void canceledAll();
	void aboutToShow();
	void aboutToHide();

protected Q_SLOTS:
	void HoverAlertTriggered();
	void SubMenuCancelTriggered();
	void SubMenuSelectTriggered();
	void AllCancelTriggered();

private:
	void Init(const QString &name);

	void DeleteItems(int index = -1);
	void GenerateMask(QBitmap *mask);
	void Reposition();

	double RadiusAt(const QPoint &pos) const;
	double AngleAt(const QPoint &pos) const;
	double IndexToAngle(int item, int total, bool shifted = false);
	int AngleToIndex(double rad, int total);

	void ActivateItem(int reason);

	bool IsTopLevelMenu() const;

	void HideShowingSubMenu();
	
	QtPieItem *HighlightedItemPtr();
	QtPieItem *ShownItemPtr();
	QtPieItem *ItemAt(int index) const;

private:
	QPixmap _maskPixmap;

	int _innerRadius;
	int outerRadius;

	QTimer _hoverTimer;
	QPoint _lastMousePoint;

	QList<QtPieItem *> _items;
	int _currentItemIndex;
	int _lastActivatedItemIndex;
	int _showingSubMenuIndex;

	bool _IsShaded;
	bool _ignoreNextMouseReleaseIfNotDragging;
	bool _hasMask;
};