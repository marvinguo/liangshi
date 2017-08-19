#include "qtpiemenu.h"
#include "qtpieaction.h"
#include <math.h>
#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

const double PI = 3.14159265358979323846264338327950288419717;
const double TWOPI = 2.0*PI;

QtPieMenu::QtPieMenu(const QString &text, QWidget *parent, const char *name, uint innerRadius, uint outerRadius):
QtPieItem(text, 1, parent),
	_maskPixmap(outerRadius * 2 + 1, outerRadius * 2 + 1),
	_hoverTimer(this),
	_innerRadius(innerRadius), outerRadius(outerRadius)
{
	Init(name);
}

QtPieMenu::QtPieMenu(const QIcon &icon, QWidget *parent, const char *name, uint innerRadius, uint outerRadius):
QtPieItem(icon, QString(), 1, parent),
	_maskPixmap(outerRadius * 2 + 1, outerRadius * 2 + 1),
	_hoverTimer(this),
	_innerRadius(innerRadius), outerRadius(outerRadius)
{
	Init(name);
}

QtPieMenu::QtPieMenu(const QIcon &icon, const QString &text, QWidget *parent, const char *name, uint innerRadius, uint outerRadius):
QtPieItem(icon, text, 1, parent),
	_maskPixmap(outerRadius * 2 + 1, outerRadius * 2 + 1),
	_hoverTimer(this),
	_innerRadius(innerRadius), outerRadius(outerRadius)
{
	Init(name);
}

void QtPieMenu::Init(const QString &name)
{
#if defined(QTSOLUTIONS_BUILDEVAL)
	void checkEval();
	checkEval();
#endif
	_hasMask = false;
	setMouseTracking(true);

	_currentItemIndex = -2;
	_showingSubMenuIndex = -1;
	_IsShaded = false;
	_ignoreNextMouseReleaseIfNotDragging = false;
	_lastActivatedItemIndex = -1;

	setObjectName(name);

	connect(&_hoverTimer, SIGNAL(timeout()), SLOT(HoverAlertTriggered()));
	setAttribute(Qt::WA_AcceptTouchEvents, true);

	SetType(SubMenu);
}

QtPieMenu::~QtPieMenu()
{
	DeleteItems();
}

void QtPieMenu::SetItemText(const QString &text, int index)
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::setItemText(\"%s\", %i) index of of range", text.toLatin1().constData(), index);
		return;
	}

	ItemAt(index)->SetText(text);
}

QString QtPieMenu::GetItemText(int index) const
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::itemText(%i) index of of range", index);
		return "";
	}

	return ItemAt(index)->GetText();
}

void QtPieMenu::SetItemIcon(const QIcon &icon, int index)
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::setItemIcon(%p, %i) index of of range", &icon, index);
		return;
	}

	ItemAt(index)->SetIcon(icon);
}

QIcon QtPieMenu::GetItemIcon(int index) const
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::itemIcon(%i) index of of range", index);
		return QIcon();
	}

	return ItemAt(index)->GetIcon();
}

void QtPieMenu::SetItemWeight(int weight, int index)
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::setItemWeight(%i, %i) index of of range", weight, index);
		return;
	}

	ItemAt(index)->SetWeight(weight);
}

int QtPieMenu::GetItemWeight(int index) const
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::itemWeight(%i) index of of range", index);
		return 0;
	}

	return ItemAt(index)->GetWeight();
}

void QtPieMenu::SetItemEnabled(bool enabled, int index)
{
	if (!ItemAt(index)) {
		qWarning("QtPieMenu::setItemEnabled(%s, %i) index of of range", enabled ? "true" : "false", index);
		return;
	}

	ItemAt(index)->SetEnabled(enabled);
}

bool QtPieMenu::IsItemEnabled(int index) const
{
	if (!ItemAt(index))
	{
		qWarning("QtPieMenu::isItemEnabled(%i) index of of range", index);
		return false;
	}

	return ItemAt(index)->IsEnabled();
}

bool QtPieMenu::IsTopLevelMenu() const
{
	return !parent()->inherits("QtPieMenu");
}

void QtPieMenu::SetInnerRadius(int r)
{
	_innerRadius = r;
}

int QtPieMenu::GetInnerRadius() const
{
	return _innerRadius;
}

void QtPieMenu::SetOuterRadius(int r)
{
	outerRadius = r;
	_maskPixmap = _maskPixmap.scaled(outerRadius * 2 + 1, outerRadius * 2 + 1);
}

int QtPieMenu::GetOuterRadius() const
{
	return outerRadius;
}

void QtPieMenu::Popup(const QPoint &pos)
{
	_currentItemIndex = IndexAt(mapFromGlobal(QCursor::pos()));
	_lastMousePoint = pos;
	_lastActivatedItemIndex = -1;

	_IsShaded = false;

	// Force whole pie menu to be inside desktop.
	int x = pos.x() - sizeHint().width() / 2;
	int y = pos.y() - sizeHint().height() / 2;

	const QRect screenGeometry = qApp->desktop()->screenGeometry(qApp->desktop()->screenNumber(pos));

	if (x < screenGeometry.left())
		x = screenGeometry.left();
	if (y < screenGeometry.top())
		y = screenGeometry.top();

	if (x + sizeHint().width() > screenGeometry.right())
		x = screenGeometry.right() - sizeHint().width();
	if (y + sizeHint().height() > screenGeometry.bottom())
		y = screenGeometry.bottom() - sizeHint().height();

	move(x, y);
	Reposition();
	show();
}

void QtPieMenu::InsertItem(QtPieMenu *item, int index)
{
	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > _items.size())
		index = _items.size();

	_items.insert(index, item);

	connect(item, SIGNAL(activated()), SLOT(SubMenuSelectTriggered()));
	connect(item, SIGNAL(canceled()), SLOT(SubMenuCancelTriggered()));
	connect(item, SIGNAL(canceledAll()), SLOT(AllCancelTriggered()));
}

void QtPieMenu::InsertItem(const QString &text, QVariant data, QObject *receiver,
	const char *member, int index)
{
	QtPieAction *action = new QtPieAction(text, data, receiver, member);

	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > int(_items.size()))
		index = _items.size();

	_items.insert(index, action);
}

void QtPieMenu::InsertItem(const QString &text, QObject *receiver,
	const char *member, int index)
{
	QtPieAction *action = new QtPieAction(text, receiver, member);

	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > int(_items.size()))
		index = _items.size();

	_items.insert(index, action);
}

void QtPieMenu::InsertItem(const QIcon &icon, QObject *receiver, const char *member, int index)
{
	QtPieAction *action = new QtPieAction(icon, QString::null, receiver, member);

	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > int(_items.size()))
		index = _items.size();

	_items.insert(index, action);
}

void QtPieMenu::InsertItem(const QIcon &icons, const QString &text,
	QObject *receiver, const char *member, int index)
{
	QtPieAction *action = new QtPieAction(icons, text, receiver, member);

	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > int(_items.size()))
		index = _items.size();

	_items.insert(index, action);
}

QtPieItem *QtPieMenu::ItemAt(int index) const
{
	// Out of bounds indexes are treated as appends.
	if (index < 0 || index > int(_items.size()))
		return 0;

	return (static_cast<const QtPieMenu * >(this))->_items[index];
}

void QtPieMenu::HideMenu()
{
	HideShowingSubMenu();
}

void QtPieMenu::Clear()
{
	HideShowingSubMenu();

	DeleteItems();

	_currentItemIndex = -2;
}

QtPieMenu *QtPieMenu::SubMenuAt(int index) const
{
	QtPieItem *item = (static_cast<const QtPieMenu *>(this))->_items[index];
	if (item && item->GetType() == SubMenu)
		return static_cast<QtPieMenu *>(item);

	return 0;
}

void QtPieMenu::RemoveItemAt(int index)
{
	if (index >= int(_items.size()) || index < 0) {
		qWarning("Attempt to remove item %i from QtPieMenu with %i items.",
			index, _items.size());
		return;
	}

	DeleteItems(index);
}

void QtPieMenu::SetHighlightedItem(int index)
{
	_currentItemIndex = index;
	update();
}

QSize QtPieMenu::sizeHint() const
{
	return QSize(outerRadius * 2 + 1, outerRadius * 2 + 1);
}

void QtPieMenu::GenerateMask(QBitmap *mask)
{
	QPainter p(mask);
	p.setPen(Qt::color1);
	p.setBrush(Qt::color1);
	p.drawEllipse(mask->rect().adjusted(0, 0, -1, -1));
}

void QtPieMenu::Reposition()
{
}

void QtPieMenu::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::Key_Right:
	case Qt::Key_Down:
	case Qt::Key_Tab:
		--_currentItemIndex;
		if (_currentItemIndex < 0)
			_currentItemIndex += _items.size();
		update();
		break;
	case Qt::Key_Left:
	case Qt::Key_Up:
	case Qt::Key_Backtab:
		++_currentItemIndex;
		if (_currentItemIndex >= char(_items.size()))
			_currentItemIndex -= _items.size();
		update();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
	case Qt::Key_Space:
		ActivateItem(KeyPress);
		if (_showingSubMenuIndex != -1) {
			((QtPieMenu *)ShownItemPtr())->SetHighlightedItem(0);
		}

		break;
	case Qt::Key_Escape:
	case Qt::Key_Backspace:
		{
			Q_EMIT aboutToHide();
			hide();
		}
		break;
	default:
		break;
	}
}

bool QtPieMenu::event(QEvent *e)
{
	if (e->type() == QEvent::TouchBegin)
	{
		//	this->log->appendPlainText(QString("touch  begin: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		QList<QTouchEvent::TouchPoint> touches = static_cast<QTouchEvent *>(e)->touchPoints();
		int count  = touches.count();

		if(count == 1)
		{
			int x = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().x();
			int y = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().y();
            
			_hoverTimer.stop();
			_lastMousePoint = QPoint(x, y);//mapFromGlobal(e->globalPos());
			_currentItemIndex = IndexAt(_lastMousePoint);
			if (_IsShaded &&  _showingSubMenuIndex != _currentItemIndex)
			{
				HideShowingSubMenu();
			}
			ActivateItem(MousePress);
			return true;
        }
	}

	if (e->type() == QEvent::TouchUpdate)
	{
		QList<QTouchEvent::TouchPoint> touches = static_cast<QTouchEvent *>(e)->touchPoints();
		int count  = touches.count();

		if(count == 1)
		{
			int x = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().x();
			int y = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().y();

			_hoverTimer.stop();
			_lastMousePoint = QPoint(x, y);//mapFromGlobal(e->globalPos());
			_currentItemIndex = IndexAt(_lastMousePoint);
			if (_IsShaded &&  _showingSubMenuIndex != _currentItemIndex)
			{
				HideShowingSubMenu();
			}
			ActivateItem(MousePress);
			return true;
		}
	}

	if (e->type() == QEvent::TouchEnd) {
		//	this->log->appendPlainText(QString("touchend: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		return true;
	}

	return QtPieItem::event(e);
}

//真正执行action, touch也一样
void QtPieMenu::mousePressEvent(QMouseEvent *e)
{
	e->accept();
	_hoverTimer.stop();

	_lastMousePoint = mapFromGlobal(e->globalPos());

	_currentItemIndex = IndexAt(_lastMousePoint);

	if (_IsShaded &&  _showingSubMenuIndex != _currentItemIndex)
	{
		HideShowingSubMenu();
	}
	ActivateItem(MousePress);
}

void QtPieMenu::mouseReleaseEvent(QMouseEvent *e)
{
	e->accept();
	_hoverTimer.stop();

	// Solve the case where a submenu is canceled by clicking inside
	// the cancel zone, and the next mouse release comes to us.
	if (_ignoreNextMouseReleaseIfNotDragging)
	{
		_ignoreNextMouseReleaseIfNotDragging = false;
	}

	_lastMousePoint = mapFromGlobal(e->globalPos());
	_hoverTimer.stop();

	if (!_IsShaded)
	{
		ActivateItem(MouseRelease);
	}
}

void QtPieMenu::mouseMoveEvent(QMouseEvent *e)
{
	e->accept();
	_lastMousePoint = mapFromGlobal(e->globalPos());
	double mouseRad = RadiusAt(_lastMousePoint);
	QtPieMenu* parentMenu = qobject_cast<QtPieMenu*>(parent());
	if (parentMenu && mouseRad > outerRadius)
	{
		// pos() will be wrong, but is not used
		parentMenu->mouseMoveEvent(e); 
		return;
	}

	// Detect changes in highlighting. The logics are as follows: If
	// the mouse pointer is outside the cancel zone and inside the
	// pie's outer radius, then highlight the section that the mouse
	// pointer is hovering over. The exception is that we also
	// highlight the section if the mouse pointer is outside the pie,
	// if a submenu is shown. In all other cases, no item is
	// highlighted.
	int prevHighlightedItem = _currentItemIndex;
	if (mouseRad >= _innerRadius && (_showingSubMenuIndex != -1 || (mouseRad <= outerRadius )))
	{
		_currentItemIndex = IndexAt(_lastMousePoint);
	}
	else
	{
		if (mouseRad < _innerRadius)
			_currentItemIndex = -1;
		else
			_currentItemIndex = -2;
	}

	// If highlight changes, order an update.
	if (prevHighlightedItem != _currentItemIndex)
	{
		Q_EMIT highlighted(_currentItemIndex);
		update();

		// And in this case, if we are in dragging mode, start the
		// timer so that in case the highlighted item is a submenu, it
		// will pop up in 0.25 seconds. If the mouse hovers over an
		// action item, any shown submenu is closed.
		_hoverTimer.setSingleShot(true);
		_hoverTimer.start(250);

	}

	// In dragging mode, always order an activation of a submenu if
	// the mouse has moved far enough away from the pie. This allows
	// more snappy response as we do not have to wait 250ms.
	if (HighlightedItemPtr() && HighlightedItemPtr()->GetType() == SubMenu && mouseRad > outerRadius)
	{
		_hoverTimer.stop();
		ActivateItem(Hovering);
	}
}

void QtPieMenu::showEvent(QShowEvent *e)
{
	Q_EMIT aboutToShow();

	//这里面有mask实现透明？不能直接透明画吗？
	if (!_hasMask)
	{
		// Create mask.
		QBitmap mask(_maskPixmap.size());
		mask.fill(Qt::color0);
		GenerateMask(&mask);
		_maskPixmap.setMask(mask);


		//这个是使窗口不规则用的
		//主要里面采用brush, 不brush不能刷出扇形，必须用mask
		setMask(mask);
		_hasMask = true;
	}

	// Determine what item in this menu should be highlighted from the
	// current mouse position. With quick gestures, the mouse cursor
	// may already have moved and stopped at a new item before we have
	// shown this submenu. We need to highlight the relevant item.
	_lastMousePoint = mapFromGlobal(QCursor::pos());
	double rad = RadiusAt(_lastMousePoint);
	if (rad >= _innerRadius && rad <= outerRadius)
	{
		_currentItemIndex = AngleToIndex(AngleAt(_lastMousePoint), _items.size());
	}
	else
	{
		if (rad < _innerRadius)
			_currentItemIndex = -1;
		else
			_currentItemIndex = -2;
	}

	QWidget::showEvent(e);
}

void QtPieMenu::hideEvent(QHideEvent *e)
{
	if (_lastActivatedItemIndex == -1)
		Q_EMIT canceled();
	_currentItemIndex = -2;
	HideShowingSubMenu();
	QWidget::hideEvent(e);
}

void QtPieMenu::HoverAlertTriggered()
{
	ActivateItem(Hovering);
}

void QtPieMenu::ActivateItem(int reason)
{
	// Ignore stuff that happens when the menu is not shown. This is
	// to avoid submenus from popping up after a parent menu has been
	// closed.
	if (!isVisible())
		return;

	// As a menu is activated, the timer must be stopped or we might
	// get two activations.
	_hoverTimer.stop();

	double mouseRad = RadiusAt(_lastMousePoint);
	bool isInCancelZone = mouseRad < _innerRadius;
	bool isOutsidePie = mouseRad > outerRadius;

	if (!(reason & KeyPress))
	{
		// A mouse release can never activate a menu when not in
		// dragging mode.
		if (reason & MouseRelease)
			return;

		// Cancel the menu if we got a MousePress in the cancel zone
		// or outside the pie. Any other activity in the cancel zone
		// when not in dragging mode is ignored.
		if (isInCancelZone || isOutsidePie)
		{
			if (reason & Hovering && _showingSubMenuIndex != -1)
			{
				// Hide shown submenu
				HideShowingSubMenu();
			}

			if (reason & MousePress)
			{
				if (!isInCancelZone)
					Q_EMIT canceledAll();
				Q_EMIT aboutToHide();
				hide();
			}

			return;
		}
	}

	// ### Investigate this special case further. It should not
	// ### happen.
	if (_currentItemIndex < 0)
		return;


	// Ignore all operations on disabled items.
	if (!_items.at(_currentItemIndex)->IsEnabled())
		return;

	int acthItem = _currentItemIndex;
	// Show new child
	if (HighlightedItemPtr()->GetType() == SubMenu)
	{
		// Avoid flickering
		if (acthItem == _showingSubMenuIndex)
			return;

		QtPieMenu *pie = (QtPieMenu *)HighlightedItemPtr();

		// Hide shown submenu
		HideShowingSubMenu();

		Q_EMIT activated(acthItem);

		_IsShaded = true;

		double angle;
		if (Count() == 1)
		{
			// If there is only one pie menu item, and the parent does
			// not inherit QtPieMenu (qualifies as the rool level pie
			// menu), then the submenu should open straight north. If
			// this is not the root menu however, the submenu should
			// open on the same axis that intersects both the parent's
			// center and this menu's center, at the point where it
			// intersects with this menu's outer radius.
			if (!parent()->inherits("QtPieMenu"))
			{
				angle = 0;
			}
			else
			{
				QtPieMenu *parentPie = (QtPieMenu *)parent();
				QPoint ppos = mapToGlobal(parentPie->pos());
				ppos.setX(ppos.x() + parentPie->width()/2);
				ppos.setY(ppos.y() + parentPie->height()/2);

				QPoint opos = mapToGlobal(pos());
				opos.setX(opos.x() + width()/2);
				opos.setY(opos.y() + height()/2);

				double xxdist = (double)(opos.x() - ppos.x());
				double yydist = (double)(opos.y() - ppos.y());
				double rrad = sqrt(xxdist * xxdist + yydist * yydist);
				angle = acos(xxdist / rrad);

				if (yydist > 0)
					angle = TWOPI - angle;
			}
		}
		else
		{
			double a = IndexToAngle(acthItem, Count(), true);
			double b = IndexToAngle(acthItem + 1, Count(), true);
			if (b > a)
				angle = (a + b) / 2;
			else
				angle = (a + b + TWOPI) / 2;
		}

		double b = ((double)(GetOuterRadius() + pie->GetOuterRadius())) * cos(angle) * 0.90;
		double c = - (double)(GetOuterRadius() + pie->GetOuterRadius()) * sin(angle) * 0.90;

		_hoverTimer.stop();

		// If hovering in non-dragging mode, show the submenu outside
		// the main menu.
		_showingSubMenuIndex = acthItem;
		pie->Popup(mapToGlobal(QPoint(rect().center().x() + (int) b,
			rect().center().y() + (int) c)));

		update();
	}
	else
	{
		if (_showingSubMenuIndex != -1)
		{
			// Hide shown submenu
			HideShowingSubMenu();
		}

		// Hovering never activates an action
		if (reason & Hovering)
			return;

		Q_EMIT activated(acthItem);

		QtPieAction *pie = (QtPieAction *)HighlightedItemPtr();
		pie->Activate();

		_lastActivatedItemIndex = acthItem;
		SubMenuSelectTriggered();
	}
}

void QtPieMenu::paintEvent(QPaintEvent *)
{
	QPainter p(&_maskPixmap);
	p.setRenderHint(QPainter::Antialiasing, true);
	QRect adjustedRect = _maskPixmap.rect().adjusted(0, 0, -1, -1);

	// Special case: 0 or 1 items
	if (_items.size() < 2)
	{
		if (_currentItemIndex == 0 && _items.at(0)->IsEnabled())
		{
			p.setBrush(palette().highlight());
		}
		else
		{
			p.setBrush(palette().background());
		}
		if (_IsShaded)
		{
			p.setBrush(QBrush(p.brush().color().dark(125)));
		}
		p.fillRect(rect(), p.brush());
	}

	// Draw the pie with borders and selected slice.
	if (_items.size() > 1)
	{
		for (int i = 0; i < _items.size(); ++i)
		{
			//if text is color value
			bool isColor = false;
			QColor color;
			QString text = _items.at(i)->GetText();
			if(text.startsWith("#") && text.length() == 7)
			{
				isColor = true;
				color = QColor(text);
				if(i == _currentItemIndex)
				{
					color = QColor(.3* color.red()   + .7 * 200,
						.3* color.green() + .7 * 200,
						.3* color.blue()  + .7 * 200,
						.3* color.alpha() + .7 * 200);
				}
			}

			if (isColor)
			{
				p.setBrush(QBrush(color));
			}
			else if (i == _currentItemIndex && _items.at(i)->IsEnabled())
			{
				p.setBrush(QBrush(palette().highlight()));
			}
			else
			{
				p.setBrush(QBrush(palette().background()));
			}

			p.setPen(palette().mid().color());
			if (_IsShaded)
			{
				p.setPen(QPen(p.pen().color().dark(125)));
				p.setBrush(QBrush(p.brush().color().dark(125)));
			}

			double a = IndexToAngle(i, _items.size(), true);
			double b = IndexToAngle(i + 1, _items.size(), true);

			if (b < a)
				b += TWOPI;
			double size = b - a;
			if (size < 0)
				size += TWOPI;
			int startAngle = (int) ((a * 360.0 * 16.0) / TWOPI);

			p.drawPie(adjustedRect, startAngle, (int) ((size * 360.0 * 16.0) / TWOPI));
		}
	}

	QColor background;
	QColor dark = palette().dark().color();
	QColor light = palette().light().color();
	if (_currentItemIndex == -1)
		background = palette().highlight().color();
	else
		background = palette().background().color();

	if (_IsShaded)
	{
		background = background.dark(125);
		dark = dark.dark(125);
		light = light.dark(125);
	}

	// Shading
	QRect innerRect(adjustedRect.center().x() - _innerRadius, adjustedRect.center().y() - _innerRadius,
		_innerRadius * 2 + 1, _innerRadius * 2 + 1);

	p.setPen(Qt::NoPen);
	p.setBrush(background);

	p.drawPie(innerRect, 0, 360 * 16);

	light.setAlpha(128);
	p.setPen(QPen(light, 1));
	p.setBrush(Qt::NoBrush);
	p.drawArc(innerRect, 225 * 16, 180 * 16);

	p.setPen(QPen(dark, 1));
	p.drawArc(innerRect, 45 * 16, 180 * 16);

	p.setPen(QPen(light, 1));
	p.setBrush(Qt::NoBrush);
	p.drawArc(adjustedRect, 45 * 16, 180 * 16);
	p.setPen(QPen(dark, 1));
	p.drawArc(adjustedRect, 225 * 16, 180 * 16);

	// Draw cancel zone
	if (_innerRadius > 0)
	{
		innerRect.setLeft(innerRect.left() + 1);
		innerRect.setRight(innerRect.right() - 1);
		innerRect.setTop(innerRect.top() + 1);
		innerRect.setBottom(innerRect.bottom() - 1);

		p.setPen(QPen(dark, 1));
		p.drawArc(innerRect, 225 * 16, 180 * 16);
		p.setPen(QPen(light, 1));
		p.drawArc(innerRect, 45 * 16, 180 * 16);
		p.setPen(QPen(dark, 1));

		p.setBrush(background);

		p.drawArc(innerRect, 225 * 16, 180 * 16);
		p.setPen(QPen(light, 1));
		p.drawArc(innerRect, 45 * 16, 180 * 16);
	}

	QRect r = innerRect;
	innerRect.setLeft(r.center().x() + ((r.left() - r.center().x()) / 3) * 1);
	innerRect.setRight(r.center().x() + ((r.right() - r.center().x()) / 3) * 1);
	innerRect.setTop(r.center().y() + ((r.top() - r.center().y()) / 3) * 1);
	innerRect.setBottom(r.center().y() + ((r.bottom() - r.center().y()) / 3) * 1);

	QColor text = _currentItemIndex == -1 ? palette().highlightedText().color() : palette().text().color();
	if (_IsShaded)
		text = text.dark(125);

	p.setPen(QPen(text, 2));
	p.drawLine(innerRect.topLeft(), innerRect.bottomRight());
	p.drawLine(innerRect.topRight(), innerRect.bottomLeft());

	// Distance from edge of pie to tip of arrow.
	int h1 = 2;

	// Height of arrow.
	int h2 = 6;

	// Width of arrow in radians.
	double w = 0.1;

	// Draw little arrows on the edge of the items that hide a
	// submenu.
	int i;
	for (i = 0; i < _items.size(); ++i)
	{
		if (!_items.at(i) || _items.at(i)->GetType() != SubMenu)
			continue;

		if (i == _currentItemIndex)
		{
			p.setBrush(palette().highlightedText());

			// Smooth edges of the arrow by drawing with a pen that
			// has the average color of the foreground and the
			// background.
			QColor c = palette().highlightedText().color();
			QColor d = palette().highlight().color();
			p.setPen(QColor((c.red() + d.red()) / 2,
				(c.green() + d.green()) / 2,
				(c.blue() + d.blue()) / 2));
		}
		else
		{
			p.setBrush(palette().text());

			QColor c = palette().text().color();
			QColor d = palette().background().color();
			p.setPen(QColor((c.red() + d.red()) / 2,
				(c.green() + d.green()) / 2,
				(c.blue() + d.blue()) / 2));
		}

		if (_IsShaded)
		{
			p.setPen(QPen(p.pen().color().dark(125)));
			p.setBrush(QBrush(p.brush().color().dark(125)));
		}

		double angle;
		if (Count() == 1)
		{
			// If there is only one pie menu item, and the parent does
			// not inherit QtPieMenu (qualifies as the rool level pie
			// menu), then the arrow should be drawn straight
			// north. If this is not the root menu however, the arrow
			// should be drawn on the same axis that intersects both
			// the parent's center and this menu's center, at the
			// point where it intersects with this menu's outer
			// radius.
			if (!parent()->inherits("QtPieMenu"))
			{
				angle = 0;
			}
			else
			{
				QtPieMenu *parentPie = (QtPieMenu *)parent();
				QPoint ppos = mapToGlobal(parentPie->pos());
				ppos.setX(ppos.x() + parentPie->width()/2);
				ppos.setY(ppos.y() + parentPie->height()/2);

				QPoint opos = mapToGlobal(pos());
				opos.setX(opos.x() + width()/2);
				opos.setY(opos.y() + height()/2);

				double xxdist = (double)(opos.x() - ppos.x());
				double yydist = (double)(opos.y() - ppos.y());
				double rrad = sqrt(xxdist * xxdist + yydist * yydist);
				angle = acos(xxdist / rrad);

				if (yydist > 0)
					angle = TWOPI - angle;
			}
		}
		else
		{
			double a = IndexToAngle(i, Count(), true);
			double b = IndexToAngle(i + 1, Count(), true);
			if (b > a)
				angle = (a + b) / 2;
			else
				angle = (a + b + TWOPI) / 2;
		}

		double trad = (double)(outerRadius - h1);
		double tb = trad * cos(angle);
		double tc = - trad * sin(angle);
		double lrad = (double)(outerRadius - h2);
		double lb = lrad * cos(angle + w);
		double lc = - lrad * sin(angle + w);
		double rrad = (double)(outerRadius - h2);
		double rb = rrad * cos(angle - w);
		double rc = - rrad * sin(angle - w);

		QPolygon ar(3);
		ar.setPoint(0, _maskPixmap.rect().center().x() + (int) tb, _maskPixmap.rect().center().y() + (int) tc);
		ar.setPoint(1, _maskPixmap.rect().center().x() + (int) rb, _maskPixmap.rect().center().y() + (int) rc);
		ar.setPoint(2, _maskPixmap.rect().center().x() + (int) lb, _maskPixmap.rect().center().y() + (int) lc);

		p.drawPolygon(ar);
	}

	// Draw the text and/or icon on each slice.
	for (i = 0; i < _items.size(); ++i)
	{
		QString text = _items.at(i)->GetText();

		double angle1 = IndexToAngle(i, _items.size(), true);
		double angle2 = IndexToAngle(i + 1, _items.size(), true);
		if (angle2 < angle1)
			angle2 += TWOPI;
		double angle = (angle1 + angle2) / 2.0;
		double rad = (double)(_innerRadius + outerRadius) / 2.0;
		double b = rad * cos(angle);
		double c = - rad * sin(angle);

		QFontMetrics fontMetrics = p.fontMetrics();
		QRect boundingRect;
		if (!text.isEmpty())
			boundingRect = fontMetrics.boundingRect(text);
		int textWidth = boundingRect.width();

		QPixmap icon = _items.at(i)->GetIcon().pixmap(QSize(32, 32), QIcon::Normal, QIcon::On);
		int x = _maskPixmap.rect().center().x();
		int y = _maskPixmap.rect().center().y();

		//现阶段，这个函数没用
		// Draw icon
		if (!_items.at(i)->GetIcon().isNull())
		{
			QRect r(x + int(b) - (icon.width() + textWidth) / 2,
				y + int(c) - icon.height() / 2,
				icon.width(), icon.height());

			if (_IsShaded)
			{
				// Only paint every second pixel.
				QBitmap bitmap(icon.width(), icon.height());
				QPainter mapper(&bitmap);
				int xorer = 0;
				for (int b = 0; b < icon.height(); ++b)
				{
					xorer = b & 1;
					for (int a = 0; a < icon.width(); ++a)
					{
						mapper.setPen(xorer++ & 1 ? Qt::color1 : Qt::color0);
						mapper.drawPoint(a, b);
					}
				}

				icon.setMask(bitmap);
			}

			p.drawPixmap(r, icon);
		}

		// Draw text
		if (!text.isEmpty())
		{
			if(text.startsWith("#") && text.length() == 7)
			{
				continue;
			}

			QBrush pen;
			QBrush brush;

			if (i == _currentItemIndex)
			{
				if (HighlightedItemPtr()->IsEnabled())
				{
					pen = palette().highlightedText();
					brush = palette().highlight();
				}
				else
				{
					pen = palette().mid();
					brush = palette().background();
				}
			}
			else
			{
				if (_items.at(i)->IsEnabled())
					pen = palette().text();
				else
					pen = palette().mid();

				brush = palette().background();
			}

			if (_IsShaded)
			{
				pen = QBrush(pen.color().dark(125));
				brush = QBrush(brush.color().dark(125));
			}

			// Draw text horizontally in center of slice.
			double angle1 = IndexToAngle(i, _items.size(), true);
			double angle2 = IndexToAngle(i + 1, _items.size(), true);
			if (angle2 < angle1)
				angle2 += TWOPI;
			double angle = (angle1 + angle2) / 2.0;

			double rad = (double)(_innerRadius + outerRadius) / 2.0;
			double b = rad * cos(angle);
			double c = - rad * sin(angle);

			// Draw rich text, always centered.
			QRect r(x + (int) b - (icon.width() + textWidth) / 2 + icon.width(),
				y + (int) c - boundingRect.height() / 2, textWidth, boundingRect.height());

			p.setPen(pen.color());
			p.setBrush(brush);
			p.drawText(r.left(), r.top() + boundingRect.height(), text);
		}
	}

	QPainter p2(this);
	p2.drawPixmap(QPoint(0, 0), _maskPixmap);
}

void QtPieMenu::SubMenuSelectTriggered()
{
	Q_EMIT activated();
	Q_EMIT aboutToHide();
	hide();
}

void QtPieMenu::SubMenuCancelTriggered()
{
	_showingSubMenuIndex = -1;

	// reset motion counter.
	_currentItemIndex = -2;

	// prevent delayed popup.
	_hoverTimer.stop();

	_IsShaded = false;

	// When a submenu is canceled, the following mouse release will
	// go to us.
	_ignoreNextMouseReleaseIfNotDragging = true;

	update();
}

void QtPieMenu::AllCancelTriggered()
{
	_showingSubMenuIndex = -1;

	_currentItemIndex = -2;

	// prevent delayed popup.
	_hoverTimer.stop();

	_IsShaded = false;

	_ignoreNextMouseReleaseIfNotDragging = true;

	Q_EMIT aboutToHide();
	hide();
	Q_EMIT canceledAll();

	update();
}

double QtPieMenu::IndexToAngle(int item, int total, bool shifted)
{
	// Wrap index.
	item %= total;

	// Sum up all the weights, and find the base angle, or the angle
	// of an item of weight 1.
	double totalWeight = 0;
	if (_items.size() != 0)
	{
		for (int i = 0; i < _items.size(); ++i)
			totalWeight += _items.at(i)->GetWeight();
	}
	else
	{
		totalWeight = total;
	}

	double baseAngle = TWOPI / (double) totalWeight;

	// Find the start angle of the first item.
	double rad = PI/2;

	if (shifted)
	{
		if (!_items.isEmpty())
			rad -= ((double)_items.at(0)->GetWeight() * baseAngle) / 2.0;
		else
			rad -= baseAngle / 2.0;
	}
	while (rad < 0)
		rad += TWOPI;

	// Add the base angle multiplied by the items' weights until we
	// reach the item we're queried for.
	for (int index = 0; index < item; ++index)
	{
		if (index >= _items.size())
			rad += baseAngle;
		else
			rad += double(_items.at(index)->GetWeight()) * baseAngle;
	}

	// Adjust accordingly.
	while (rad > TWOPI)
		rad -= TWOPI;

	return rad;
}

int QtPieMenu::AngleToIndex(double angle, int total)
{
	if (total == 0)
		return -1;

	// Sum up all the weights, and find the base angle, or the angle
	// of an item of weight 1.
	double totalWeight = 0;
	for (int i = 0; i < _items.size(); ++i)
		totalWeight += _items.at(i)->GetWeight();
	double baseAngle = TWOPI / (double) totalWeight;

	// Find the start angle of the first item.
	double rad = PI/2;

	if (!_items.isEmpty())
		rad -= (double(_items.at(0)->GetWeight()) * baseAngle) / 2.0;
	while (rad < 0) rad += TWOPI;

	// Add the base angle multiplied by the items' weights until we
	// reach the item we're queried for.
	double oldrad = rad;
	for (int index = 0; index < _items.size(); ++index)
	{
		rad += double(_items.at(index)->GetWeight()) * baseAngle;
		if (angle > oldrad && angle <= rad)
		{
			return index;
		}
		else if (angle+TWOPI > oldrad && angle+TWOPI <= rad)
		{
			return index;
		}

		oldrad = rad;
	}

	return -1;
}

int QtPieMenu::IndexAt(const QPoint &pos)
{
	return AngleToIndex(AngleAt(pos), _items.size());
}

double QtPieMenu::RadiusAt(const QPoint &pos) const
{
	double mousexdist = pos.x() - rect().center().x();
	double mouseydist = pos.y() - rect().center().y();

	return sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
}

double QtPieMenu::AngleAt(const QPoint &pos) const
{
	double mousexdist = pos.x() - rect().center().x();
	double mouseydist = pos.y() - rect().center().y();
	double mouserad = sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
	double angle = acos(mousexdist / mouserad);
	if (mouseydist >= 0)
		angle = TWOPI - angle;

	return angle;
}

void QtPieMenu::HideShowingSubMenu()
{
	if (_showingSubMenuIndex != -1)
	{
		QtPieMenu *menu = static_cast<QtPieMenu *>(_items[_showingSubMenuIndex]);
		if (!menu)
		{
			// ### Dead code
			return;
		}

		Q_EMIT menu->aboutToHide();
		menu->hide();

		_showingSubMenuIndex = -1;
		_IsShaded = false;
		update();
	}
}

int QtPieMenu::HighlightedItem()
{
	return _currentItemIndex;
}

void QtPieMenu::DeleteItems(int index)
{
	if (index >= 0)
	{
		delete _items[index];
		_items.removeAt(index);
	}
	else
	{
		for (int i=0; i<_items.size(); ++i)
			delete _items[i];
		_items.clear();
	}
}

QtPieItem *QtPieMenu::HighlightedItemPtr()
{
	if (_currentItemIndex < 0)
		return 0;
	else
		return _items[_currentItemIndex];
}

QtPieItem *QtPieMenu::ShownItemPtr()
{
	if (_showingSubMenuIndex == -1)
		return 0;
	else
		return _items[_showingSubMenuIndex];
}

bool QtPieMenu::IsShaded() const
{
	return _IsShaded;
}

QtPieItem::ItemType QtPieMenu::GetItemType(int index) const
{
	if (!ItemAt(index))
		return Invalid;
	return ItemAt(index)->GetType();
}
