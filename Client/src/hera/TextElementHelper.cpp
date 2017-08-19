#include "TextElementHelper.h"
#include "TextElement.h"
#include "Element.h"
#include "SizeAction.h"
#include "TextChangedAction.h"
#include "BanBaseWidget.h"
#include "NotifyWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TextElementHelper::TextElementHelper(TextElement* textElement, QWidget *parent):
QTextEdit(parent),
_titleBar(new ElementResizableTitlebar(parent)),
_textElement(textElement)
{
	//setAttribute(Qt::WA_DeleteOnClose, true);
	setLineWrapMode(QTextEdit::NoWrap);
	setAcceptRichText(false);
	setFocusPolicy(Qt::ClickFocus);
	//setFrameStyle(QFrame::StyledPanel);
	setAttribute(Qt::WA_Hover, true);

	int ctrlc = grabShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	int ctrlv = grabShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	int ctrlz = grabShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	int ctrly = grabShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));

	//不起作用
	setShortcutEnabled(ctrlc, false);
	//setShortcutEnabled(ctrlv, false);
	//setShortcutEnabled(ctrlz, false);
	//setShortcutEnabled(ctrly, false);

	//许多东西要设置在viewport()上面，需要总结
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//setAutoFillBackground(false);
	//setAttribute(Qt::WA_ContentsPropagated);
	//setAttribute(Qt::WA_OpaquePaintEvent);
	//setAttribute(Qt::WA_NoSystemBackground);
	viewport()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	viewport()->setAutoFillBackground(false);

	BanBaseWidget* base = dynamic_cast<BanBaseWidget*>(parent);
	if(base != NULL)
	{
		_scBase = base;
		connect(this, &TextElementHelper::textChanged, this, &TextElementHelper::OnTextChanged);
		connect(this, &TextElementHelper::OnAnimationAction, _scBase, &BanBaseWidget::OnAnimationAction);
		connect(this, &TextElementHelper::OnDeleteElement, _scBase, &BanBaseWidget::RemoveDrawingElementTriggered);
		connect(this, &TextElementHelper::OnTopMostElement, _scBase, &BanBaseWidget::OnTopMostElementTriggered);
		connect(this, &TextElementHelper::OnBottomMostElement, _scBase, &BanBaseWidget::OnBottomMostElementTriggered);
		connect(this, SIGNAL(OnDirty()), _scBase, SLOT(SetModifiedTriggered()));
		//this->installEventFilter(_scBase);
	}

	_titleBar->GetDeleteButton()->show();
	_titleBar->GetTopMostButton()->show();
	_titleBar->GetBottomMostButton()->show();
	connect(_titleBar->GetDeleteButton(), &QPushButton::clicked, this, &TextElementHelper::OnDeleteTriggered);
	connect(_titleBar->GetTopMostButton(), &QPushButton::clicked, this, &TextElementHelper::OnTopMostTriggered);
	connect(_titleBar->GetBottomMostButton(), &QPushButton::clicked, this, &TextElementHelper::OnBottomMostTriggered);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QPoint leftTop = _textElement->GetActualLeftTop();
	move(leftTop.x() - HIGHTITEM_BORDER, leftTop.y() - HIGHTITEM_BORDER);
	//move(_textElement->_leftTop._x, _textElement->_leftTop._y);
	setPlainText("");
	SetFontParameters(true);
	setPlainText(QString(_textElement->_text.c_str()));
	SetFontParameters(true);
	setFocus();
	//document()->setDefaultTextOption(QTextOption(Qt::AlignTop | Qt::AlignLeft));

	QTextFrame *mainFrame = textCursor().currentFrame();
	QTextFrameFormat frameFormat;
	frameFormat.setMargin(0);
	frameFormat.setTopMargin(0);
	frameFormat.setPadding(0);
	frameFormat.setBorder(0);
	mainFrame->setFrameFormat(frameFormat);
	setTabStopWidth(TEXTELEMENT_TABSTOP);
}

TextElementHelper::~TextElementHelper()
{
	if(_titleBar != NULL)
	{
		_titleBar->deleteLater();
		_titleBar = NULL;
	}
}

void TextElementHelper::OnDeleteTriggered()
{
	_textElement->SetCanDelete(false);
	Q_EMIT OnDeleteElement(_textElement);
}

void TextElementHelper::OnTopMostTriggered()
{
	Q_EMIT OnTopMostElement(_textElement);
}

void TextElementHelper::OnBottomMostTriggered()
{
	Q_EMIT OnBottomMostElement(_textElement);
}

void TextElementHelper::SetFontParameters(bool needEmit)
{
	QColor color = _textElement->GetLineColor();
	//windows上全用宋体，android,ios上，不支持文字输入
	QString style = "TextElementHelper {line-height:0px; margin-top:0px; padding-top:0px; color:"+ color.name()+ "; font-size:" + QString::number(_textElement->_fontSize) + "pt; font-family:" + tr("宋体}");
	//QString style = "TextElementHelper {line-height:0px; margin-top:0px; padding-top:0px; color:"+ color.name()+ "; font-size:" + QString::number(_textElement->_fontSize) + "pt; font-family:" + tr("DroidSansFallback}");
	QString text = toPlainText();
	bool hasNoneEnglish = FontUtil::HasNoneEnglish(text);
	//setFont(FontUtil::GetTextFontBySize(_textElement->_fontSize, hasNoneEnglish));
	setStyleSheet(style);
	if(needEmit)
	{
		Q_EMIT textChanged();

		TextChangedAction action;
		action.SetTartgetBanUuid(_scBase->GetObjectUuid().toStdString());
		action.SetTartgetElementUuid(_textElement->GetObjectUuid());
		action.Update(toPlainText(), _textElement->_fontSize);
		Q_EMIT OnAnimationAction(action, _textElement);
	}
}

void TextElementHelper::contextMenuEvent(QContextMenuEvent *e)
{
	e->ignore();
	//QPlainTextEdit::contextMenuEvent(e);
}

void TextElementHelper::keyPressEvent(QKeyEvent *e)
{
	if(e->modifiers() & Qt::ControlModifier)
	{
		if(e->key() == Qt::Key_C ||
			e->key() == Qt::Key_V ||
			e->key() == Qt::Key_Z ||
			e->key() == Qt::Key_Y)
		{
			if(false)
			{
				e->ignore();
				return;
			}
		}
	}

	return QTextEdit::keyPressEvent(e);
}

void TextElementHelper::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
		qApp->setOverrideCursor(Qt::SizeAllCursor);
		//setMouseTracking(true);
	}
	return QTextEdit::mousePressEvent(event);
}

void TextElementHelper::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint moveto = event->globalPos() - _dragPosition;
		move(moveto);
		event->accept();
	}
}

void TextElementHelper::mouseReleaseEvent(QMouseEvent *e)
{
	qApp->restoreOverrideCursor();
}

void TextElementHelper::resizeEvent(QResizeEvent *e)
{
	setContentsMargins(0, 0, 0, 0);
	setViewportMargins(0, 0, 0, 0);
	SendSizeAnimation();
	QTextEdit::resizeEvent(e);
}

void TextElementHelper::moveEvent(QMoveEvent *e)
{
	QPoint point(pos().x(), pos().y() - 30);
	_titleBar->move(point);
	SendSizeAnimation();
	QTextEdit::moveEvent(e);
}

void TextElementHelper::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;

	if(numSteps > 0)
	{
		_textElement->_fontSize++;
	}
	else if(numSteps < 0)
	{
		if(_textElement->_fontSize >= 2)
		{
			_textElement->_fontSize--;
		}
	}

	QString msg = tr("字体大小:") + QString::number(_textElement->_fontSize);
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(msg);

	SetFontParameters();
	e->accept();
}

//目前，好像，就text的color有用
void TextElementHelper::showEvent(QShowEvent * e)
{
	resize(document()->size().width() + 5, document()->size().height() + 5);
	setFocus();
	_titleBar->show();
	SetFontParameters(false);
	QTextEdit::showEvent(e);
}

void TextElementHelper::hideEvent(QHideEvent *e)
{
	_titleBar->hide();
	QTextEdit::hideEvent(e);

	_textElement->_text = toPlainText().toStdString();
	if(toPlainText().isEmpty() && _textElement->CanDelete())
	{
		OnDeleteTriggered();
	}
}

void TextElementHelper::OnTextChanged()
{
	Q_EMIT OnDirty();
	resize(document()->size().width() + 5, document()->size().height() + 5);
	if(toPlainText().isEmpty())
	{
		//OnDeleteTriggered();
	}

	TextChangedAction action;
	action.SetTartgetBanUuid(_scBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(_textElement->GetObjectUuid());
	action.Update(toPlainText(), _textElement->_fontSize);
	Q_EMIT OnAnimationAction(action, _textElement);
}

void TextElementHelper::UpdateSize()
{
	int x1 = frameGeometry().topLeft().x();
	int y1 = frameGeometry().topLeft().y();
	QTextBlock text_block = document()->begin();
	QRectF r1 = text_block.layout()->boundingRect();
	QPoint p1 = r1.topLeft().toPoint();
	QAbstractTextDocumentLayout *layout = document()->documentLayout();
	QRectF r2 = layout->blockBoundingRect(text_block);
	QPoint p2 = r2.topLeft().toPoint();
	QTextFrame *root = document()->rootFrame();

	int topLeftX = mapToParent(rect().topLeft()).x();
	int topLeftY = mapToParent(rect().topLeft()).y();

	QMatrix matrix = _textElement->GetMatrix().ToQMatrix();
	QPoint mapPoint1 = matrix.inverted().map(QPoint(topLeftX, topLeftY));
//	_textElement->_leftTop = SPoint(mapPoint1);

	//就是算不对，手动调整吧
	int cTopLeftX = geometry().topLeft().x() + HIGHTITEM_BORDER;
	int cTopLeftY = geometry().topLeft().y() + HIGHTITEM_BORDER;

	//int cTopLeftX = mapToParent(viewport()->rect().topLeft()).x() + HIGHTITEM_BORDER;
	//int cTopLeftY = mapToParent(viewport()->rect().topLeft()).y() + HIGHTITEM_BORDER;
	QPoint mapPoint2 = matrix.inverted().map(QPoint(cTopLeftX, cTopLeftY));
	_textElement->_leftTop = SPoint(mapPoint2);

	//从5.3开始，这个width算错了，现在就需要不用width和height了，还是自己算合适
	int width = rect().width();
	int height = rect().height();
	//虽然没用，但也保存
	_textElement->_width = width;
	_textElement->_height = height;
	
	//如果是用helper作用过的话，需要clear matrix
	QMatrix temp_matrix;
	_textElement->SetMatrix(temp_matrix);
}

void TextElementHelper::SendSizeAnimation()
{
	UpdateSize();
	Q_EMIT OnDirty();
	SizeAction action;
	action.SetTartgetBanUuid(_scBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(_textElement->GetObjectUuid());

	int cTopLeftX = geometry().topLeft().x() + HIGHTITEM_BORDER;
	int cTopLeftY = geometry().topLeft().y() + HIGHTITEM_BORDER;
	QMatrix matrix = _textElement->GetMatrix().ToQMatrix();
	QPoint mapPoint2 = matrix.inverted().map(QPoint(cTopLeftX, cTopLeftY));
	action.Update(mapPoint2, rect().width(), rect().height());
	Q_EMIT OnAnimationAction(action, _textElement);
}