#include "BanScriptEditor.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanScriptEditor::BanScriptEditor(QWidget *parent):
JSEdit(parent)
{
	setWindowTitle(tr("程序编辑器"));
	setStyleSheet("font: 12pt \"Courier\";");
	setWordWrapMode(QTextOption::NoWrap);

	QFont font;
	font.setFamily("Courier");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(12);
	const int tabStop = 4;
	QFontMetrics metrics(font);
	setTabStopWidth(tabStop * metrics.width(' '));

	// dark color scheme
	setColor(JSEdit::Background, QColor("#0C152B"));
	setColor(JSEdit::Normal,     QColor("#FFFFFF"));
	setColor(JSEdit::Comment,    QColor("#666666"));
	setColor(JSEdit::Number,     QColor("#DBF76C"));
	setColor(JSEdit::String,     QColor("#5ED363"));
	setColor(JSEdit::Operator,   QColor("#FF7729"));
	setColor(JSEdit::Identifier, QColor("#FFFFFF"));
	setColor(JSEdit::Keyword,    QColor("#FDE15D"));
	setColor(JSEdit::BuiltIn,    QColor("#F7240F"));
	setColor(JSEdit::Cursor,     QColor("#1E346B"));

	//_codeEdit->setWindowFlags(Qt::FramelessWindowHint); 
	//_codeEdit->setWindowFlags(Qt::WindowTitleHint);

	setWindowIcon(QIcon(":/App/collect"));
	resize(800, 600);
}

BanScriptEditor::~BanScriptEditor()
{
}

void BanScriptEditor::closeEvent(QCloseEvent *e)
{
	e->ignore();
	hide();
}

