#include "HtmlElementEditor.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HtmlElementEditor::HtmlElementEditor():
QtSingleton<HtmlElementEditor>(this)
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("HTML文本编辑器"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_viewer = new HtmlElementViewer();
	QScrollArea* scrollArea = new QScrollArea();
//	QHBoxLayout* layout = new QHBoxLayout();
//	layout->addWidget(_viewer);
//	scrollArea->setLayout(layout);
	scrollArea->setWidget(_viewer);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setWidgetResizable(true);

	_edit = new QTextEdit();
	connect(_edit, &QTextEdit::textChanged, this, &HtmlElementEditor::OnEditTextChangeTriggered);
	_edit->setStyleSheet("{line-height:0px; margin-top:0px; padding-top:0px; font-size:24pt;}");
	_edit->setAcceptRichText(false);

	TitleButton* open = new TitleButton(tr("打开"));
	connect(open, &TitleButton::clicked, this, &HtmlElementEditor::OnOpenTriggered);
	TitleButton* save = new TitleButton(tr("另存"));
	connect(save, &TitleButton::clicked, this, &HtmlElementEditor::OnSaveTriggered);
	TitleButton* ok = new TitleButton(tr("确定"));
	connect(ok, &TitleButton::clicked, this, &HtmlElementEditor::OnOKTriggered);
	QHBoxLayout* titlebar = new QHBoxLayout();
	titlebar->addWidget(open);
	titlebar->addWidget(save);
	titlebar->addWidget(ok);

	_mainLayout = new QVBoxLayout();
	_mainLayout->addWidget(scrollArea, 1);
	_mainLayout->addWidget(_edit, 2);
	_mainLayout->setSpacing(0);
	_mainLayout->setContentsMargins(0, 0, 0, 0);

	_mainLayout->addLayout(titlebar);
	_mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(_mainLayout);
	setMinimumWidth(600);
	setMinimumHeight(500);
}

HtmlElementEditor::~HtmlElementEditor()
{
}

void HtmlElementEditor::ShowModal(HtmlTextElement* element)
{
	_element = element;
	_edit->setPlainText(QString(_element->_text.c_str()));
	QString text = _edit->toPlainText();
	_viewer->SetHtmlText(text, 18, _element->_lineQColor);
	_edit->setFocus();
	show();
	exec();
}

void HtmlElementEditor::OnEditTextChangeTriggered()
{
	QString text = _edit->toPlainText();
	_viewer->SetHtmlText(text, 18, _element->_lineQColor);
//	_mainLayout->activate();
//	resize(sizeHint());
}

void HtmlElementEditor::OnOpenTriggered()
{
	QString path = QFileDialog::getOpenFileName(NULL, QObject::tr("打开Html或MathML文件"), ".", QObject::tr("所有支持文件(*.html *.htm *.mml *.txt);;Html文件(*.html *.htm);;MathML文件(*.mml);;文本文件(*.txt)"));
	if(!path.isEmpty())
	{
		QFile file(path);
		file.open(QIODevice::ReadOnly);
		_edit->setPlainText(file.readAll());
		file.close();
	}
}

void HtmlElementEditor::OnSaveTriggered()
{
	QString path = QFileDialog::getSaveFileName(this, tr("另存为文件"));
	if(!path.isEmpty())
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&file);
		out << _edit->toPlainText();
		file.flush();
		file.close();
	}
}

void HtmlElementEditor::OnOKTriggered()
{
	QString text = _edit->toPlainText();
	_element->UpdateText(text);
	close();
}