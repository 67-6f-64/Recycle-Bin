#include"SubWindow.h"

/*********************************************************************************
	サブウィンドウ作成系

*********************************************************************************/

Air::SubWindow::SubWindow(){

	hMain = NULL;
	hMainInstance = NULL;
	hFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "ＭＳ ゴシック");

}

//初期化
bool Air::SubWindow::Init(HWND hWndParent){

	if(!hMain){
		hMain = hWndParent;
		hMainInstance = (HINSTANCE)GetWindowLongA(hWndParent, GWL_HINSTANCE);
		return true;
	}

	return false;
}

//文字列の幅を計算
int Air::SubWindow::GetWidthByString(LPCSTR lpWindowName){
	int i;

	for(i=0; lpWindowName[i]; i++){
		//none
	}

	return i*6;
}

//フォント変更
void Air::SubWindow::SetFont(int nIDDlgItem){
	SendDlgItemMessageA(hMain, nIDDlgItem, WM_SETFONT, (WPARAM)hFont, NULL);
}

//チェックボックスコントロール
bool Air::SubWindow::CreateCheckBox(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 15;
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hMain){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hMain, (HMENU)nIDDlgItem, NULL, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}

//プッシュボタンコントロール
bool Air::SubWindow::CreateButton(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 12;
	int nHeight = 18;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hMain){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+1), nWidth, nHeight, hMain, (HMENU)nIDDlgItem, NULL, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}

//エディットコントロール
bool Air::SubWindow::CreateEdit(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth, bool bNumber, int nLine){
	int nHeight = 16;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER);
	
	if(bNumber){
		dwStyle |= ES_NUMBER;
	}

	if(nLine > 1){
		dwStyle |= (ES_MULTILINE | WS_VSCROLL);
		nHeight += (nLine-1)*12;
	}

	if(!hMain){
		return false;
	}

	if(!CreateWindowExA(NULL, "EDIT", lpWindowName, dwStyle, X, (Y+2), nWidth, nHeight, hMain, (HMENU)nIDDlgItem, NULL, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}

//スタティックテキスト
bool Air::SubWindow::CreateStaticText(int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName);
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hMain){
		return false;
	}

	if(!CreateWindowExA(NULL, "STATIC", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hMain, (HMENU)nIDDlgItem, NULL, NULL)){
		return false;
	}

	SetFont(nIDDlgItem);

	return true;
}



/*********************************************************************************
	追加機能などなど

*********************************************************************************/

//一行追記
void Air::SubWindow::AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hMain, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hMain, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hMain, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);
	//改行追加
	InjectPoint = SendDlgItemMessageA(hMain, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
	SendDlgItemMessageA(hMain, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hMain, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");

	//最下層までスクロール
	if(bScroll){
		SendDlgItemMessageA(hMain, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//文字列書き込み
void Air::SubWindow::SetString(int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hMain, nIDDlgItem, lpString);
}

//読み取り専用化
void Air::SubWindow::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hMain, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR Air::SubWindow::GetString(int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hMain, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	if(StringSize < 1){
		return NULL;
	}

	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	GetDlgItemTextA(hMain, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}