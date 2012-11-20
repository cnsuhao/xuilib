#include "stdafx.h"

static CString g_strXLibPath;

class CXUILIFE
{
public:
	CXUILIFE();
	~CXUILIFE();
};

CXUILIFE g_xui_life;

CXUILIFE::CXUILIFE()
{
	CString strLocalXLib(SMCRelativePath(_T("XLib.dll")));
	if (::PathFileExists(strLocalXLib))
	{
		g_strXLibPath = strLocalXLib;
		return;
	}

	TCHAR szDevXLib[MAX_PATH] = {};
	::GetModuleFileName(NULL, szDevXLib, MAX_PATH);
	::PathRemoveFileSpec(szDevXLib);
#ifdef _DEBUG
	::PathAppend(szDevXLib, _T("../../XLib/Debug/XLib.dll"));
#else
	::PathAppend(szDevXLib, _T("../../XLib/Release/XLib.dll"));
#endif

	if (::PathFileExists(szDevXLib))
	{
		g_strXLibPath = szDevXLib;
		return;
	}

	ATLASSERT(!_T("FATAL ERROR: NO XLIB FOUND."));
}

CXUILIFE::~CXUILIFE()
{
	SMCReleaseAllDLL();
}

const CString & XLibPath()
{
	return g_strXLibPath;
}
