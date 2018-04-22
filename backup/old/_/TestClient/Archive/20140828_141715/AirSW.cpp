#include"AirWindow.h"

HFONT DefaultFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "ＭＳ ゴシック");

//文字列の幅を計算
int AirWindow::GetWidthByString(LPCSTR lpWindowName){
	int i;

	for(i=0; lpWindowName[i]; i++){
		//none
	}

	return i*6;
}

//フォント変更
void AirWindow::SetFont(int nIDDlgItem){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_SETFONT, (WPARAM)DefaultFont, NULL);
	return;
}

//チェックボックスコントロール
bool AirWindow::CreateCheckBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 15;
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndAW){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	if(bCheck){
		CheckDlgButton(hWndAW, nIDDlgItem, BST_CHECKED);
	}

	return true;
}

//プッシュボタンコントロール
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
}