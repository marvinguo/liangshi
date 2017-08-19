#include "WindowsFileAssociation.h"
#include "OsVersion.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

WindowsFileAssociation::WindowsFileAssociation()
{
}

WindowsFileAssociation::~WindowsFileAssociation()
{
}

//http://tortoisecvs.cvs.sourceforge.net/viewvc/tortoisecvs/TortoiseCVS/src/Utils/ShellUtils.cpp?revision=1.70&view=markup&pathrev=MAIN
bool WindowsFileAssociation::RebuildIcons()
{
	const int BUFFER_SIZE = 1024;

	HKEY hRegKey = 0;
	LONG lRegResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop\\WindowMetrics",
		0, KEY_READ | KEY_WRITE, &hRegKey);
	if (lRegResult != ERROR_SUCCESS)
	{
		return false;
	}

	char* buf = new char[BUFFER_SIZE];

	// Before Win2k, we're going to change the icon size, otherwise the color depht
	std::string sRegValueName;
	bool bUseSize = false;
	if (WindowsVersionIs2KOrHigher())
		sRegValueName = "Shell Icon BPP";
	else
	{
		bUseSize = true;
		sRegValueName = "Shell Icon Size";
	}

	// Read registry value
	DWORD dwSize = BUFFER_SIZE;
	lRegResult = RegQueryValueExA(hRegKey, sRegValueName.c_str(), NULL, NULL, (LPBYTE) buf, &dwSize);
	if (lRegResult == ERROR_FILE_NOT_FOUND)
	{
		strncpy(buf, "32", BUFFER_SIZE);
	}
	else if (lRegResult != ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		return false;
	}


	// Change registry value
	DWORD dwRegValue = atoi(buf);
	DWORD dwRegValueTemp = 0;
	if (!bUseSize)
	{
		if (dwRegValue == 4)
			dwRegValueTemp = 32;
		else
			dwRegValueTemp = 4;
	}
	else
	{
		if (dwRegValue == 31)
			dwRegValueTemp = 32;
		else
			dwRegValueTemp = 31;
	}

	dwSize = _snprintf(buf, BUFFER_SIZE, "%d", dwRegValueTemp) + 1; 
	lRegResult = RegSetValueExA(hRegKey, sRegValueName.c_str(), 0, REG_SZ, (LPBYTE) buf, dwSize); 
	if (lRegResult != ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		delete[] buf;
		return false;
	}


	// Update all windows
	PDWORD dwResult;
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS, 
		0, SMTO_ABORTIFHUNG, 5000, reinterpret_cast<PDWORD_PTR>(&dwResult));

	// Reset registry value
	dwSize = _snprintf(buf, BUFFER_SIZE, "%d", dwRegValue) + 1; 
	lRegResult = RegSetValueExA(hRegKey, sRegValueName.c_str(), 0, REG_SZ, (LPBYTE) buf, dwSize); 
	if(lRegResult != ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		delete[] buf;
		return false;
	}

	// Update all windows
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS, 
		0, SMTO_ABORTIFHUNG, 5000, reinterpret_cast<PDWORD_PTR>(&dwResult));

	RegCloseKey(hRegKey);
	delete[] buf;
	return true;
}

void WindowsFileAssociation::RegisterFileType(const QString& documentId, const QString& fileTypeName, const QString& fileExtension, const QString& appName, qint32 appIconIndex)
{
	LONG lSize = _MAX_PATH * 2;
	wchar_t szTempLokiBuffer[_MAX_PATH * 2];
	LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT,
		(const wchar_t*)fileExtension.utf16(),
		szTempLokiBuffer,
		&lSize);

	if (lResult == ERROR_SUCCESS)
	{
		//return;
	}

	if (!SetHkcrUserRegKey(documentId, fileTypeName))
		return;

	QString dir = QCoreApplication::applicationDirPath();
	QString commandLine = appName == "" ? dir + "/" + QCoreApplication::applicationName() + ".exe" : dir + "/" + appName;
	commandLine = QDir::toNativeSeparators(commandLine);

	if (!SetHkcrUserRegKey(QString("%1\\DefaultIcon").arg(documentId), QString("\"%1\",%2").arg(commandLine).arg(appIconIndex)))
		return;

	lResult = ::RegQueryValue(HKEY_CLASSES_ROOT,
		(const wchar_t*)fileExtension.utf16(),
		szTempLokiBuffer,
		&lSize);

	QString temp = QString::fromUtf16((unsigned short*)szTempLokiBuffer);

	if (lResult != ERROR_SUCCESS || temp.isEmpty() || temp == documentId)
	{
		// no association for that suffix
		if (!SetHkcrUserRegKey(fileExtension, documentId))
			return;

		//SetHkcrUserRegKey(QString("%1\\ShellNew").arg(fileExtension), QLatin1String(""), QLatin1String("NullFile"));
	}

	RegisterCommand("Open", documentId, "\"%1\"", appName);
}

bool WindowsFileAssociation::SetHkcrUserRegKey(QString key, const QString& value, const QString& valueName)
{
	HKEY hKey;

	key.prepend("Software\\Classes\\");

	LONG lRetVal = RegCreateKey(HKEY_CURRENT_USER,
		(const wchar_t*)key.utf16(),
		&hKey);

	if(ERROR_SUCCESS == lRetVal)
	{
		LONG lResult = ::RegSetValueExW(hKey,
			valueName.isEmpty() ? 0 : (const wchar_t*)valueName.utf16(),
			0,
			REG_SZ,
			(CONST BYTE*)value.utf16(),
			(value.length() + 1) * sizeof(wchar_t));

		if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
			return true;
	}
	else
	{
		wchar_t buffer[4096];
		::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, lRetVal, 0, buffer, 4096, 0);
		QString szText = QString::fromUtf16((const ushort*)buffer);
	}
	return false;
}

void WindowsFileAssociation::RegisterCommand(const QString& command, const QString& documentId, const QString cmdLineArg, const QString appName)
{
	QString dir = QCoreApplication::applicationDirPath();
	QString commandLine = appName == "" ? dir + "/" + QCoreApplication::applicationName() + ".exe" : dir + "/" + appName;
	commandLine = QDir::toNativeSeparators(commandLine);
	
	commandLine.prepend(QLatin1String("\""));
	commandLine.append(QLatin1String("\""));

	if(!cmdLineArg.isEmpty())
	{
		commandLine.append(QChar(' '));
		commandLine.append(cmdLineArg);
	}

	if (!SetHkcrUserRegKey(QString("%1\\shell\\%2\\command").arg(documentId).arg(command), commandLine))
		return;       // just skip it
}