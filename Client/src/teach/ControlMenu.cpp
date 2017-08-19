#include "ControlMenu.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

ControlMenu::ControlMenu(QWidget* parent):
Menu(parent),
_rootFolder("d:/sc")
{
}

ControlMenu::~ControlMenu()
{
}

void ControlMenu::InitMenu()
{
	AddKeInDir(tr("素材"), _rootFolder, _rootMenu);
}

void ControlMenu::OnKeSelectedTriggered()
{
	QtPieAction* action = dynamic_cast<QtPieAction*>(sender());
	if(action != NULL)
	{
		QString filepath = (action->GetData()).toString();
		emit OnKeSelected(filepath);
	}
}

void ControlMenu::AddKeInDir(QString title, QString path, QtPieMenu* parent)
{
	if(!path.isEmpty())
	{
		QtPieMenu *materailSubMenu = new QtPieMenu(title, parent, "");
		QDir dir(path);
		//QStringList filters;
		//filters << "*.sc";
		//dir.setNameFilters(filters);
		dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks);
		QFileInfoList files = dir.entryInfoList();
		for(QFileInfoList::iterator i = files.begin(); i != files.end(); ++i)
		{
			QFileInfo file = *i;
			if(file.isFile())
			{
				if(FileUtil::IsValidExtensionFile(file.absoluteFilePath(), "ke"))
				{
					materailSubMenu->InsertItem(file.baseName(), QVariant(file.absoluteFilePath()), this, SLOT(OnKeSelectedTriggered()));
				}
			}

			if(file.isDir() && !FileUtil::IsDirDotTwo(file.fileName()))
			{
				AddKeInDir(file.fileName(), file.absoluteFilePath(), materailSubMenu);
			}
		}
		parent->InsertItem(materailSubMenu);
	}
}
