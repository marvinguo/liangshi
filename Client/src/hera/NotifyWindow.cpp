#include "NotifyWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

NotifyWindow::NotifyWindow(QWidget* parent):
QWidget(parent),
QtSingleton<NotifyWindow>(this),
_delay(8000)
{
	setAttribute(Qt::WA_ShowWithoutActivating,true);
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);

#if defined(Q_OS_MAC)
	flags = flags | Qt::FramelessWindowHint;
#else
	flags = flags | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip;
#endif
	setWindowFlags(flags);

	QLabel* icon = new QLabel();
//	icon->setPixmap(QPixmap(DXIM_DATADIR "/icons/dxim.png"));
	_text = new QLabel();
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(icon);
	mainLayout->addWidget(_text);
	mainLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum));
	setLayout(mainLayout);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);

	//_timer = new QTimer(this);
	//connect(_timer, &QTimer::timeout, this, &NotifyWindow::OnHideTriggered);
	//_timer->stop();
}

NotifyWindow::~NotifyWindow()
{
	hide();
	//_timer->stop();
}

void NotifyWindow::ShowNotify(QString text, int position)
{
	hide();
#if defined _WIN32
	_text->setText(QString("<b>良师益友软件提示</b><br>%1").arg(text));
	QRect geometry = QApplication::desktop()->availableGeometry();
	int moveToPointX = 0;
	int moveToPointY = 0;

	int width = sizeHint().width() + 5;
	int height = sizeHint().height() + 5;

	moveToPointX = geometry.right() - width;
	moveToPointY = geometry.bottom() - height;

	move(moveToPointX, moveToPointY);
	show();
	QTimer::singleShot(_delay, this, SLOT(hide()));
	//_timer->start(_delay);
#elif defined Q_OS_ANDROID
	qDebug() << "NotifyWindow::ShowNotify android branch";
	QAndroidJniObject jtext = QAndroidJniObject::fromString(text);
	QAndroidJniObject::callStaticMethod<void>("org/qtproject/qt5/android/bindings/QtActivity",
		"displayToast",
		"(Ljava/lang/String;)V",
		jtext.object<jstring>());
	qDebug() << "end NotifyWindow::ShowNotify android branch";
#endif
}

void NotifyWindow::OnHideTriggered()
{
	//this->hide();
}