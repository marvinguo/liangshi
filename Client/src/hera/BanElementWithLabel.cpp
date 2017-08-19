#include "BanElementWithLabel.h"
#include "BanFile.h"
#include "../common/Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanElementWithLabel::BanElementWithLabel():
BanElement()
{
}

BanElementWithLabel::BanElementWithLabel(BanBaseWidget* viwer):
BanElement(viwer)
{
}

BanElementWithLabel::~BanElementWithLabel()
{
}

void BanElementWithLabel::SetLabel(QString title)
{
	_label = title.toStdString();
}

QString BanElementWithLabel::GetLabel()
{
	return QString(_label.c_str());
}

void BanElementWithLabel::SetLineColor(QColor color)
{
	//不走ScElement::SetLineColor
	Element::SetLineColor(color);
}

ElementType BanElementWithLabel::GetType()
{
	return ElementType_BanWithLabel;
}

bool BanElementWithLabel::IsPointIn(QPoint point)
{
	return BanElement::GetActualRect().contains(point);
	//return GetActualRect().contains(point);
}

QRect BanElementWithLabel::GetActualRect()
{
	QRect rect = BanElement::GetActualRect();
	QString text(_label.c_str());
	if(!text.isEmpty())
	{
		text += ":";
		QFont font;
		font.setPointSize(DEFAULT_FONT_SIZE);
		font.setFamily(QObject::tr("宋体"));
		QFontMetrics fm(font);
		int width = fm.width(text) + 2;
		int height = fm.height();
		rect.adjust(-width, 0 , 0, 0);
	}
	return rect;
}

void BanElementWithLabel::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	if(!IsHightlighting())
	{
		BanElement::DrawOnQtPainter(painter);

		QString text(_label.c_str());
		if(!text.isEmpty())
		{
			text += ":";
			QFont font;
			font.setPointSize(DEFAULT_FONT_SIZE);
			font.setFamily(QObject::tr("宋体"));
			painter->setFont(font);
			QPen pen(_lineQColor);
			painter->setPen(pen);
			QFontMetrics fm(font);
			QRect rect = GetActualRect();
			int height = fm.height();
			int x = rect.topLeft().x();
			int y = rect.topLeft().y() + rect.height()/2+height/2;
			if(y > rect.bottomRight().y())
				y = rect.bottomRight().y();
			//下面的函数的x,y 一个是左上角，一个是右下角
			//painter->drawText(x, y , size.width(), size.height(), Qt::AlignTop | Qt::AlignLeft, text);
			painter->drawText(x, y, text);
		}

	}
	painter->restore();
}

LokiBuffer BanElementWithLabel::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

