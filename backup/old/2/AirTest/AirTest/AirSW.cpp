#include"AirWindow.h"

namespace Air{
	HFONT AWFont = CreateFontA(12, NULL, NULL, NULL, FW_NORMAL, NULL, NULL, NULL, SHIFTJIS_CHARSET, NULL, NULL, NULL, NULL, "ＭＳ ゴシック");
	int CalculateWidth(LPSTR lpWindowName);
}

void AirWindow::SetFont(int nIDDlgItem){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_SETFONT, (WPARAM)Air::AWFont, NULL);
}

int Air::CalculateWidth(LPSTR lpWindowName){
	int i = 0;

	while(lpWindowName[i]){
		i++;
	}

	return i*6;
}

void AirWindow::StaticText(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth){
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT);

	if(nWidth == AUTORESIZE){
		nWidth = Air::CalculateWidth(lpWindowName);
	}

	if(CreateWindowExA(NULL, "STATIC", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
}
}

void AirWindow::CheckBox(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, bool bCheck, int nWidth){
	int nHeight = 13;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);

	if(nWidth == AUTORESIZE){
		nWidth = Air::CalculateWidth(lpWindowName) + 15;
	}

	if(CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+3), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
		CheckDlgButton(hWndAW, nIDDlgItem, bCheck);
	}
}

bool AirWindow::CheckBoxStatus(int nIDDlgItem){
	return IsDlgButtonChecked(hWndAW, nIDDlgItem);
}

void AirWindow::EditBox(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth, int nLine, bool IsOnlyNumber){
	int nHeight = 16;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER);

	if(nWidth == AUTORESIZE){
		nWidth = Air::CalculateWidth(lpWindowName) + 6;
	}

	if(nLine > 1){
		dwStyle |= (ES_MULTILINE | WS_VSCROLL);
		nHeight += (nLine-1) * 12;
	}

	if(IsOnlyNumber){
		dwStyle |= ES_NUMBER;
	}

	if(CreateWindowExA(NULL, "EDIT", lpWindowName, dwStyle, X, (Y+2), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
	}
}

//文字列セット
void AirWindow::SetString(int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hWndAW, nIDDlgItem, lpString);
}

//追記
void AirWindow::AddString(int nIDDlgItem, LPCSTR lpNewString){
	int InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//最下層までスクロール
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
}

//読み取り専用化
void AirWindow::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR AirWindow::GetString(int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	if(StringSize < 1){
		return NULL;
	}

	GetDlgItemTextA(hWndAW, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}

DWORD AirWindow::GetInt(int nIDDlgItem){
	return GetDlgItemInt(hWndAW, nIDDlgItem, NULL, FALSE);
}


//書式指定追記
void _stdcall AirWindow::AddFormatString(int nIDDlgItem, char *Format, ...){
	char *Buffer = (char *)malloc(MaxString);
	Air::_AirPrint(Buffer, Format, (LPDWORD)&Format + 1);
	AddString(nIDDlgItem, Buffer);
}

void _stdcall AirWindow::SetFormatString(int nIDDlgItem, char *Format, ...){
	char *Buffer = (char *)malloc(MaxString);
	Air::_AirPrint(Buffer, Format, (LPDWORD)&Format + 1);
	SetString(nIDDlgItem, Buffer);
}

void AirWindow::Button(int nIDDlgItem, LPSTR lpWindowName, int X, int Y, int nWidth){
	int nHeight = 18;
	DWORD dwStyle = (WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);

	if(nWidth == AUTORESIZE){
		nWidth = Air::CalculateWidth(lpWindowName) + 12;
	}

	if(CreateWindowExA(NULL, "BUTTON", lpWindowName, dwStyle, X, (Y+1), nWidth, nHeight, hWndAW, (HMENU)nIDDlgItem, hInstanceAW, NULL)){
		SetFont(nIDDlgItem);
	}
}

