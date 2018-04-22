#ifndef __AIRGUI_FUNCTION_H__
#define __AIRGUI_FUNCTION_H__

#include<Windows.h>
#include<Richedit.h>

namespace Air{
	void AddString(HWND hDlg, int nIDDlgItem, LPCSTR lpNewString, bool bScroll = true);
	void SetString(HWND hDlg, int nIDDlgItem, LPCSTR lpString);
	void ReadOnly(HWND hDlg, int nIDDlgItem, bool bFlag = true);
	LPSTR GetString(HWND hDlg, int nIDDlgItem);
	void _stdcall AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, ...);
}

#endif