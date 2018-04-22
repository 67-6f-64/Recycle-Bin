#include"AirGUI_Sub.h"
#include"AirFunction.h"

namespace Air{
	HINSTANCE hMainInstance = NULL;
	HFONT hFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "ＭＳ ゴシック");
	int GetWidthByString(LPCSTR lpWindowName);
	void SetFont(HWND hDlg, int nIDDlgItem);
}

//初期化
void Air::SetMainInstance(HINSTANCE hInstance){
	hMainInstance = hInstance;
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
	SendDlgItemMessageA(hDlg, nIDDlgItem, WM_SETFONT, (WPARAM)hFont, NULL);
}

//チェックボックスコントロール
bool Air::CreateCheckBox(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, bool bCheck, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 15;
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndParent){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndParent, (HMENU)nIDDlgItem, hMainInstance, NULL)){
		return false;
	}

	SetFont(hWndParent, nIDDlgItem);

	if(bCheck){
		CheckDlgButton(hWndParent, nIDDlgItem, BST_CHECKED);
	}

	return true;
}

//プッシュボタンコントロール
bool Air::CreateButton(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName) + 12;
	int nHeight = 18;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndParent){
		return false;
	}

	if(!CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+1), nWidth, nHeight, hWndParent, (HMENU)nIDDlgItem, hMainInstance, NULL)){
		return false;
	}

	SetFont(hWndParent, nIDDlgItem);

	return true;
}

//エディットコントロール
bool Air::CreateEdit(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int nWidth, bool bNumber, int nLine){
	int nHeight = 16;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER);
	
	if(bNumber){
		dwStyle |= ES_NUMBER;
	}

	if(nLine > 1){
		dwStyle |= (ES_MULTILINE | WS_VSCROLL);
		nHeight += (nLine-1)*12;
	}

	if(!hWndParent){
		return false;
	}

	if(!CreateWindowExA(NULL, "EDIT", lpWindowName, dwStyle, X, (Y+2), nWidth, nHeight, hWndParent, (HMENU)nIDDlgItem, hMainInstance, NULL)){
		return false;
	}

	SetFont(hWndParent, nIDDlgItem);

	return true;
}

//スタティックテキスト
bool Air::CreateStaticText(HWND hWndParent, int nIDDlgItem, LPCSTR lpWindowName, int X, int Y, int uWidth){
	int nWidth = GetWidthByString(lpWindowName);
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT);

	if(uWidth){
		nWidth = uWidth;
	}

	if(!hWndParent){
		return false;
	}

	if(!CreateWindowExA(NULL, "STATIC", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndParent, (HMENU)nIDDlgItem, hMainInstance, NULL)){
		return false;
	}

	SetFont(hWndParent, nIDDlgItem);

	return true;
}

//Function
//一行追記
void Air::AddString(HWND hDlg, int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//改行追加
	if(bScroll){
		InjectPoint = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	}

	//最下層までスクロール
	if(bScroll){
		SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//文字列書き込み
void Air::SetString(HWND hDlg,int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hDlg, nIDDlgItem, lpString);
}

//読み取り専用化
void Air::ReadOnly(HWND hDlg, int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hDlg, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR Air::GetString(HWND hDlg, int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hDlg, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	if(StringSize < 1){
		return NULL;
	}

	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	GetDlgItemTextA(hDlg, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}

namespace Air{
	void _stdcall _AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, DWORD *dwArgument);
}


void _stdcall Air::_AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, DWORD *dwArgument){
	char temp[0x100] = {0};
	_CreateFormatString(temp, _Format, dwArgument);
	AddString(hWndParent, nIDDlgItem, temp, true);
}


void _stdcall Air::AddFormatString(HWND hWndParent,int nIDDlgItem, char *_Format, ...){

	_asm{
		pushad
		lea esi,[_Format]
		mov eax,esi
		add eax,4
		push eax
		push [esi]
		push [nIDDlgItem]
		push [hWndParent]
		call _AddFormatString
		popad
	}
}