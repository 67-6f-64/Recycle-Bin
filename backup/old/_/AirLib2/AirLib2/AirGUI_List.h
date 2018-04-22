#ifndef __AIRGUI_LIST_H__
#define __AIRGUI_LIST_H__

#include<Windows.h>
#include<Richedit.h>
#include<commctrl.h>
#pragma comment(lib,"comctl32.lib")

#define Temporary_String_Max_Length 8192

class ListView{
	private:
		HWND hwndLV;
		DWORD HeaderCount;
		DWORD ListCount;
		DWORD Select;
		DWORD Selected;
		char lpTemporary[Temporary_String_Max_Length];

	public:
		ListView();
		HWND CreateSimpleListView(HWND hWndParent, int X, int Y, int nWidth, int nHeight, bool bMultiSelect = true, bool bGridLine = true);
		bool CreateHeader(LPSTR pszText, int cx);
		bool _stdcall _AddItem(LPSTR *pszText);
		bool GetSelectedItem(int ItemNum, HWND hDlg, int nIDDlgItem);
		HWND GetHWND();
		void Clear();
};

#endif