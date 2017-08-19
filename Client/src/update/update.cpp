#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <string>
#include <sstream>
using namespace std;

void KillLiangshiProcess(DWORD id, wstring liangshiPath)
{
	HANDLE handle = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
	if(handle == NULL)
		return;

	wchar_t buffer[MAX_PATH];
	GetModuleFileNameEx(handle, NULL, buffer, sizeof(buffer)/sizeof(wchar_t));

	wstring processPath(buffer);
	if(processPath.find(liangshiPath) != std::string::npos)
	{
		TerminateProcess(handle, 0);
		std::ostringstream s;
		s << "taskkill /pid ";
		s << id;
		s << " /f";
		system(s.str().c_str());
		WaitForSingleObject(handle, INFINITE);
		Sleep(500);
	}

	CloseHandle(handle);
	return;
}

wstring GetCurrentExePath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	wstring::size_type pos = wstring(buffer).find_last_of(_T("\\/"));
	return wstring(buffer).substr(0, pos);
}

void MoveUpdate(wstring liangshiPath)
{
	wstring from = liangshiPath + _T("\\Update\\*")  + wstring(1, '\0');
	//from.append(1, '\0');
	wstring to = liangshiPath + wstring(1, '\0');
	//to.append(1, '\0');
	SHFILEOPSTRUCT sfo = {0};
	sfo.wFunc = FO_MOVE;
	sfo.pFrom = from.c_str();
	sfo.pTo = to.c_str();
	sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
	SHFileOperation(&sfo);
}

void DeleteSelf(wstring liangshiPath)
{
	wstring self = liangshiPath + _T("\\Update\\update.exe");
	DeleteFile(self.c_str());
}

int main(void)
{
	wstring liangshiPath = GetCurrentExePath();
	DWORD currentProcessID = GetCurrentProcessId();

	DWORD processes[10000];
	DWORD needed;
	if (!EnumProcesses(processes, sizeof(processes), &needed))
		return 1;

	DWORD cProcesses = needed/sizeof(DWORD);
	for (int i = 0; i < cProcesses; i++ )
	{
		if(processes[i] != currentProcessID)
			KillLiangshiProcess(processes[i], liangshiPath);
	}
	Sleep(5000);
	DeleteSelf(liangshiPath);
	MoveUpdate(liangshiPath);
#ifdef _DEBUG
	wstring liangshi = liangshiPath + _T("\\teach_web.exe");
#else
	wstring liangshi = liangshiPath + _T("\\Á¼Ê¦ÒæÓÑ.exe");
#endif
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	CreateProcess(NULL,
		const_cast<wchar_t*>(liangshi.c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	return 0;
}

