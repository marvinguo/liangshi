#include "TitleWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TitleWidget::TitleWidget(KeTime* keTime, NewableDocType type, QWidget *parent):
QWidget(parent),
_type(type),
_isMoving(false),
_dragable(false),
_icon(new QLabel()),
_title(new QLabel()),
_drawing(new DrawingToolsTitleWidget(keTime)),
_material(new BanTitleWidget()),
_test(new TestTitleWidget()),
#ifdef _LiangshiDesktop
_script(new ScriptTitleWidget()),
#endif
_ke(new KeTitleWidget(keTime)),
_live(new LiveTitleWidget()),
_play(new PlayerTitleWidget(type, keTime))
{
	switch(type)
	{
	case NewableDocType_Ban:
	case NewableDocType_BanEmbedded:
		_file = new FileTitleWidget(FileTitleType_WithSaveWithoutOpen);
		Init(":/App/collect", true, true, false, false, false, false, false, false);
		break;
	case NewableDocType_BanScript:
		_file = new FileTitleWidget(FileTitleType_WithSaveWithoutOpen);
		Init(":/App/collect", false, false, true, false, false, false, false, false);
		break;
	case NewableDocType_BanTest:
	case NewableDocType_TestEmbedded:
		_file = new FileTitleWidget(FileTitleType_WithSaveWithoutOpen);
		Init(":/App/collect", false, false, false, true, false, false, false, false);
		break;
	case NewableDocType_Ke:
	case NewableDocType_KeEmbedded:
		_file = new FileTitleWidget(FileTitleType_WithSaveWithoutOpen);
		Init(":/App/collect", true, false, false, false, true, false, false, false);
		break;
	case NewableDocType_Player:
		_file = new FileTitleWidget(FileTitleType_WithOpenWithoutSave);
		Init(":/App/player", false, false, false, false, false, false, true, true);
		break;
	case NewableDocType_PlayerPreview:
		_file = new FileTitleWidget(FileTitleType_WithOpenWithoutSave);
		Init(":/App/player", false, false, false, false, false, false, true, true);
		break;
	case NewableDocType_PlayerInBrowser:
		_file = new FileTitleWidget(FileTitleType_WithOpenWithoutSave);
		Init(":/App/player", false, false, false, false, false, false, true, false);
		break;
	case NewableDocType_Live:
	{
		_file = new FileTitleWidget(FileTitleType_WithOpenWithoutSave);
		bool hasMax = false;
		QStringList arguments = QCoreApplication::arguments();
		if (arguments.size() >= 3)
		{
			hasMax = arguments[2].toInt() == 1;
		}

		Init(":/App/live", true, false, false, false, false, true, false, hasMax);
		break;
	}
	default:
		break;
	}
}

TitleWidget::~TitleWidget()
{
	if(_file != NULL)
	{
		_file->deleteLater();
		_file = NULL;
	}
	if(_drawing != NULL)
	{
		_drawing->deleteLater();
		_drawing = NULL;
	}
	if(_material != NULL)
	{
		_material->deleteLater();
		_material = NULL;
	}
#ifdef _LiangshiDesktop
	if(_script != NULL)
	{
		_script->deleteLater();
		_script = NULL;
	}
#endif
	if(_test != NULL)
	{
		_test->deleteLater();
		_test = NULL;
	}
	if(_ke != NULL)
	{
		_ke->deleteLater();
		_ke = NULL;
	}
	if(_live != NULL)
	{
		_live->deleteLater();
		_live = NULL;
	}
	if(_play != NULL)
	{
		_play->deleteLater();
		_play = NULL;
	}
	if(_system != NULL)
	{
		_system->deleteLater();
		_system = NULL;
	}
}

void TitleWidget::	Init(QString icon, bool hasDrawing, bool hasBan, bool hasScript, bool hasTest, bool hasKe, bool hasLive, bool hasPlay, bool hasMax)
{
	QPixmap iconPixmap(icon);
	_icon->setPixmap(iconPixmap);
	_icon->setFixedSize(24, 24);
	_icon->setScaledContents(true);
	_icon->setContentsMargins(3, 3, 0, 0);

	QFont font = _title->font();
	font.setPointSize(10);
	font.setBold(true);
	_title->setFont(font);
	_title->setAlignment(Qt::AlignLeft |Qt::AlignVCenter);
	_title->setContentsMargins(3, 0, 0, 0);

	//2014,09.27，想来还是能dragable,因为这样的吧也可照着编辑
	if(hasPlay || hasKe || hasLive || hasBan)
	{
		if(_type != NewableDocType_PlayerInBrowser)
		{
			_dragable = true;
		}
	}

	_system = new SysTitleWidget(hasMax);

	//_title->setStyleSheet("qproperty-alignment: 'AlignBottom | AlignLeft';");
	//_title->setFixedHeight(25);

	QHBoxLayout* mainLayout = new QHBoxLayout();

	if(_type != NewableDocType_PlayerInBrowser)
	{
		mainLayout->addWidget(_icon, 0, Qt::AlignVCenter);
		if(_type != NewableDocType_Live)
			mainLayout->addWidget(_title, 0, Qt::AlignVCenter);
	}
	
	if(_type != NewableDocType_PlayerInBrowser && _type != NewableDocType_PlayerPreview && _type != NewableDocType_Live)
	{
		mainLayout->addWidget(_file, Qt::AlignLeft | Qt::AlignVCenter);
	}

	//mainLayout->addItem(leftSpace);
	if(hasDrawing)
	{
		mainLayout->addWidget(_drawing, Qt::AlignLeft | Qt::AlignVCenter);
	}
	if(hasBan)
	{
		mainLayout->addWidget(_material, Qt::AlignLeft | Qt::AlignVCenter);
	}
	if(hasScript)
	{
#ifdef _LiangshiDesktop
		mainLayout->addWidget(_script, Qt::AlignLeft | Qt::AlignVCenter);
#endif
	}
	if(hasTest)
	{
		mainLayout->addWidget(_test, Qt::AlignLeft | Qt::AlignVCenter);
	}
	if(hasKe)
	{
		mainLayout->addWidget(_ke, Qt::AlignLeft | Qt::AlignVCenter);
	}
	if(hasLive)
	{
		mainLayout->addWidget(_live, Qt::AlignLeft | Qt::AlignVCenter);
	}
	if(hasPlay)
	{
		mainLayout->addStretch();
		mainLayout->addWidget(_play, 10, Qt::AlignLeft | Qt::AlignVCenter);
	}
	mainLayout->addStretch();
	if(_type != NewableDocType_PlayerInBrowser)
	{
		mainLayout->addWidget(_system, Qt::AlignRight | Qt::AlignVCenter);
	}
	mainLayout->setSpacing(0);
	//这个必须，否则有留白
	mainLayout->setContentsMargins(0, 0, 3, 0);

	setFixedHeight(27);
	setLayout(mainLayout);
}

void TitleWidget::OnSetTitleTriggered(QString title, bool modifysign)
{
	if(_type != NewableDocType_PlayerPreview)
	{
		QFontMetrics metrics(_title->font());
		int width = 100;//_titleLabel->width();
		QString elidedText = metrics.elidedText(title, Qt::ElideRight, width);
		if(modifysign)
			elidedText += "*";
		_title->setText(elidedText);
	}
	else
		_title->setText(tr("预览"));
}

void TitleWidget::mousePressEvent(QMouseEvent *e)
{
    _clickPoint = e->pos();
    _isMoving = true;
}

void TitleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && _isMoving && _dragable)
    {
        QWidget* parent_widget = this->parentWidget();
        QPoint parent_point = parent_widget->pos();
        parent_point.setX(parent_point.x() + e->x() - _clickPoint.x());
        parent_point.setY(parent_point.y() + e->y() - _clickPoint.y());
        parent_widget->move(parent_point);
    }
}

void TitleWidget::mouseReleaseEvent(QMouseEvent *)
{
	_isMoving = false;
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *)
{
	_system->OnMaxTriggered();
	Q_EMIT OnMax();
}

