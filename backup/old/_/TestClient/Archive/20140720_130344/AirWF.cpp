#include"AirWindow.h"

//一行追記
void AirWindow::AddString(int nIDDlgItem, LPCSTR lpNewString, bool bScroll){
	int InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	//文字列追加
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)lpNewString);

	//改行追加
	if(bScroll){
		InjectPoint = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETSEL, InjectPoint, InjectPoint);
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_REPLACESEL, NULL, (LPARAM)"\r\n");
	}

	//最下層までスクロール
	if(bScroll){
		SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SCROLL, SB_BOTTOM, NULL);
	}

}

//文字列書き込み
void AirWindow::SetString(int nIDDlgItem, LPCSTR lpString){
	SetDlgItemTextA(hWndAW, nIDDlgItem, lpString);
}

//読み取り専用化
void AirWindow::ReadOnly(int nIDDlgItem, bool bFlag){
	SendDlgItemMessageA(hWndAW, nIDDlgItem, EM_SETREADONLY, bFlag, NULL);
}

//文字列取得
LPSTR AirWindow::GetString(int nIDDlgItem){
	int StringSize = SendDlgItemMessageA(hWndAW, nIDDlgItem, WM_GETTEXTLENGTH, NULL, NULL);

	if(StringSize < 1){
		return NULL;
	}

	LPSTR lpStrting = (LPSTR)malloc(StringSize+1);

	GetDlgItemTextA(hWndAW, nIDDlgItem, lpStrting, StringSize+1);

	return lpStrting;
}


void _stdcall AirWindow::_AddFormatString(int nIDDlgItem, char *_Format, DWORD *dwArgument){
	char temp[MaxString] = {0};
	Air::_CreateFormatString(temp, _Format, dwArgument);
	AddString(nIDDlgItem, temp, true);
}


void _stdcall AirWindow::AddFormatString(int nIDDlgItem, char *_Format, ...){
	_AddFormatString(nIDDlgItem, _Format, (DWORD *)(&_Format+1));
}