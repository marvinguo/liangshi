#include "TeachMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TeachMessageBox::TeachMessageBox(QWidget *parent):
QObject(parent),
_parentWidget(parent),
_msgBox(NULL)
{

}

TeachMessageBox::~TeachMessageBox()
{
}

void TeachMessageBox::ShowWaitingForConnection()
{
	_type = 1;
	//这个方法是modal的，但不blocking
	_msgBox = new QMessageBox(_parentWidget);
	_msgBox->setAttribute(Qt::WA_DeleteOnClose);
	_msgBox->setStandardButtons(QMessageBox::Cancel);

	_msgBox->setWindowTitle(tr("信息"));
	_msgBox->setText(tr("正在等待连接"));
	
	_msgBox->setModal(false);
	_msgBox->open(this, SLOT(msgBoxClosed(QAbstractButton*)));

	//下面的方法是非模态的
	//_msgBox->setWindowModality(Qt::NonModal);
	//_msgBox->show();
}

void TeachMessageBox::ShowSyncing()
{
	_type = 2;
	_msgBox = new QMessageBox(_parentWidget);
	_msgBox->setAttribute(Qt::WA_DeleteOnClose);
	_msgBox->setStandardButtons(QMessageBox::Cancel);

	_msgBox->setWindowTitle(tr("信息"));
	_msgBox->setText(tr("正在同步文件"));

	_msgBox->setModal(false);
	_msgBox->open(this, SLOT(msgBoxClosed(QAbstractButton*)));
}

void TeachMessageBox::HideMsgBox()
{
	if(_msgBox != NULL)
	{
		_msgBox->close();
		_msgBox = NULL;
	}
}

void TeachMessageBox::msgBoxClosed(QAbstractButton* button)
{
	if(_msgBox != NULL)
	{
		QMessageBox::ButtonRole role = _msgBox->buttonRole(button);

		//什么也不做？也可以emit一个消息？需不需要
		if(role == QMessageBox::RejectRole)
		{
			if(_type == 1)
			{
				emit OnCancelWaitingForConnection();
			}

			if(_type == 2)
			{
				emit OnCancelSyncing();
			}
		}
	}
}
