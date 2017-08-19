#include "../common/Qt.h"

#ifdef Q_OS_WIN32
#include "../common/platforms/windows/WindowsFileAssociation.h"
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

#ifdef Q_OS_WIN32
	WindowsFileAssociation windows;
	windows.RegisterFileType("ban.Document", "板书文件", ".ban", "collect.exe");
	windows.RegisterFileType("ke.Document", "课件文件", ".ke", "player.exe");
	windows.RebuildIcons();
#endif
}
