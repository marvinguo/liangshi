#include "BanBaseWidget.h"
#ifdef _LiangshiDesktop
	#include "NotMobile/BanScriptRuntime.h"
#endif
#include "DrawElement.h"
#include "OrigialDrawElement.h"
#include "LineElement.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "RectElement.h"
#include "EllipseElement.h"
#include "ImageElement.h"
#include "CompressedImageElement.h"
#include "BanElement.h"
#include "BanStateElement.h"
#include "ImageStateElement.h"

#include "HideAction.h"
#include "ShowAction.h"
#include "LineStyleAction.h"
#include "LineWidthAction.h"
#include "LineColorAction.h"
#include "TopMostAction.h"
#include "BottomMostAction.h"

#include "../common/library.h"
#include "../common/Util.h"
#include "../common/FileUtil.h"
#include "BanFile.h"
#include "NotifyWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanBaseWidget::BanBaseWidget(bool embedded, QWidget *parent):
QWidget(parent),
_reviseDraw(false),
_embedded(embedded),
_dirty(false),
_disablePaste(false),
_disableDraw(false),
_disableHighlight(false),
_disableDrop(false),
_isDrawing(false),
_redoing(false),
_needSaveScript(true),
_canEditInside(true),
_emitAdditionAction(true),
_lineWidth(2),
_lineStyle(Qt::SolidLine),
_lineColor(QColor(Qt::white).rgba()),
_lineQColor(Qt::white),
_uuid(QUuid::createUuid()),
_highlightElement(NULL),
_layer(0)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	setAttribute(Qt::WA_ContentsPropagated);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);

#ifdef _LiangshiDesktop
	InitShortCutKey();
#endif

	setFocus();
	//我想是，只能有这一个MouseTracking
	setMouseTracking(true);
	//installEventFilter(this);
	SetMode(DrawMode_Draw);
}

BanBaseWidget::~BanBaseWidget()
{
	_drawingElements.clear();

	for(auto i = _allElements.begin(); i != _allElements.end(); i++)
	{
		delete *i;
	}
}

void BanBaseWidget::OnInsertBanTriggered(QString filepath)
{
	vector<BanElement*> elements  = BanFile::LoadAsScElements(filepath);

	for(vector<BanElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		//需要调整top left
		(*i)->_leftTop = GetCursorPoint();
		AddElement(*i);
	}
}

LokiBuffer BanBaseWidget::GetMsgPackData(bool init)
{
	DeHighlightElement();

	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	packer.pack(_uuid.toString().toStdString());

	msgpack::sbuffer thisBuffer;
	msgpack::packer<msgpack::sbuffer> packerThis(&thisBuffer);
	packerThis.pack(*this);
	packer.pack_raw(thisBuffer.size());
	packer.pack_raw_body(thisBuffer.data(), thisBuffer.size());

	if(init)
	{
		for(auto i = _allElements.begin(); i != _allElements.end(); i++)
		{
			Element* element = *i;
			element->ToInitState();
		}

		for(auto i = _allElements.begin(); i != _allElements.end(); i++)
		{
			Element* element = *i;
			LokiBuffer elementData = element->GetMsgPackData();

			packer.pack((int)element->GetType());
			packer.pack_raw(elementData.Size());
			packer.pack_raw_body(elementData.Data(), elementData.Size());
		}

		for(auto i = _allElements.begin(); i != _allElements.end(); i++)
		{
			Element* element = *i;
			element->ToCurrentState();
		}
	}
	else
	{
		for(auto i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			Element* element = *i;
			LokiBuffer elementData = element->GetMsgPackData();

			packer.pack((int)element->GetType());
			packer.pack_raw(elementData.Size());
			packer.pack_raw_body(elementData.Data(), elementData.Size());
		}
	}

	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void BanBaseWidget::SetMsgPackData(LokiBuffer& buffer)
{
	ClearAll();

	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	bool getUuid = false;
	bool getThis = false;
	int type = 0;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::POSITIVE_INTEGER)
		{
			object.convert(&type);
		}
		if(object.type == msgpack::type::BOOLEAN)
		{
			//object.convert(&_loadDrawing);
		}
		if(object.type == msgpack::type::RAW)
		{
			if(!getUuid && type == 0)
			{
				string uuid;
				object.convert(&uuid);
				_uuid = QUuid(QString(uuid.c_str()));
				getUuid = true;
			}
			else if(!getThis && type == 0)
			{
				msgpack::unpacked msg;
				msgpack::unpack(&msg, object.via.raw.ptr, object.via.raw.size);
				msg.get().convert(this);
				getThis = true;
			}
			else
			{
				if(type != 0)
				{
					LokiBuffer rawLokiBuffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
					Element* element = Element::CreateFromType((ElementType)type, rawLokiBuffer);

					//必须检测NULL,未来版本有不确定的东西创建，低版本中打开未来版本，一方面会有提示，另一方面，
					//也必须跳过不支持的element
					if(element != NULL)
					{
						//2014. hack to init ImageStateElement And BanStateElement
						if(element->GetType() == ElementType_BanState)
						{
							BanStateElement* banStateElement = dynamic_cast<BanStateElement*>(element);
							string uuid = banStateElement->_banElementUuid;
							BanElement* findElement = dynamic_cast<BanElement*>(FindElementByUuid(uuid));
							banStateElement->_banElement = findElement;
							findElement->SetCanPaint(false);
							findElement->SetCanPreview(false);
						}

						if(element->GetType() == ElementType_ImageState)
						{
							ImageStateElement* imageStateElement = dynamic_cast<ImageStateElement*>(element);
							string uuid = imageStateElement->_imageElementUuid;
							ImageBaseElement* findElement = dynamic_cast<ImageBaseElement*>(FindElementByUuid(uuid));
							imageStateElement->_imageElement = findElement;
							findElement->SetCanPaint(false);
							findElement->SetCanPreview(false);
						}

						//FindElementByUuid
						_allElements.insert(element);
						_drawingElements.push_back(element);
					}
				}
			}
		}
	}
	_cachePixmap = _backgroundImage.GetImage();
}

void BanBaseWidget::UpdateDataFromMsgPack()
{
	Repaint();
}

QString BanBaseWidget::GetSaveFileDialongExtension()
{
	return tr("板书文件(*.ban)");
}

void BanBaseWidget::UpdateFilepath(QString filepath)
{
	_filepath = filepath;
	QString title = FileUtil::GetFilename(_filepath, _filepath);
	Q_EMIT OnSaveFileNameChanged(title, false);
}

void BanBaseWidget::SetModifiedTriggered(bool modified)
{
	_dirty = modified;
	QString title;
	title = FileUtil::GetFilename(_filepath, _filepath);
	Q_EMIT OnSaveFileNameChanged(title, modified);
}

bool BanBaseWidget::HasSavedPath()
{
	QFileInfo info(_filepath);
	return info.exists();
}

bool BanBaseWidget::SaveDocument(QString filepath)
{
	if(filepath == "")
		filepath = _filepath;

	BanFile file(this);
	file.SaveFile(filepath);
	_filepath = filepath;
	_dirty = false;
	UpdateFilepath(_filepath);
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("保存成功"));
	return true;
}

bool BanBaseWidget::SaveAsDocument(QString filepath)
{
	//这里需要改变uuid;
	QUuid oldUuid = _uuid;
	_uuid = QUuid::createUuid();
	BanFile file(this);
	file.SaveFile(filepath);
	_uuid = oldUuid;
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("另存为成功"));
	return true;
}

void BanBaseWidget::SetLayer(int layer)
{
	_layer = layer;
	if(layer > 0)
	{
		_filepath = tr("第") + QString::number(layer) + tr("层板书");
	}
	UpdateFilepath(_filepath);
}

void BanBaseWidget::InitShortCutKey()
{
	QShortcut *ctrlC = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
	connect(ctrlC, &QShortcut::activated, this, &BanBaseWidget::OnCopyTriggered);
	QShortcut *ctrlV = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
	connect(ctrlV, &QShortcut::activated, this, &BanBaseWidget::OnPasteTriggered);
	QShortcut *ctrlZ = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
	connect(ctrlZ, &QShortcut::activated, this, &BanBaseWidget::OnUndoTriggered);
	QShortcut *ctrlY = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);
	connect(ctrlY, &QShortcut::activated, this, &BanBaseWidget::OnRedoTriggered);
}

QPoint BanBaseWidget::GetCursorPoint()
{
	QPoint pos1 = QCursor::pos();
	QPoint pos2 = mapFromGlobal(pos1);
	QPoint pos3 = mapTo(this, pos2);

	if(pos3.x() <= 0)
		pos3.setX(10);

	if(pos3.y() <= 0)
		pos3.setY(10);

	if(pos3.x() >= width())
		pos3.setX(10);

	if(pos3.y() >= height())
		pos3.setY(10);

	return pos3;
}

void BanBaseWidget::OnCopyTriggered()
{
	QClipboard* clipboard = QApplication::clipboard();

	if(_highlightElement != NULL)
	{
		_highlightElement->Highlight(this, false);
		LokiBuffer buffer = _highlightElement->GetCopyableData();
		QByteArray data(buffer.Data(), buffer.Size());

		QMimeData* mimeData = new QMimeData();
		mimeData->setData("collect/element", data);
		clipboard->setMimeData(mimeData);
		_highlightElement->Highlight(this);

		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("已经复制"));
	}
}

void BanBaseWidget::OnPasteTriggered()
{
	if(_disablePaste)
		return;
	const QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();

	bool pasted = false;

	//纯文本，但不是文件路径
	if(mimeData->hasText() && !mimeData->hasUrls())
	{
		Element * element;
		QString text = mimeData->text();
		if(Util::IsMathML(text))
		{
			element = new HtmlTextElement(GetCursorPoint(), text);
		}
		else
		{
			element = new TextElement(GetCursorPoint(), text);
		}
		AddElement(element);
		pasted = true;
	}	

	if(mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); ++i)
		{
			QUrl url = *i;
			QPixmap pixmap;
			if(pixmap.load(url.toLocalFile()))
			{
				if(pixmap.hasAlpha())
				{
					ImageElement* element = new ImageElement(GetCursorPoint(), pixmap);
					AddElement(element);
				}
				else
				{
					CompressedImageElement* element = new CompressedImageElement(GetCursorPoint(), pixmap);
					AddElement(element);
				}
				pasted = true;
			}
			else if(mimeData->hasFormat("text/plain"))
			{
			}
		}
	}

	if(mimeData->hasImage())
	{
		QImage imageData = qvariant_cast<QImage>(mimeData->imageData());
		QPixmap pixmap = QPixmap::fromImage(imageData);
		if(pixmap.hasAlpha())
		{
			ImageElement* element = new ImageElement(GetCursorPoint(), pixmap);
			AddElement(element);
		}
		else
		{
			CompressedImageElement* element = new CompressedImageElement(GetCursorPoint(), pixmap);
			AddElement(element);
		}
		pasted = true;
	}

	//own format
	if(mimeData->hasFormat("collect/element"))
	{
		QByteArray data = mimeData->data("collect/element");
		LokiBuffer buffer(data.data(), data.size());
		Element* element = Element::CreateFromCopyableData(buffer);
		if(element != NULL)
		{
			//如果是复制，需要新的uuid,以免在动画里出问题
			element->_objUuid = QUuid::createUuid().toString().toStdString();
			element->UpdateSize(GetCursorPoint());
			AddElement(element);
			pasted = true;
		}
	}

	if(pasted)
	{
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("粘贴成功"));
	}
}

void BanBaseWidget::OnUndoTriggered()
{
	if(_disableDraw)
		return;

	if(_drawingElements.size() > 0)
	{
		RemoveDrawingElementTriggered(*_drawingElements.rbegin());
	}
}

void BanBaseWidget::OnRedoTriggered()
{
	if(_disableDraw)
		return;

	if(!_undos.empty())
	{
		//ref http://stackoverflow.com/questions/2678175/iterator-to-last-element-in-stdlist
		Element* element = *(--_undos.end());
		_undos.pop_back();
		_redoing = true;
		AddElement(element, false);
	}
}

void BanBaseWidget::OnInsertPixmapTriggered(QPixmap& pixmap)
{
	if(pixmap.hasAlpha())
	{
		ImageElement* element = new ImageElement(GetCursorPoint(), pixmap);
		AddElement(element);
	}
	else
	{
		CompressedImageElement* element = new CompressedImageElement(GetCursorPoint(), pixmap);
		AddElement(element);
	}
}

void BanBaseWidget::OnInsertPixmapBackgroundTriggered(QPixmap& pixmap)
{
	_cachePixmap = pixmap.copy();
	_backgroundImage.SetImage(_cachePixmap);
	Repaint();
}

void BanBaseWidget::ClearAll()
{	
	for(set<Element*>::iterator i = _allElements.begin(); i != _allElements.end(); i++)
	{
		delete *i;
	}

	SetHighlightElement(NULL);
	_allElements.clear();
	_drawingElements.clear();
	_undos.clear();
}

void BanBaseWidget::ResetElementToInit()
{
	SetHighlightElement(NULL);
	for(set<Element*>::iterator i = _allElements.begin(); i != _allElements.end(); i++)
	{
		Element* element = *i;
		element->ToInitState();
	}
	_drawingElements.clear();
}

void BanBaseWidget::AddElement(Element* element, bool updateProperty, bool highlight, bool addToDrawing)
{
	DeHighlightElement(false);

	//fix this later
	BanElement* scElement = dynamic_cast<BanElement*>(element);
	if(scElement == NULL && updateProperty)
	{
		TextElement* text = dynamic_cast<TextElement*>(element);
		HtmlTextElement* html = dynamic_cast<HtmlTextElement*>(element);

		if(text == NULL && html == NULL)
		{
			element->SetLineWidth(_lineWidth);
		}
		element->SetLineStyle(_lineStyle);
		element->SetLineColor(_lineQColor);
	}
	element->SetMatrix(_elementMatrix);
	element->SaveInitState();

	if(highlight)
	{
		element->Highlight(this);
		SetHighlightElement(element);
	}

	_allElements.insert(element);
	if(addToDrawing)
	{
		_drawingElements.push_back(element);
		Repaint();
		//other problems
		if(!_redoing)
		{
			_undos.clear();
		}
		else
		{
			_redoing = false;
		}

		SetModifiedTriggered();

		//if(emitAction)
		{
			ShowAction showAction;
			showAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
			showAction.SetTartgetElementUuid(element->GetObjectUuid());
			Q_EMIT OnAnimationAction(showAction, element);

			if(scElement == NULL && updateProperty && _emitAdditionAction)
			{
				LineWidthAction widthAction;
				widthAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
				widthAction.SetTartgetElementUuid(element->GetObjectUuid());
				widthAction.Update(_lineWidth);
				Q_EMIT OnAnimationAction(widthAction, element);

				LineStyleAction styleAction;
				styleAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
				styleAction.SetTartgetElementUuid(element->GetObjectUuid());
				styleAction.Update(_lineStyle);
				Q_EMIT OnAnimationAction(styleAction, element);

				LineColorAction colorAction;
				colorAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
				colorAction.SetTartgetElementUuid(element->GetObjectUuid());
				colorAction.Update(_lineQColor);
				Q_EMIT OnAnimationAction(colorAction, element);
			}
		}
	}
}

Element* BanBaseWidget::FindElementByUuid(string uuid)
{	
	Element* element = NULL;
	for(set<Element*>::iterator i = _allElements.begin(); i != _allElements.end(); ++i)
	{
		Element* e = *i;
		if(e->GetObjectUuid() == uuid)
		{
			element = e;
			break;
		}
	}
	return element;
}

void BanBaseWidget::RemoveDrawingElementTriggered(Element* element)
{
	DeHighlightElement();

	list<Element*>::iterator i = std::find(_drawingElements.begin(), _drawingElements.end(), element);
	if(i != _drawingElements.end())
	{
		_drawingElements.erase(i);
		_undos.push_back(element);
	}

	SetModifiedTriggered();
	HideAction action;
	action.SetTartgetBanUuid(GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(element->GetObjectUuid());
	Q_EMIT OnAnimationAction(action, element);
	Repaint();
}

void BanBaseWidget::OnTopMostElementTriggered(Element* element)
{
	//到最上面，是到尾巴
	list<Element*>::iterator i = std::find(_drawingElements.begin(), _drawingElements.end(), element);
	if(i != _drawingElements.end())
	{
		_drawingElements.erase(i);
		_drawingElements.push_back(element);
	}

	SetModifiedTriggered();
	TopMostAction action;
	action.SetTartgetBanUuid(GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(element->GetObjectUuid());
	Q_EMIT OnAnimationAction(action, element);
	Repaint();
}

void BanBaseWidget::OnBottomMostElementTriggered(Element* element)
{
	//到最下面，是到头
	list<Element*>::iterator i = std::find(_drawingElements.begin(), _drawingElements.end(), element);
	if(i != _drawingElements.end())
	{
		_drawingElements.erase(i);
		_drawingElements.push_front(element);
	}

	SetModifiedTriggered();
	BottomMostAction action;
	action.SetTartgetBanUuid(GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(element->GetObjectUuid());
	Q_EMIT OnAnimationAction(action, element);
	Repaint();
}

void BanBaseWidget::OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete)
{
	DeHighlightElement();
	Element* relatedElement = (Element*)relatedData.value<void*>();
	list<Element*>::iterator i = std::find(_drawingElements.begin(), _drawingElements.end(), relatedElement);
	if(i != _drawingElements.end())
	{
		BanElement* e = dynamic_cast<BanElement*>(relatedElement);
		if(e != NULL)
		{
			//相当于是重新new 一个element出来，lefttop, width, height, _sc都相于要改动
			//外面的一些参数也需要，ScElement已经破坏，看看怎么办
			//_leftTopInSc
			if(buffer.Size() > 0)
			{
				e->UpdateFromBuffer(buffer);

				//还点检测是否为空，如果为空，就可以删除了
				if(e->IsEmptyInside() && canDelete)
				{
					RemoveDrawingElementTriggered(e);
				}
				else
				{
					//这个应该用别的方法
					//e->Highlight(this);
					//_highlightElement = e;
					SetHighlightElement(NULL);
					SetModifiedTriggered();
				}
			}
			else
			{
				if(e->IsEmptyInside())
				{
					RemoveDrawingElementTriggered(e);
				}
			}
		}
	}
	Repaint();
}

ScType BanBaseWidget::GetType()
{
	return ScType_Basic;
}

QPixmap BanBaseWidget::ToFullPixmap(double xZoom, double yZoom, bool withBackground)
{
	DeHighlightElement();
	QRect unitedRect = rect();

	int width = unitedRect.width();
	int height = unitedRect.height();
	QPixmap pixmap(width, height);
	if(withBackground)
	{
		if(_cachePixmap.width() > 10 && _cachePixmap.height() > 10)
		{
			QSize size(pixmap.width(), pixmap.height());
			QPixmap scaledPix = _cachePixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			pixmap = scaledPix.copy();
		}
		else
			pixmap.fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	}
	else
		pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	SetPainterStyle(painter);
	painter.scale(xZoom, yZoom);
	for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
	{
		Element* element = *i;
		if(element->CanPreview())
			element->DrawOnQtPainter(&painter);
	}
	return pixmap.copy();
}

QPixmap BanBaseWidget::ToFixedPixmap(int x, int y, bool withBackground)
{
	QRect unitedRect = GetUnitedRect();
	double xZoom = (double)x/unitedRect.width();
	double yZoom = (double)y/unitedRect.height();

	return ToPixmap(xZoom, yZoom, withBackground);
}

QRect BanBaseWidget::GetUnitedRect(double xZoom, double yZoom)
{
	QRect rect;
	bool first = true;
	{
		for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			Element* element = *i;
			if(element->CanPreview())
			{
				if(first)
				{
					rect = element->GetActualRect();
					first = false;
				}
				else
				{
					rect = element->GetActualRect().united(rect);
				}
			}
		}
	}
	if(false)
		rect.adjust(-MATERIALITEM_RECT_INSIDE, -MATERIALITEM_RECT_INSIDE, MATERIALITEM_RECT_INSIDE, MATERIALITEM_RECT_INSIDE);
	return QRect(rect.topLeft().x()*xZoom, rect.topLeft().y()*yZoom, rect.width()*xZoom, rect.height()*yZoom);
}

QPixmap BanBaseWidget::ToPixmap(double xZoom, double yZoom, bool withBackground, bool refresh)
{
	DeHighlightElement(refresh);
	QRect unitedRect = GetUnitedRect(xZoom, yZoom);

	int width = unitedRect.width();
	int height = unitedRect.height();
	QPixmap pixmap(width, height);
	if(withBackground)
		pixmap.fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	else
		pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	SetPainterStyle(painter);
	painter.translate(-unitedRect.topLeft().x(), -unitedRect.topLeft().y());
	painter.scale(xZoom, yZoom);
	for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
	{
		Element* element = *i;
		//只要是设置painter的scale就行了，不需要里面做事情，Qt的这种设计是我没有想到的
		if(element->CanPreview())
			element->DrawOnQtPainter(&painter);
	}
	return pixmap.copy();
}

bool BanBaseWidget::CanQuit()
{
	return !_dirty;
}

QString BanBaseWidget::GetObjectUuid()
{
	return _uuid.toString();
}

void BanBaseWidget::SetMode(DrawMode mode)
{
	if(_disableDraw)
		return;

	_mode = mode;
	setFocus();
	ChangeModeCursor();
	DeHighlightElement();
}

void BanBaseWidget::ChangeModeCursor()
{
	qApp->restoreOverrideCursor();

	if(_mode == DrawMode_Draw)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/draw");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_Line)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/line");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_Rect)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/rect");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_PerfectRect)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/pRect");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_Ellipse)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/ellipse");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_PerfectEllipse)
	{
		QCursor cursor = GetDrawingCursor(":/Cursor/pEllipse");
		setCursor(cursor);
	}
	else if(_mode == DrawMode_Select)
	{
		setCursor(Qt::OpenHandCursor);
	}
}

QCursor BanBaseWidget::GetDrawingCursor(QString resource)
{
	QPixmap pixmap(resource);
	QPixmap mask = pixmap.createMaskFromColor(QColor(255, 0, 0), Qt::MaskOutColor);

	{
		QPainter painter(&pixmap);
		painter.setPen(_lineQColor);
		painter.drawPixmap(pixmap.rect(), mask, mask.rect());
	}

	return QCursor(pixmap, 9, 9);
}


void BanBaseWidget::DrawOnQtPainter(QPainter* painter)
{
	//currently, it is path, need to change symbol draw, right?
	for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
	{
		Element* element = *i;
		if(element->CanPaint())
			element->DrawOnQtPainter(painter);
	}
}

//这个不太正确，应该保存一份当前的缓存，不要每次都画，而是图片复制，除非resize, repaint都不算
void BanBaseWidget::RenderPixmap(QPixmap& pixmap)
{
	pixmap.fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	QPainter painter(&pixmap);
	SetPainterStyle(painter);

	if(_cachePixmap.width() > 10 && _cachePixmap.height() > 10)
	{
		QSize size(pixmap.width(), pixmap.height());
		QPixmap scaledPix = _cachePixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		painter.drawPixmap(0, 0, scaledPix.width(), scaledPix.height(), scaledPix);
	}

	{
		//2014,加入world matrix功能，为了LiveWidget
		painter.setWorldMatrix(_worldMatrix, true);

		//currently, it is path, need to change symbol draw, right?
		for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			Element* element = *i;
			if(element->CanPaint())
				element->DrawOnQtPainter(&painter);
		}
	}
}

void BanBaseWidget::SetPainterStyle(QPainter& painter)
{
	//这个必须开启，否则效果太差，是不是每次都画效果差呢？
	//还是说pc上有webrtc, skia呢？
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	//下面的设置还需要，在drawing的时候有用
	//	painter.setMatrix(_matrix);
	QPen pen(_lineQColor);
	pen.setWidth(_lineWidth);
	pen.setStyle(_lineStyle);
	painter.setPen(pen);
}

void BanBaseWidget::Repaint(bool updateBackgroud)
{
	//对于程序，不repaint, 而是用程序画上去
	if(!_embedded)
	{
		if(updateBackgroud)
		{
			RenderPixmap(_backgroudPixmap);
			LetDrawingPixmapReady();
		}
		update();
	}
}

void BanBaseWidget::LetDrawingPixmapReady()
{
	_drawingPixmap = _backgroudPixmap.copy(0, 0, _backgroudPixmap.width(), _backgroudPixmap.height());
}

void BanBaseWidget::DrawModeTriggered()
{
	SetMode(DrawMode_Draw);
}

void BanBaseWidget::LineModeTriggered()
{
	SetMode(DrawMode_Line);
}

void BanBaseWidget::RectModeTriggered()
{
	SetMode(DrawMode_Rect);
}

void BanBaseWidget::PerectRectModeTriggered()
{
	SetMode(DrawMode_PerfectRect);
}

void BanBaseWidget::EclipseModeTriggered()
{
	SetMode(DrawMode_Ellipse);
}

void BanBaseWidget::PerfectEclipseModeTriggered()
{
	SetMode(DrawMode_PerfectEllipse);
}

void BanBaseWidget::SelectModeTriggered()
{
	SetMode(DrawMode_Select);
}

void BanBaseWidget::LineWidthChanged(int width, bool all)
{
	_lineWidth = width;
	PathElement* pathE = dynamic_cast<PathElement*>(_highlightElement);
	if(_highlightElement != NULL && pathE == NULL)
	{
		_highlightElement->SetCanDelete(false);
		_highlightElement->SetLineWidth(width);
		_highlightElement->Highlight(this, false);
		_highlightElement->SetLineWidth(width);
		SetModifiedTriggered();
		_highlightElement->Highlight(this);
		_highlightElement->SetCanDelete(true);
		LineWidthAction widthAction;
		widthAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
		widthAction.SetTartgetElementUuid(_highlightElement->GetObjectUuid());
		widthAction.Update(_lineWidth);
		Q_EMIT OnAnimationAction(widthAction, _highlightElement);
	}

	if(all)
	{
		for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			//fix later
			BanElement* scElement = dynamic_cast<BanElement*>(*i);
			if(scElement == NULL)
			{
				if((*i)->CanPaint())
					(*i)->SetLineWidth(width);
			}
		}
	}
	//SetMode(_mode);
	Repaint();
}

void BanBaseWidget::LineStyleChanged(int style, bool all)
{
	_lineStyle = (Qt::PenStyle)style;

	PathElement* pathE = dynamic_cast<PathElement*>(_highlightElement);
	if(_highlightElement != NULL && pathE ==  NULL)
	{
		_highlightElement->SetCanDelete(false);
		_highlightElement->SetLineStyle(style);
		_highlightElement->Highlight(this, false);
		SetModifiedTriggered();
		_highlightElement->Highlight(this);
		_highlightElement->SetCanDelete(true);
		LineStyleAction styleAction;
		styleAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
		styleAction.SetTartgetElementUuid(_highlightElement->GetObjectUuid());
		styleAction.Update(_lineStyle);
		Q_EMIT OnAnimationAction(styleAction, _highlightElement);
	}

	if(all)
	{
		for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			//fix later
			BanElement* scElement = dynamic_cast<BanElement*>(*i);
			if(scElement == NULL)
			{
				if((*i)->CanPaint())
					(*i)->SetLineStyle(style);
			}
		}
	}
	//SetMode(_mode);
	Repaint();
}

void BanBaseWidget::LineColorChanged(QColor color, bool all)
{
	_lineQColor = color;
	_lineColor = _lineQColor.rgba();

	PathElement* pathE = dynamic_cast<PathElement*>(_highlightElement);
	//不是PathElement的其它Element
	if(_highlightElement != NULL && pathE ==  NULL)
	{
		_highlightElement->SetCanDelete(false);
		_highlightElement->SetLineColor(color);
		_highlightElement->Highlight(this, false);
		SetModifiedTriggered();
		_highlightElement->Highlight(this);
		_highlightElement->SetCanDelete(true);
		LineColorAction colorAction;
		colorAction.SetTartgetBanUuid(GetObjectUuid().toStdString());
		colorAction.SetTartgetElementUuid(_highlightElement->GetObjectUuid());
		colorAction.Update(_lineQColor);
		Q_EMIT OnAnimationAction(colorAction, _highlightElement);
	}

	if(all)
	{
		for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); i++)
		{
			//fix later
			BanElement* scElement = dynamic_cast<BanElement*>(*i);
			if(scElement == NULL)
			{
				if((*i)->CanPaint())
					(*i)->SetLineColor(color);
			}
		}
	}

	//change the cursor color
	ChangeModeCursor();
	Repaint();
}

void BanBaseWidget::ReviseDrawTriggered(bool ok)
{
	_reviseDraw = ok;
}

void BanBaseWidget::ChangeImageElementSize(string uuid, QPoint leftTop, QSize size)
{
	for(auto& element : _allElements)
	{
		if(element->GetObjectUuid() == uuid)
		{
			ImageElement* imageElement = dynamic_cast<ImageElement*>(element);
			if(imageElement != NULL)
			{
				imageElement->UpdateSize(leftTop, size.width(), size.height());
				//这里需要刷新本地的？是吧
				//不能边改size,边刷新，这可能是问题，需要所有操作完成，repaint
				//Repaint();
				break;
			}
		}
	}
}

void BanBaseWidget::AddToDrawing(string uuid)
{
	bool find = false;
	for(auto& element : _drawingElements)
	{
		if(element->GetObjectUuid() == uuid)
		{
			find = true;
			break;
		}
	}

	if(!find)
	{
		Element* need = NULL;
		for(auto& element : _allElements)
		{
			if(element->GetObjectUuid() == uuid)
			{
				need = element;
				break;
			}
		}
		if(need != NULL)
		{
			_drawingElements.push_back(need);
		}
	}
}

void BanBaseWidget::RemoveFromDrawing(string uuid)
{
	for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); ++i)
	{
		if((*i)->GetObjectUuid() == uuid)
		{
			_drawingElements.erase(i);
			break;
		}
	}
}

void BanBaseWidget::SetHighlightElement(Element* element)
{
	_highlightElement = element;
	if(_highlightElement != NULL)
	{
		Q_EMIT OnHighlightElementChanged(element);
	}
}

void BanBaseWidget::HighlightElement(QPoint pos, bool repaint)
{
	DeHighlightElement(false);
	SetHighlightElement(NULL);
	setFocus();
	{
		bool getFirst = false;
		for (list<Element*>::reverse_iterator i = _drawingElements.rbegin(); i!= _drawingElements.rend(); ++i)
		{
			Element* element = *i;
			if(element->CanPaint())
			{
				if(element->IsPointIn(pos) && !getFirst)
				{
					element->Highlight(this, true);
					SetHighlightElement(element);
					getFirst = true;
					//break;
				}
				else
				{
					element->Highlight(this, false);
				}
			}
		}
	}
	if(repaint)
		Repaint();
}

void BanBaseWidget::DeHighlightElement(bool repaint)
{
	if(_highlightElement != NULL)
	{
		_highlightElement->Highlight(this, false);
		SetHighlightElement(NULL);
		if(repaint)
			Repaint();
	}
}

void BanBaseWidget::dragEnterEvent(QDragEnterEvent *e)
{
	if(_disableDrop)
		return;
	const QMimeData *mimeData = e->mimeData();
	if(mimeData->hasUrls())
	{
		e->acceptProposedAction();
	}
}

void BanBaseWidget::dropEvent(QDropEvent *e)
{
	if(_disableDrop)
		return;

	const QMimeData *mimeData = e->mimeData();
	if(mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); ++i)
		{
			QUrl url = *i;
			QPixmap pixmap;
			if(pixmap.load(url.toLocalFile()))
			{
				if(pixmap.hasAlpha())
				{
					ImageElement* element = new ImageElement(GetCursorPoint(), pixmap);
					AddElement(element);
				}
				else
				{
					CompressedImageElement* element = new CompressedImageElement(GetCursorPoint(), pixmap);
					AddElement(element);
				}
			}
		}
	}
}

void BanBaseWidget::resizeEvent(QResizeEvent *e)
{
	_backgroudPixmap = QPixmap(e->size());
	RenderPixmap(_backgroudPixmap);
	LetDrawingPixmapReady();
	Q_EMIT OnBitmapSizeChanged(e->size());
	QWidget::resizeEvent(e);
}

void BanBaseWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	//这个painter不需要
	//SetPainterStyle(painter);
	if(_isDrawing)
	{
		painter.drawPixmap(0, 0, _drawingPixmap);
	}
	else
	{
		//这里的想法是，在有数据改变的情况下，直接重做_backgroudPixmap,而在paint里，直接重绘，这样，很多事情
		//都很直接
		//RenderPixmap(*_backgroudPixmap);
		painter.drawPixmap(0, 0, _backgroudPixmap);
	}
	QWidget::paintEvent(e);
}

bool BanBaseWidget::eventFilter(QObject * watched, QEvent * e)
{
	if(e->type() == QEvent::MouseButtonDblClick)
	{
		ImageElementHelper* imageHelper = dynamic_cast<ImageElementHelper*>(watched);
		BanElementHelper* banHelper = dynamic_cast<BanElementHelper*>(watched);
		TextElementHelper* textHelper = dynamic_cast<TextElementHelper*>(watched);
		HtmlTextElementHelper* htmlHelper = dynamic_cast<HtmlTextElementHelper*>(watched);
		if(imageHelper != NULL 
			|| banHelper != NULL
			|| textHelper != NULL 
			|| htmlHelper != NULL)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
			QPoint global = mouseEvent->globalPos();
			QPoint pos = this->mapFromGlobal(global);
			QMouseEvent* newEvent = new QMouseEvent(mouseEvent->type(), pos, mouseEvent->screenPos(), mouseEvent->button(), mouseEvent->buttons(), mouseEvent->modifiers());
			mouseDoubleClickEvent(newEvent);
			delete newEvent;
			return true;
		}
	}
	return QWidget::eventFilter(watched, e);
}

void BanBaseWidget::keyPressEvent(QKeyEvent* e)
{
	/*
	if(e->key() == Qt::Key_Control && !_disableHighlight)
	{
		HighlightWidget(mapFromGlobal(QCursor::pos()));
		e->accept();
		return;
	}
	*/
	return QWidget::keyPressEvent(e);
}

void BanBaseWidget::keyReleaseEvent(QKeyEvent* e)
{
	if(e->key() == Qt::Key_Control)
	{
	}
	return QWidget::keyReleaseEvent(e);
}

void BanBaseWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	if(e->button() != Qt::LeftButton)
	{
		return;
	}
	
	if(_mode != DrawMode_Select)
	{
		return;
	}
#ifdef _LiangshiDesktop
	if(!_disablePaste)
	{
		if(e->modifiers() & Qt::ControlModifier)
		{
			HtmlTextElement* element = new HtmlTextElement(e->pos(), "");
			AddElement(element);
		}
		else
		{
			TextElement* element = new TextElement(e->pos(), "");
			AddElement(element);
		}
	}
#endif
	//QWidget::mouseDoubleClickEvent(e);
}

void BanBaseWidget::contextMenuEvent(QContextMenuEvent *e)
{
}

void BanBaseWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
	{
		return;
	}

	if(_mode == DrawMode_Select && !_disableHighlight)
	{
		//hightlight需要repaint
		HighlightElement(e->pos(), true);
	}

	if(_disableDraw)
		return;

	//setMouseTracking(true);
	qApp->restoreOverrideCursor();
	if(_mode == DrawMode_Draw ||
		_mode == DrawMode_Line ||
		_mode == DrawMode_Rect ||
		_mode == DrawMode_PerfectRect ||
		_mode == DrawMode_Ellipse ||
		_mode == DrawMode_PerfectEllipse)
	{
		SetMode(_mode);
		_isDrawing = true;
		_movingPoints.clear();
		_clickPoint = e->pos();
		_movingPoints.push_back(_clickPoint);
		LetDrawingPixmapReady();
	}
	QWidget::mousePressEvent(e);
}

void BanBaseWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(_isDrawing)
	{
		if(_mode == DrawMode_Draw)
		{
			_movingPoints.push_back(e->pos());
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			painter.drawLine(_clickPoint, e->pos());
			_clickPoint = e->pos();
		}

		if(_mode == DrawMode_Line)
		{
			LetDrawingPixmapReady();
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			painter.drawLine(_clickPoint, e->pos());
		}

		if(_mode == DrawMode_Rect)
		{
			LetDrawingPixmapReady();
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			QRect rect(_clickPoint, e->pos());
			painter.drawRect(rect);
		}

		if(_mode == DrawMode_PerfectRect)
		{
			LetDrawingPixmapReady();
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			QRect rect(_clickPoint, Util::GetPerfectPoint(_clickPoint, e->pos()));
			painter.drawRect(rect);
		}

		if(_mode == DrawMode_Ellipse)
		{
			LetDrawingPixmapReady();
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			QRect rect(_clickPoint, e->pos());
			painter.drawEllipse(rect);
		}

		if(_mode == DrawMode_PerfectEllipse)
		{
			LetDrawingPixmapReady();
			QPainter painter(&_drawingPixmap);
			SetPainterStyle(painter);
			QRect rect(_clickPoint, Util::GetPerfectPoint(_clickPoint, e->pos()));
			painter.drawEllipse(rect);
		}

		update();
	}
	else
	{
		if(e->modifiers() & Qt::ControlModifier && _mode == DrawMode_Select && !_disableHighlight)
		{
			//hightlight需要repaint
			HighlightElement(e->pos(), true);
		}
	}
	QWidget::mouseMoveEvent(e);
}

void BanBaseWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
	{
		return;
	}

	_isDrawing = false;
	if(_disableDraw)
		return;

	if(_mode == DrawMode_Draw)
	{
		if(_reviseDraw)
		{
			DrawElement* element = new DrawElement();
			if(element->UpdatePoints(_movingPoints))
			{
				AddElement(element);
			}
			else
			{
				delete element;
			}
		}
		else
		{
			OrigialDrawElement* element = new OrigialDrawElement();
			if(element->UpdatePoints(_movingPoints))
			{
				AddElement(element);
			}
			else
			{
				delete element;
			}
		}
	}

	if(_mode == DrawMode_Line)
	{
		LineElement* element = new LineElement();
		if(element->UpdatePoints(_clickPoint, e->pos()))
		{
			AddElement(element);
		}
		else
		{
			delete element;
		}
	}

	if(_mode == DrawMode_Rect)
	{
		RectElement* element = new RectElement();
		if(element->UpdatePoints(_clickPoint, e->pos()))
		{
			AddElement(element);
		}
		else
		{
			delete element;
		}
	}

	if(_mode == DrawMode_PerfectRect)
	{
		RectElement* element = new RectElement();
		if(element->UpdatePoints(_clickPoint, Util::GetPerfectPoint(_clickPoint, e->pos())))
		{
			AddElement(element);
		}
		else
		{
			delete element;
		}
	}

	if(_mode == DrawMode_Ellipse)
	{
		EllipseElement* element = new EllipseElement();
		if(element->UpdatePoints(_clickPoint, e->pos()))
		{
			AddElement(element);
		}
		else
		{
			delete element;
		}
	}

	if(_mode == DrawMode_PerfectEllipse)
	{
		EllipseElement* element = new EllipseElement();
		if(element->UpdatePoints(_clickPoint, Util::GetPerfectPoint(_clickPoint, e->pos())))
		{
			AddElement(element);
		}
		else
		{
			delete element;
		}
	}
	QWidget::mouseReleaseEvent(e);
}

