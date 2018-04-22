#ifndef __AIRGUI_SUB_H__
#define __AIRGUI_SUB_H__

#include<Windows.h>
#include<Richedit.h>

//SubWindow
namespace Air{
	void SetMainInstance(HINSTANCE hInstance);
	bool CreateCheckBox(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
	bool CreateButton(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
	bool CreateEdit(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth = 100, bool bNumber = false, int nLine = 1);
	bool CreateStaticText(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth = 0);
}

//Function
namespace Air{
	void AddString(HWND hDlg, int nIDDlgItem, LPCSTR lpNewString, bool bScroll = true);
	void SetString(HWND hDlg, int nIDDlgItem, LPCSTR lpString);
	void ReadOnly(HWND hDlg, int nIDDlgItem, bool bFlag = true);
	LPSTR GetString(HWND hDlg, int nIDDlgItem);
	void _stdcall AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, ...);
}

#endif