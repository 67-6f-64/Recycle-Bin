#include"AirWindow2.h"

namespace Air{
	HFONT DefaultFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "ＭＳ ゴシック");

	int GetWidthByString(LPCSTR lpWindowName);
	void SetFont(HWND hDlg, int nIDDlgItem);
}

//文字列の幅を計算
int Air::GetWidthByString(LPCSTR lpWindowName){
	int i;

	for(i=0; lpWindowName[i]; i++){
		//none
	}

	return i*6;
}

//フォント変更
void Air::SetFont(HWND hDlg, int nIDDlgItem){
	SendDlgItemMessageA(hDlg, nIDDlgItem, WM_SETFONT, (WPARAM)DefaultFont, NULL);
}

//チェックボックスコントロール
bool Air::CreateCheckBox(HINSTANCE hInstance, HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 15;
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndParent){
		return false;
	}
	HWND bla;
	bla = CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndParent, (HMENU)nIDDlgItem, hInstance, NULL);




	SetFont(hWndParent, nIDDlgItem);

	if(bCheck){
		CheckDlgButton(hWndParent, nIDDlgItem, BST_CHECKED);
	}
	ShowWindow(bla ,SW_SHOW);
	UpdateWindow(hWndParent);

	return true;
}

/*
bool AirWindow::CreateButton(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 12;
	int nHeight = 18;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndAW){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+1), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}

//エディットコントロール
bool AirWindow::CreateEditBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth, bool bNumber, int nLine){
	int nHeight = 16;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER);
	
	if(bNumber){
		dwStyle |= ES_NUMBER;
	}

	if(nLine > 1){
		dwStyle |= (ES_MULTILINE | WS_VSCROLL);
		nHeight += (nLine-1)*12;
	}

	if(!hWndAW){
		return false;
	}

	if(!CreateWindowExA(NULL, "EDIT", lpWindowName, dwStyle, X, (Y+2), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}

//スタティックテキスト
bool AirWindow::CreateStaticText(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName);
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndAW){
		return false;
	}

	if(!CreateWindowExA(NULL, "STATIC", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}*/